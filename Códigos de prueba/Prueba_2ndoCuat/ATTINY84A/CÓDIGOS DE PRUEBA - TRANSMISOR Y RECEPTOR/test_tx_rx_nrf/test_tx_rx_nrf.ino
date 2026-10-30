 // Código de prueba NRF ATTINY. Recibe mensaje, y transmite lo recibido.
 
#include <SPI.h>
#include <RF24.h>
#include <stdint.h>
#include <string.h>

// ==========================================
// ATtiny84A - NRF24L01 NON-BLOCKING RELAY
// ATtinyCore
// ==========================================

// PIN CONFIGURATION
#define CE_PIN PIN_PA3
#define CSN_PIN PIN_PA7
#define PIN_LED PIN_PB1
#define PIN_IRQ PIN_PB2

RF24 radio(CE_PIN, CSN_PIN, 1000000);

// ==========================================
// RADIO CONFIGURATION
// ==========================================

const uint64_t INCOMING_ADDRESS = 0xF0F0F0F0E1LL;
const uint64_t FORWARD_ADDRESS = 0xF0F0F0F0E2LL;

const uint8_t CANAL_RF = 76;
const uint8_t PAYLOAD_SIZE = 32;

// ==========================================
// STATE MACHINE
// ==========================================

enum estadosNRF {
  RX,
  TX
} estadosNRF_t;

estadosNRF NRF = RX;

// ==========================================
// MESSAGE BUFFER
// ==========================================

uint8_t messageBuffer[PAYLOAD_SIZE];

// ==========================================
// LED TIMER
// ==========================================

unsigned long ledTimer = 0;
const unsigned long LED_TIME = 150;

// ==========================================
// TRANSMISSION TIMER
// ==========================================

unsigned long txTimer = 0;
const unsigned long TX_TIMEOUT = 100;

// ==========================================
// SETUP
// ==========================================

void setup() {

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_IRQ, INPUT_PULLUP);

  digitalWrite(PIN_LED, LOW);

  // Begin radio
  radio.begin();

  // Shared radio configuration
  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(PAYLOAD_SIZE);

  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  // Configure incoming and outgoing addresses
  radio.openReadingPipe(1, INCOMING_ADDRESS);
  radio.openWritingPipe(FORWARD_ADDRESS);

  // Enable only RX_DR interrupt
  radio.maskIRQ(true, true, false);

  // Start receiving
  radio.startListening();
  digitalWrite(PIN_LED, HIGH);
}

// ==========================================
// MAIN LOOP
// ==========================================

void loop() {

  // Función de NRF con máquina de estados.
  nrf();

 
 // Controlar el estado del LED.
  if (digitalRead(PIN_LED) == HIGH && millis() - ledTimer >= LED_TIME) {

    digitalWrite(PIN_LED, LOW);
  }
}


void nrf() {

  switch (NRF) {

    case RX:
      if (radio.available()) {

        // Read received packet
        radio.read(messageBuffer, PAYLOAD_SIZE);

        // LED reception indicator
        digitalWrite(PIN_LED, HIGH);
        ledTimer = millis();

        // Switch to transmission mode
        radio.stopListening();

        // Start asynchronous transmission
        radio.startWrite(messageBuffer, PAYLOAD_SIZE, false);

        txTimer = millis();

        NRF = TX;
      }

      break;

    case TX:
      bool tx_ok = false;
      bool tx_fail = false;
      bool rx_ready = false;

      // Check transmission status without waiting
      radio.whatHappened(tx_ok, tx_fail, rx_ready);

      if (tx_ok || tx_fail) {

        // Return to receiving mode

        radio.startListening();

        NRF = RX;
      }

      // Transmission failsafe timeout
      else if (millis() - txTimer >= TX_TIMEOUT) {

        radio.stopListening();
        radio.flush_tx();

        radio.startListening();

        NRF = RX;
      }

      break;
  }
}
