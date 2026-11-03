
// ATtiny84A - NRF24L01 ECHO
// Non-blocking RX -> WAIT_TX -> TX -> RX

#include <SPI.h>
#include <RF24.h>
#include <stdint.h>

#define CE_PIN PIN_PA3
#define CSN_PIN PIN_PA7
#define PIN_LED PIN_PB1

RF24 radio(CE_PIN, CSN_PIN, 1000000);

const uint64_t RX_ADDRESS = 0xF0F0F0F0E1LL;
const uint64_t TX_ADDRESS = 0xF0F0F0F0E2LL;

const uint8_t CANAL_RF = 76;
const uint8_t PAYLOAD_SIZE = 32;

enum RadioState {
  RX,
  WAIT_TX,
  TX
};

RadioState state = RX;

uint8_t messageBuffer[PAYLOAD_SIZE];

unsigned long txTimer = 0;
unsigned long ledTimer = 0;

const unsigned long TX_DELAY = 3;
const unsigned long LED_TIME = 150;

void setup() {

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  radio.begin();

  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);

  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(PAYLOAD_SIZE);

  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(1, RX_ADDRESS);
  radio.openWritingPipe(TX_ADDRESS);

  radio.maskIRQ(false, false, false);

  digitalWrite(PIN_LED, HIGH);
  radio.stopListening();
  radio.startWrite("HELLO FROM ATTINY", PAYLOAD_SIZE, false);
  radio.startListening();
}

void loop() {

  // --------------------------------
  // RX
  // --------------------------------

  if (state == RX) {

    if (radio.available()) {

      radio.read(messageBuffer, PAYLOAD_SIZE);

      digitalWrite(PIN_LED, HIGH);
      ledTimer = millis();

      radio.stopListening();

      txTimer = millis();
      state = WAIT_TX;
    }
  }

  // --------------------------------
  // WAIT BEFORE TRANSMISSION
  // --------------------------------

  if (state == WAIT_TX) {

    if (millis() - txTimer >= TX_DELAY) {

      // Clear previous interrupt flags
      bool tx_ok, tx_fail, rx_ready;
      radio.whatHappened(tx_ok, tx_fail, rx_ready);

      radio.startWrite(messageBuffer, PAYLOAD_SIZE, false);
      
      state = TX;
    }
  }

  // --------------------------------
  // TX
  // --------------------------------

  if (state == TX) {

    bool tx_ok, tx_fail, rx_ready;

    radio.whatHappened(tx_ok, tx_fail, rx_ready);

    if (tx_ok) {
      radio.startListening();

      state = RX;
    }
  }

  // --------------------------------
  // LED
  // --------------------------------

  if (digitalRead(PIN_LED) == HIGH && millis() - ledTimer >= LED_TIME) {

    digitalWrite(PIN_LED, LOW);
  }
}
