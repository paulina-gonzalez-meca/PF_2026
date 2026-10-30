
#include <SPI.h>
#include <RF24.h>
#include <stdint.h>

#define CE_PIN   PIN_PA3
#define CSN_PIN  PIN_PA7
#define PIN_LED  PIN_PB1
#define PIN_IRQ  PIN_PB2

RF24 radio(CE_PIN, CSN_PIN, 1000000);

const uint64_t address = 0xF0F0F0F0E1LL;
const uint8_t CANAL_RF = 76;

// bool radioReady = false;

unsigned long ledTimer = 0;
const unsigned long LED_TIME = 300;

void setup() {

  pinMode(PIN_LED, OUTPUT);
  //pinMode(PIN_IRQ, INPUT_PULLUP);

  digitalWrite(PIN_LED, HIGH);

  radio.begin();

  /*
   if (!radioReady) {
    digitalWrite(PIN_LED, HIGH); // Initialization failure
    return;
  }
  */

  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.setAutoAck(false);
  radio.disableDynamicPayloads();
  radio.setPayloadSize(32);
  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(1, address);

  radio.maskIRQ(true, true, false);

  radio.startListening();
  digitalWrite(PIN_LED, LOW);
}

void loop() {

  /* if (!radioReady) {
    return;
  } */

    if (radio.available()) {

      char mensajeRX[33] = {0};

      radio.read(mensajeRX, 32);

      digitalWrite(PIN_LED, HIGH);
      ledTimer = millis();
    }
  

  if (digitalRead(PIN_LED) == HIGH &&
      millis() - ledTimer >= LED_TIME) {

    digitalWrite(PIN_LED, LOW);
  }
}
