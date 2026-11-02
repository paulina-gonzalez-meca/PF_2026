
#include <SPI.h>
#include <RF24.h>
#include <stdint.h>
#include <string.h>

// ==========================================
// ATtiny84A - nRF24L01 RECEIVER
// ATtinyCore
// ==========================================

// PIN CONFIGURATION
#define CE_PIN   PIN_PA3
#define CSN_PIN  PIN_PA7
#define PIN_LED  PIN_PB1
#define PIN_IRQ  PIN_PB2

RF24 radio(CE_PIN, CSN_PIN, 1000000);

// RADIO CONFIGURATION
const uint64_t address = 0xF0F0F0F0E1LL;
const uint8_t CANAL_RF = 76;

bool radioReady = false;

unsigned long ledTimer = 0;
const unsigned long LED_TIME = 300;

void setup() {

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_IRQ, INPUT_PULLUP);

  digitalWrite(PIN_LED, HIGH);

  // Initialize radio
   radio.begin();


  // Shared configuration
  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(32);

  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  // Listen on pipe 1
  radio.openReadingPipe(1, address);

  // Only RX_DR should activate IRQ
  radio.maskIRQ(true, true, false);

  // Start receiving
  radio.startListening();
  digitalWrite(PIN_LED, LOW);
}

void loop() {

  // IRQ is ACTIVE LOW
  if (digitalRead(PIN_IRQ) == LOW) {

    if (radio.available()) {

      char mensajeRX[33] = {0};

      radio.read(mensajeRX, 32);
      mensajeRX[32] = '\0';

      // A packet was received
      digitalWrite(PIN_LED, HIGH);

      ledTimer = millis();
    }
  }

  // LED timeout (non-blocking)
  if (digitalRead(PIN_LED) == HIGH &&
      millis() - ledTimer >= LED_TIME) {

    digitalWrite(PIN_LED, LOW);
  }
}
