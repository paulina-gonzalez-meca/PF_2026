
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// ---------------- CONFIGURACION ----------------

#define CANAL_RADIO 108

#define TIEMPO_CONECTAR 2000UL
#define TIMEOUT_CONECTAR 10000UL
#define TIEMPO_ENVIO_DATOS 60000UL

#define TIMEOUT_ACK_DATOS 500UL
#define TIMEOUT_TX 100UL
#define MAX_INTENTOS 5

#define PIN_PULSADOR PIN_PB0
#define PIN_LED PIN_PB1
#define PIN_CSN PIN_PA7
#define PIN_CE PIN_PA3

#define PIN_TERMISTOR PIN_PA1
#define PIN_PUERTA PIN_PA2
#define PIN_ENERGIA PIN_PA0

#define DEBOUNCE 20UL

#define ABIERTO 1
#define CERRADO 0

// ---------------- ESTADOS ----------------

enum EstadoPeriferico {
  ESPERA_BOTON,
  CONECTANDO,
  TX_EN_CURSO,
  ESPERA_ASIGNACION,
  OPERANDO,
  ESPERA_ACK_DATOS
};

enum TipoTransmision {
  TX_NINGUNA,
  TX_CONEXION,
  TX_CONFIRMACION,
  TX_TELEMETRIA
};

EstadoPeriferico estadoActual = ESPERA_BOTON;
TipoTransmision tipoTX = TX_NINGUNA;

// ---------------- NRF ----------------

RF24 radio(PIN_CE, PIN_CSN, 2000000);

const uint64_t direccionA = 0xF0F0F0F0E0LL;
const uint64_t direccionB = 0xF0F0F0F0E1LL;

bool radioLista = false;

char mensaje[32];
char mensajeRecibidoNRF[32];
char trama[32];
char bufferTX[32];

char nombre[4] = { 0 };

// ---------------- TEMPORIZADORES ----------------

unsigned long tiempoInicioConexion = 0;
unsigned long ultimoIntentoConexion = 0;
unsigned long ultimoEnvioDatos = 0;
unsigned long tiempoInicioTX = 0;
unsigned long tiempoInicioACK = 0;

// ---------------- PULSADOR ----------------

bool lecturaAnteriorBoton = HIGH;
bool estadoBoton = HIGH;

unsigned long tiempoDebounce = 0;

// ---------------- SENSORES ----------------

int tipoSensor1 = 1;
int tipoSensor2 = 0;

float tempTermistor = 0.0;
int porcentajeBateria = 0;

bool puerta = CERRADO;

bool estadoPuerta = CERRADO;
bool ultimaLecturaPuerta = CERRADO;

unsigned long ultimoTiempoDebounce = 0;

// ---------------- TRANSMISION ----------------

int intentosDatos = 0;

bool datosPendientes = false;

bool flagConectado = false;


// =================================================
// SETUP
// =================================================

void setup() {

  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PUERTA, INPUT_PULLUP);

  digitalWrite(PIN_LED, HIGH);


  radio.begin();
  radio.setAutoAck(true);
  radio.setRetries(2, 5);
  radio.enableDynamicPayloads();
  radio.setChannel(CANAL_RADIO);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, direccionA);
  radio.openWritingPipe(direccionB);

  radio.startListening();


  // Arma mensaje de conexion

  strcpy(mensaje, "#C,");

  mensaje[3] = '0' + tipoSensor1;
  mensaje[4] = ',';
  mensaje[5] = '0' + tipoSensor2;
  mensaje[6] = '*';
  mensaje[7] = '\0';
}


// =================================================
// LOOP PRINCIPAL
// =================================================

void loop() {

  // Lecturas de sensores

  tempTermistor = leerTemperaturaTermistor();
  puerta = lecturaPuerta();
  porcentajeBateria = lecturaBaterias();

  // Control del pulsador

  leerPulsador();

  // Recepcion NRF

  recibirNRF();

  // Maquina de estados

  switch (estadoActual) {

    case ESPERA_BOTON:

      digitalWrite(PIN_LED, LOW);

      break;


    case CONECTANDO:

    case ESPERA_ASIGNACION:

      procesoConexion();

      break;


    case TX_EN_CURSO:

      comprobarTX();

      break;


    case OPERANDO:

      enviarDatosCentral();

      break;


    case ESPERA_ACK_DATOS:

      comprobarACKDatos();

      break;
  }
}


// =================================================
// PULSADOR
// =================================================

void leerPulsador() {

  bool lectura = digitalRead(PIN_PULSADOR);

  if (lectura != lecturaAnteriorBoton) {
    tiempoDebounce = millis();
  }

  if (millis() - tiempoDebounce >= DEBOUNCE) {

    if (lectura != estadoBoton) {

      estadoBoton = lectura;

      // Pulsacion detectada

      if (estadoBoton == LOW) {

        if (estadoActual == ESPERA_BOTON) {

          iniciarConexion();
        }
      }
    }
  }

  lecturaAnteriorBoton = lectura;
}


