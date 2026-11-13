#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>

#define CANAL_RADIO 76           // <-- PON AQUÍ EL MISMO CANAL QUE EN EL ESP32 (76 por defecto o 108)
#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 10000
#define TIEMPO_ENVIO_DATOS 60000  // Envía telemetría cada 1 minuto

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

RF24 radio(PIN_CE, PIN_CSN);
const uint64_t direccionConectar = 0xF0F0F0F0E0LL;

char mensaje[10];
char mensajeRecibidoNRF[32];
char trama[28];

unsigned long tiempoInicio = 0;
unsigned long ultimoEnvio = 0;
unsigned long ultimoEnvioDatos = 0;

int tipoSensor1 = 1;
int tipoSensor2 = 0;
char nombre[4];

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

  if (!radio.begin()) {
    while (1) {
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
      delay(150);
    }
  }

  radio.setChannel(CANAL_RADIO);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(0, direccionConectar);  // Pipe 0 evita duplicar dirección con Pipe 1
  radio.startListening();

  // Arma "#C,1,0*" sin usar snprintf
  mensaje[0] = '#';
  mensaje[1] = 'C';
  mensaje[2] = ',';
  mensaje[3] = '0' + tipoSensor1;
  mensaje[4] = ',';
  mensaje[5] = '0' + tipoSensor2;
  mensaje[6] = '*';
  mensaje[7] = '\0';

  digitalWrite(PIN_LED, LOW);
  tiempoInicio = millis();
  ultimoEnvio = tiempoInicio - TIEMPO_CONECTAR;
}

void loop() {
  tempTermistor = leerTemperaturaTermistor();
  puerta = lecturaPuerta();
  porcentajeBateria = lecturaBaterias();

  if (estadoActual == CONFIGURACION) {
    configuracion();
  } else {
    enviarDatosCentral();
    recibirDatosCentral();
  }
}

void configuracion() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoInicio < TIMEOUT_CONECTAR) {
    digitalWrite(PIN_LED, HIGH);
    if (conectar(mensaje, direccionConectar)) {
      digitalWrite(PIN_LED, LOW);
      ultimoEnvioDatos = millis() - TIEMPO_ENVIO_DATOS;  // Primer envío inmediato
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

void enviarDatosCentral() {
  unsigned long ahora = millis();

  if (ahora - ultimoEnvioDatos >= TIEMPO_ENVIO_DATOS) {
    ultimoEnvioDatos = ahora;

    // Arma "#P1,BAT,VALORSENSOR1,VALORSENSOR2*" sin snprintf ni dtostrf
    char buf[8];
    trama[0] = '#';
    trama[1] = '\0';

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

    // Parpadeo breve de confirmación visual al transmitir
    digitalWrite(PIN_LED, HIGH);
    radio.stopListening();
    radio.openWritingPipe(direccionConectar);
    radio.write(trama, strlen(trama) + 1);
    radio.startListening();
    digitalWrite(PIN_LED, LOW);
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

  if (lenMsg > lenNombre + 3 &&
      mensajeRecibidoNRF[0] == '#' &&
      strncmp(&mensajeRecibidoNRF[1], nombre, lenNombre) == 0 &&
      mensajeRecibidoNRF[lenNombre + 1] == ',' &&
      mensajeRecibidoNRF[lenMsg - 1] == '*') {

    const char* comando = &mensajeRecibidoNRF[lenNombre + 2];

    if (strncmp(comando, "REQ*", 4) == 0) {
      ultimoEnvioDatos = millis() - TIEMPO_ENVIO_DATOS;
    } else if (strncmp(comando, "LED,1*", 6) == 0) {
      digitalWrite(PIN_LED, HIGH);
    } else if (strncmp(comando, "LED,0*", 6) == 0) {
      digitalWrite(PIN_LED, LOW);
    }
  }
}

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
  int bateriaAnalog = analogRead(PIN_ENERGIA);
  float bateriaPin = bateriaAnalog * (3.0 / 1023.0);
  float bateria = (bateriaPin / 0.54) * 3.0;
  int bateriaPorcentaje = (int)((bateria / 3.0) * 100.0);
  return constrain(bateriaPorcentaje, 0, 100);
}