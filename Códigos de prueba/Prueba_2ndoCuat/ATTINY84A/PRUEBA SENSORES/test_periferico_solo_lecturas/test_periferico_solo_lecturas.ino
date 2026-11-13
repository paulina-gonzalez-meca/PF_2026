#include <RF24.h>
#include <stdint.h>

#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 10000
#define TIEMPO_ENVIO_DATOS 5000
// #P1,BAT,VALORSENSOR,VALORSENSOR2*

// pines para probar en esp32:
// CE --> 4
// CSN --> 5
// LED --> 2
// PULSADOR --> 16

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

// radio configuration
RF24 radio(PIN_CE, PIN_CSN);
const uint64_t direccionConectar = 0xF0F0F0F0E0LL;
// mensaje buffer
char mensaje[8];
char mensajeRecibidoNRF[32];
char trama[32];

unsigned long tiempoInicio = 0;
unsigned long ultimoEnvio = 0;

// datos que se le enviarán al central
int tipoSensor1 = 1;
int tipoSensor2 = 0;
char nombre[4];

float tempTermistor;
float porcentajeBateria;
bool puerta;  // en donde se guardará el valor del pin puerta.


bool estadoPuerta = CERRADO;
bool ultimaLecturaPuerta;
unsigned long ultimoTiempoDebounce = 0;



void setup() {
  pinMode(PIN_PUERTA, INPUT_PULLUP);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  radio.begin();
  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_LOW);  // Sube a RF24_PA_MAX solo cuando tengas capacitor en VCC/GND
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();

  snprintf(mensaje, sizeof(mensaje), "#C,%d,%d*", tipoSensor1, tipoSensor2);
  digitalWrite(PIN_LED, LOW);
  tiempoInicio = millis();
  ultimoEnvio = tiempoInicio - TIEMPO_CONECTAR;
}

void loop() {

  configuracion();
  tempTermistor = leerTemperaturaTermistor();
  puerta = lecturaPuerta();
  porcentajeBateria = lecturaBaterias();
}

float leerTemperaturaTermistor() {

  int adc = analogRead(PIN_TERMISTOR);

  // Función cuadrática obtenida en base a los valores calculados del termisor, comparados con la temperatura del DHT11
  float temperatura = 0.0000215256 * adc * adc - 0.129388 * adc + 197.010;

  float tempAjustada = temperatura - 4;  // la temperatura obtenida tenía un offset de 4°C

  return tempAjustada;
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
float lecturaBaterias() {
  char nivelBateria[32];
  int bateriaAnalog = analogRead(PIN_ENERGIA);
  float bateriaPin = bateriaAnalog * (3 / 1023.0);
  float bateria = (bateriaPin / 0.54) * 3;
  // lo pasé a porcentaje
  float bateriaPorcentaje = (bateria / 3) * 100;
  return bateriaPorcentaje;
  //para ponerlo en el char
  //dtostrf(bateria, 4, 2, nivelBateria);  // float, minimo, decimales, char en donde se guarda
}

void configuracion() {

  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoInicio < TIMEOUT_CONECTAR) {
    digitalWrite(PIN_LED, HIGH);
    if (conectar(mensaje, direccionConectar)) {

      digitalWrite(PIN_LED, LOW);
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

    // Validación directa sin while: acepta "#C,P<1 dígito>*" o "#C,P<2 dígitos>*"
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