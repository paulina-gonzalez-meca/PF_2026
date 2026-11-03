#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TIEMPO_LED 1000
#define PRESSED 0
#define NOT_PRESSED 1
#define PIN_SW PIN_PB0
#define PIN_CE PIN_PA3
#define PIN_CSN PIN_PA7
#define PIN_LED PIN_PB1

uint64_t direccionConectar = 0xF0F0F0F0E0LL;
char mensajeRecibidoNRF[32];
char mensaje[] = "hola";
bool flagSW = 0;
int tiempo = 0;

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  //Serial.begin(9600);
  //SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  radio.begin();
  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SW, INPUT_PULLUP);

  digitalWrite(PIN_LED, LOW);
}

void loop() {
  if (millis() - tiempo >= TIEMPO_LED) {
    digitalWrite(PIN_LED, LOW);
  }
  if (flagSW == 0) {
    if (digitalRead(PIN_SW) == PRESSED) {
      flagSW = 1;
    }
  } else {
    if (digitalRead(PIN_SW) == NOT_PRESSED) {
      flagSW = 0;
      radio.stopListening();
      radio.write(mensaje, strlen(mensaje) + 1);
      radio.startListening();
    }
  }
  if (radio.available() > 0) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    mensajeCentralConectar.trim();
    if (mensajeCentralConectar == "hola") {
      //Serial.println(mensajeCentralConectar);
      digitalWrite(PIN_LED, HIGH);
      tiempo = millis();
    }
  }
}
