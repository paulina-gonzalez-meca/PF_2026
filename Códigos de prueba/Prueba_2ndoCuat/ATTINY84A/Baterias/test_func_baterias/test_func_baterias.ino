// CÓDIGO CON LECTURA DE PIN BATERÍA
// Función void lecturaBaterias(){}
#include <SPI.h>
#include <RF24.h>
#include <stdint.h>

// ------------------------------------------
// PIN CONFIGURATION
// ------------------------------------------
#define CE_PIN PIN_PA3
#define CSN_PIN PIN_PA7
#define PIN_LED PIN_PB1
#define PIN_ENERGIA PIN_PA0


RF24 radio(CE_PIN, CSN_PIN, 1000000);

// ------------------------------------------
// RADIO CONFIGURATION
// ------------------------------------------
const uint64_t direccionConectar = 0xF0F0F0F0E0LL;
//const uint8_t CANAL_RF = 76;

char mensaje[32] = "HOLA";
float bateriaAnalog;
float bateria;
float bateriaPin;
bool radioReady = false;

void setup() {

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  // Initialize nRF24L01
  radio.begin();

  // Explicit shared configuration

  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_MAX);

  radio.stopListening();

  // Initialization successful
  //digitalWrite(PIN_LED, HIGH);

  delay(1000);
  digitalWrite(PIN_LED, LOW);
}

void loop() {

  lecturaBaterias();


  bool success = radio.write(mensaje, sizeof(mensaje));

  if (success) {

    digitalWrite(PIN_LED, HIGH);

    delay(100);

    digitalWrite(PIN_LED, LOW);

  } else {

    digitalWrite(PIN_LED, HIGH);
  }

  delay(1000);
}

void lecturaBaterias(){

   bateriaAnalog = analogRead(PIN_ENERGIA);
  bateriaPin = bateriaAnalog * (3 / 1023.0);
  bateria = (bateriaPin / 0.54) * 3;
  dtostrf(bateria, 4, 2, mensaje);  // float, minimo, decimales, char en donde se guarda

};

 