// =================================================
// INICIAR CONEXION
// =================================================

void iniciarConexion() {

  if (!radioLista) {
    return;
  }

  flagConectado = false;

  tiempoInicioConexion = millis();

  ultimoIntentoConexion = millis() - TIEMPO_CONECTAR;

  estadoActual = CONECTANDO;

  digitalWrite(PIN_LED, HIGH);
}


// =================================================
// PROCESO DE CONEXION
// =================================================

void procesoConexion() {

  unsigned long ahora = millis();

  // Timeout general de conexion

  if (ahora - tiempoInicioConexion >= TIMEOUT_CONECTAR) {

    cancelarConexion();

    return;
  }

  // Reintentar solicitud cada 2 segundos

  if (ahora - ultimoIntentoConexion >= TIEMPO_CONECTAR) {

    ultimoIntentoConexion = ahora;

    iniciarTX(mensaje, TX_CONEXION);
  }
}


// =================================================
// CANCELAR CONEXION
// =================================================

void cancelarConexion() {

  radio.stopListening();
  radio.startListening();

  estadoActual = ESPERA_BOTON;

  tipoTX = TX_NINGUNA;

  flagConectado = false;

  digitalWrite(PIN_LED, LOW);
}


// =================================================
// INICIAR TRANSMISION NO BLOQUEANTE
// =================================================

void iniciarTX(const char *contenido, TipoTransmision tipo) {

  if (estadoActual == TX_EN_CURSO) {
    return;
  }

  memset(bufferTX, 0, sizeof(bufferTX));

  strncpy(bufferTX, contenido, sizeof(bufferTX) - 1);

  // Limpiar flags anteriores de interrupcion

  bool tx_ok;
  bool tx_fail;
  bool rx_ready;

  radio.whatHappened(tx_ok, tx_fail, rx_ready);

  radio.stopListening();

  radio.openWritingPipe(direccionA);

  tipoTX = tipo;

  tiempoInicioTX = millis();

  estadoActual = TX_EN_CURSO;

  // Inicia la transmision sin esperar su finalizacion

  radio.startWrite(bufferTX, 32, false);
}


// =================================================
// COMPROBAR FINALIZACION DE TRANSMISION
// =================================================

void comprobarTX() {

  bool tx_ok = false;
  bool tx_fail = false;
  bool rx_ready = false;

  radio.whatHappened(tx_ok, tx_fail, rx_ready);

  if (tx_ok) {

    radio.startListening();

    finalizarTX(true);

    return;
  }

  if (tx_fail || millis() - tiempoInicioTX >= TIMEOUT_TX) {

    radio.flush_tx();

    radio.startListening();

    finalizarTX(false);
  }
}


// =================================================
// FINALIZAR TRANSMISION
// =================================================

void finalizarTX(bool exitoso) {

  TipoTransmision tipoAnterior = tipoTX;

  tipoTX = TX_NINGUNA;

  // Solicitud inicial de conexion

  if (tipoAnterior == TX_CONEXION) {

    estadoActual = ESPERA_ASIGNACION;

    return;
  }

  // Confirmacion de asignacion al central

  if (tipoAnterior == TX_CONFIRMACION) {

    if (exitoso) {

      flagConectado = true;

      estadoActual = OPERANDO;

      // Primer envio inmediato

      ultimoEnvioDatos = millis() - TIEMPO_ENVIO_DATOS;

      digitalWrite(PIN_LED, LOW);

    } else {

      estadoActual = ESPERA_ASIGNACION;
    }

    return;
  }

  // Transmision de telemetria

  if (tipoAnterior == TX_TELEMETRIA) {

    if (exitoso) {

      tiempoInicioACK = millis();

      estadoActual = ESPERA_ACK_DATOS;

    } else {

      estadoActual = OPERANDO;

      reintentarDatos();
    }
  }
}


// =================================================
// RECEPCION NRF
// =================================================

