#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>

#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 10000
#define TIEMPO_ENVIO_DATOS 10000  // Intervalo de envío de telemetría (10 s)

#define PIN_PULSADOR PIN_PB0
#define PIN_LED PIN_PB1

#define PIN_CSN PIN_PA7
#define PIN_CE PIN_PA3

#define PIN_TERMISTOR PIN_PA1
#define PIN_PUERTA PIN_PA2
#define PIN_ENERGIA PIN_PA0

#define ABIERTO 1
#define CERRADO 0
#define DEBOUNCE 20

enum EstadoPeriferico {
  CONFIGURACION,
  OPERANDO
};
EstadoPeriferico estadoActual = CONFIGURACION;

// Configuración de radio
RF24 radio(PIN_CE, PIN_CSN);
const uint64_t direccionConectar = 0xF0F0F0F0E0LL;

// Buffers de comunicación
char mensaje[10];
char mensajeRecibidoNRF[32];
char trama[32];

unsigned long tiempoInicio = 0;
unsigned long ultimoEnvio = 0;
unsigned long ultimoEnvioDatos = 0;

// Datos de identificación y sensores
int tipoSensor1 = 1;
int tipoSensor2 = 0;
char nombre[4];  // Ej: "P1" o "P12"

float tempTermistor = 0.0;
int porcentajeBateria = 0;
bool puerta = CERRADO;

bool estadoPuerta = CERRADO;
bool ultimaLecturaPuerta = CERRADO;
unsigned long ultimoTiempoDebounce = 0;

void setup() {
  pinMode(PIN_PUERTA, INPUT_PULLUP);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  radio.begin();
  radio.setChannel(108);  // Debe coincidir con el canal de la central
  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();

  snprintf(mensaje, sizeof(mensaje), "#C,%d,%d*", tipoSensor1, tipoSensor2);

  digitalWrite(PIN_LED, LOW);
  tiempoInicio = millis();
  ultimoEnvio = tiempoInicio - TIEMPO_CONECTAR;
}

void loop() {
  // Lectura continua y no bloqueante de sensores
  tempTermistor = leerTemperaturaTermistor();
  puerta = lecturaPuerta();
  porcentajeBateria = lecturaBaterias();

  switch (estadoActual) {
    case CONFIGURACION:
      configuracion();
      break;

    case OPERANDO:
      enviarDatosCentral();
      recibirDatosCentral();
      break;
  }
}

// --- FASE 1: CONFIGURACIÓN INICIAL ---

void configuracion() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoInicio < TIMEOUT_CONECTAR) {
    digitalWrite(PIN_LED, HIGH);
    if (conectar(mensaje, direccionConectar)) {
      digitalWrite(PIN_LED, LOW);
      ultimoEnvioDatos = millis() - TIEMPO_ENVIO_DATOS;  // Fuerza primer envío inmediato
      estadoActual = OPERANDO;
    }
  } else {
    digitalWrite(PIN_LED, LOW);
    tiempoInicio = millis();
  }
}

bool conectar(char mensajeConectar[], uint64_t direccion) {
  unsigned long ahora = millis();

  if (ahora - ultimoEnvio >= TIEMPO_CONECTAR) {
    ultimoEnvio = ahora;
    radio.stopListening();
    radio.openWritingPipe(direccion);
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    radio.startListening();
  }

  if (radio.available()) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF) - 1);

    if (strncmp(mensajeRecibidoNRF, "#C,P", 4) == 0 && isDigit(mensajeRecibidoNRF[4])) {
      bool formatoValido = false;
      memset(nombre, 0, sizeof(nombre));
      nombre[0] = 'P';
      nombre[1] = mensajeRecibidoNRF[4];

      if (mensajeRecibidoNRF[5] == '*') {
        formatoValido = true;
      } else if (isDigit(mensajeRecibidoNRF[5]) && mensajeRecibidoNRF[6] == '*') {
        nombre[2] = mensajeRecibidoNRF[5];
        formatoValido = true;
      }

      if (formatoValido) {
        radio.stopListening();
        radio.openWritingPipe(direccion);
        radio.write("OK", 3);
        radio.startListening();
        return true;
      }
    }
  }

  return false;
}

// --- FASE 2: TRANSMISIÓN Y RECEPCIÓN NO BLOQUEANTE ---

void enviarDatosCentral() {
  unsigned long ahora = millis();

  if (ahora - ultimoEnvioDatos >= TIEMPO_ENVIO_DATOS) {
    ultimoEnvioDatos = ahora;

    char strTemp[8];
    dtostrf(tempTermistor, 1, 1, strTemp);  // Convierte float a char con 1 decimal (ej: "24.5")

    // Arma la trama: #P1,BAT,VALORSENSOR1,VALORSENSOR2*
    memset(trama, 0, sizeof(trama));
    snprintf(trama, sizeof(trama), "#%s,%d,%s,%d*",
             nombre,
             porcentajeBateria,
             strTemp,
             puerta ? 1 : 0);

    radio.stopListening();
    radio.openWritingPipe(direccionConectar);
    radio.write(trama, strlen(trama) + 1);
    radio.startListening();
  }
}

void recibirDatosCentral() {
  if (!radio.available()) {
    return;
  }

  memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
  radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF) - 1);

  size_t lenNombre = strlen(nombre);
  size_t lenMsg = strlen(mensajeRecibidoNRF);

  // Verifica que el mensaje empiece con "#<nombre>," y termine en '*'
  // Ejemplos: "#P1,REQ*" o "#P1,LED,1*"
  if (lenMsg > lenNombre + 3 &&
      mensajeRecibidoNRF[0] == '#' &&
      strncmp(&mensajeRecibidoNRF[1], nombre, lenNombre) == 0 &&
      mensajeRecibidoNRF[lenNombre + 1] == ',' &&
      mensajeRecibidoNRF[lenMsg - 1] == '*') {

    const char* comando = &mensajeRecibidoNRF[lenNombre + 2];

    if (strncmp(comando, "REQ*", 4) == 0) {
      // Fuerza el envío inmediato de sensores en la siguiente iteración del loop
      ultimoEnvioDatos = millis() - TIEMPO_ENVIO_DATOS;
    } else if (strncmp(comando, "LED,1*", 6) == 0) {
      digitalWrite(PIN_LED, HIGH);
    } else if (strncmp(comando, "LED,0*", 6) == 0) {
      digitalWrite(PIN_LED, LOW);
    }
  }
}

// --- LECTURA DE SENSORES ---

float leerTemperaturaTermistor() {
  int adc = analogRead(PIN_TERMISTOR);
  float temperatura = 0.0000215256 * adc * adc - 0.129388 * adc + 197.010;
  return temperatura - 4.0;
}

bool lecturaPuerta() {
  bool lecturaPinPuerta = digitalRead(PIN_PUERTA);

  if (lecturaPinPuerta != ultimaLecturaPuerta) {
    ultimoTiempoDebounce = millis();
  }

  if ((millis() - ultimoTiempoDebounce) >= DEBOUNCE) {
    if (lecturaPinPuerta != estadoPuerta) {
      estadoPuerta = lecturaPinPuerta;
    }
  }

  ultimaLecturaPuerta = lecturaPinPuerta;
  return estadoPuerta;
}

int lecturaBaterias() {
int bateriaPorcentaje = (int)(analogRead(PIN_ENERGIA) * (100.0 / (1023.0 * 0.54)));
  return bateriaPorcentaje;
}