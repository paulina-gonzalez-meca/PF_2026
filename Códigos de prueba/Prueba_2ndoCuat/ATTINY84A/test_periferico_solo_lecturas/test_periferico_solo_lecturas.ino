#include <RF24.h>
#include <stdint.h>

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
RF24 radio(PIN_CE, PIN_CSN, 1000000);
const uint64_t direccionConectar = 0xF0F0F0F0E0LL;


float tempTermistor;
bool puerta;  // en donde se guardará el valor del pin puerta.
float nivelBateria;

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
  radio.setPALevel(RF24_PA_MAX);

  radio.stopListening();
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  tempTermistor = leerTemperaturaTermistor();
  puerta = lecturaPuerta();
  nivelBateria = lecturaBaterias();
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
  return bateria;
  //para ponerlo en el char
  //dtostrf(bateria, 4, 2, nivelBateria);  // float, minimo, decimales, char en donde se guarda
};