void recibirNRF() {

  if (!radioLista || !radio.available()) {
    return;
  }

  memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));

  radio.read(mensajeRecibidoNRF, 32);

  mensajeRecibidoNRF[31] = '\0';


  // ---------------------------------------------
  // CONFIRMACION DE CONEXION
  // ---------------------------------------------

  if (estadoActual == ESPERA_ASIGNACION || estadoActual == CONECTANDO) {

    if (strncmp(mensajeRecibidoNRF, "#C,P", 4) == 0) {

      bool formatoValido = false;

      memset(nombre, 0, sizeof(nombre));

      nombre[0] = 'P';
      nombre[1] = mensajeRecibidoNRF[4];

      if (mensajeRecibidoNRF[4] >= '0' && mensajeRecibidoNRF[4] <= '9') {

        if (mensajeRecibidoNRF[5] == '*') {

          formatoValido = true;

        } else if (
          mensajeRecibidoNRF[5] >= '0' && mensajeRecibidoNRF[5] <= '9' && mensajeRecibidoNRF[6] == '*') {

          nombre[2] = mensajeRecibidoNRF[5];

          formatoValido = true;
        }
      }

      if (formatoValido) {

        // El central asigno un identificador

        iniciarTX("OK", TX_CONFIRMACION);

        return;
      }
    }
  }


  // ---------------------------------------------
  // CONFIRMACION DE TELEMETRIA
  // ---------------------------------------------

  if (estadoActual == ESPERA_ACK_DATOS) {

    char confirmacion[16];

    memset(confirmacion, 0, sizeof(confirmacion));

    strcpy(confirmacion, "#");
    strcat(confirmacion, nombre);
    strcat(confirmacion, ",recibido*");

    if (strcmp(mensajeRecibidoNRF, confirmacion) == 0) {

      // Datos confirmados por el central

      datosPendientes = false;

      intentosDatos = 0;

      ultimoEnvioDatos = millis();

      estadoActual = OPERANDO;

      digitalWrite(PIN_LED, LOW);
    }
  }
}


// =================================================
// ENVIO DE DATOS AL CENTRAL
// =================================================

void enviarDatosCentral() {

  unsigned long ahora = millis();

  if (!flagConectado) {
    return;
  }

  if (ahora - ultimoEnvioDatos >= TIEMPO_ENVIO_DATOS) {

    if (!datosPendientes) {

      // Construir trama una sola vez

      construirTrama();

      datosPendientes = true;

      intentosDatos = 0;
    }

    iniciarEnvioDatos();
  }
}


// =================================================
// CONSTRUIR TRAMA
// =================================================

void construirTrama() {

  char buf[8];

  memset(trama, 0, sizeof(trama));

  strcat(trama, "#");
  strcat(trama, nombre);
  strcat(trama, ",");

  itoa(porcentajeBateria, buf, 10);
  strcat(trama, buf);
  strcat(trama, ",");

  int tempDecimas = (int)(tempTermistor * 10.0);

  if (tempDecimas < 0) {

    strcat(trama, "-");

    tempDecimas = -tempDecimas;
  }

  itoa(tempDecimas / 10, buf, 10);
  strcat(trama, buf);

  strcat(trama, ".");

  itoa(tempDecimas % 10, buf, 10);
  strcat(trama, buf);

  strcat(trama, ",");

  strcat(trama, puerta ? "1*" : "0*");
}


// =================================================
// INICIAR ENVIO DE DATOS
// =================================================

void iniciarEnvioDatos() {

  if (intentosDatos >= MAX_INTENTOS) {
    return;
  }

  intentosDatos++;

  digitalWrite(PIN_LED, HIGH);

  iniciarTX(trama, TX_TELEMETRIA);
}


// =================================================
// COMPROBAR CONFIRMACION DE DATOS
// =================================================

void comprobarACKDatos() {

  if (millis() - tiempoInicioACK >= TIMEOUT_ACK_DATOS) {

    reintentarDatos();
  }
}


// =================================================
// REINTENTAR ENVIO
// =================================================

void reintentarDatos() {

  if (intentosDatos < MAX_INTENTOS) {

    // Reintento inmediato, sin esperar 60 segundos

    estadoActual = OPERANDO;

    iniciarEnvioDatos();

  } else {

    // Se agotaron los cinco intentos

    datosPendientes = false;

    intentosDatos = 0;

    ultimoEnvioDatos = millis();

    estadoActual = OPERANDO;

    digitalWrite(PIN_LED, LOW);
  }
}


// =================================================
// SENSORES
// =================================================

float leerTemperaturaTermistor() {

  int adc = analogRead(PIN_TERMISTOR);

  float temperatura =
    0.0000215256 * adc * adc - 0.129388 * adc + 197.010;

  return temperatura - 4.0;
}


bool lecturaPuerta() {

  bool lecturaPinPuerta = digitalRead(PIN_PUERTA);

  if (lecturaPinPuerta != ultimaLecturaPuerta) {

    ultimoTiempoDebounce = millis();
  }

  if (millis() - ultimoTiempoDebounce >= DEBOUNCE) {

    if (lecturaPinPuerta != estadoPuerta) {

      estadoPuerta = lecturaPinPuerta;
    }
  }

  ultimaLecturaPuerta = lecturaPinPuerta;

  return estadoPuerta;
}


int lecturaBaterias() {

  int bateriaPorcentaje(analogRead(PIN_ENERGIA) / 1023 * 100);
  return bateriaPorcentaje;
}
