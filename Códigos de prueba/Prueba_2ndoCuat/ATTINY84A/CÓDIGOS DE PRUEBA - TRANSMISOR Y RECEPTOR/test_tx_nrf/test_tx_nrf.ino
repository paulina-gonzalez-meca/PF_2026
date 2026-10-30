// ==========================================
// ATtiny84A - nRF24L01 TRANSMITTER
// ATtinyCore
// ==========================================

#include <SPI.h>
#include <RF24.h>
#include <stdint.h>

// ------------------------------------------
// PIN CONFIGURATION
// ------------------------------------------
#define CE_PIN  PIN_PA3
#define CSN_PIN PIN_PA7
#define PIN_LED PIN_PB1

RF24 radio(CE_PIN, CSN_PIN, 1000000);

// ------------------------------------------
// RADIO CONFIGURATION
// ------------------------------------------
const uint64_t address = 0xF0F0F0F0E1LL;
const uint8_t CANAL_RF = 76;

char txMessage[32] = "ATtiny84 test";

bool radioReady = false;

void setup() {

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  // Initialize nRF24L01
  radioReady = radio.begin();

  // Explicit shared configuration
  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(sizeof(txMessage));

  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(address);

  radio.stopListening();

  // Initialization successful
  //digitalWrite(PIN_LED, HIGH);

  delay(1000);
  digitalWrite(PIN_LED, LOW);
}

void loop() {

  bool success = radio.write(txMessage, sizeof(txMessage));

  if (success) {

    digitalWrite(PIN_LED, HIGH);

    delay(100);

    digitalWrite(PIN_LED, LOW);

  } else {

    digitalWrite(PIN_LED, HIGH);

  }

  delay(1000);
}