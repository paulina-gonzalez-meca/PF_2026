
#include <RF24.h>

#define PIN_PULSADOR PIN_PB0
#define PIN_LED      PIN_PB1
#define PIN_CSN      PIN_PA7
#define PIN_CE       PIN_PA3

RF24 radio(PIN_CE, PIN_CSN);

uint32_t ultimoIntento = 0;
uint32_t ultimoCambioLED = 0;

bool inicializada = false;
bool estadoLED = false;

void setup() {
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_CSN, OUTPUT);
  pinMode(PIN_CE, OUTPUT);

  digitalWrite(PIN_CSN, HIGH);
  digitalWrite(PIN_CE, LOW);
  digitalWrite(PIN_LED, LOW);

  // Permitir el primer intento inmediatamente.
  ultimoIntento = millis() - 1000;
}

void loop() {
  uint32_t ahora = millis();

  if (!inicializada &&
      (uint32_t)(ahora - ultimoIntento) >= 1000) {
    ultimoIntento = ahora;

    inicializada = radio.begin();

    if (inicializada) {
      radio.setChannel(76);
      radio.setDataRate(RF24_1MBPS);
      radio.setPALevel(RF24_PA_MIN);
      radio.setAutoAck(false);
      radio.openWritingPipe(0xF0F0F0F0E1LL);
      radio.stopListening();

      digitalWrite(PIN_LED, HIGH);
    }
  }

  if (!inicializada &&
      (uint32_t)(ahora - ultimoCambioLED) >= 250) {
    ultimoCambioLED = ahora;
    estadoLED = !estadoLED;
    digitalWrite(PIN_LED, estadoLED);
  }
}
