#include <SPI.h>
#include <RF24.h>
#include <string.h>

// --------------------------------------
// ESP32 PINS
// --------------------------------------
#define PIN_CE  2
#define PIN_CSN 4

RF24 radio(PIN_CE, PIN_CSN, 1000000);

// --------------------------------------
// SHARED CONFIGURATION
// --------------------------------------
const uint64_t address = 0xF0F0F0F0E1LL;
const uint8_t CANAL_RF = 76;

char receivedMessage[32];

void setup() {

  Serial.begin(115200);
  delay(500);

  Serial.println("Initializing NRF24L01 receiver...");

  if (!radio.begin()) {

    Serial.println("ERROR: NRF24L01 not detected.");

    while (1) {
      delay(1000);
    }
  }

  Serial.println("NRF24L01 initialized successfully.");

  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);

  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(sizeof(receivedMessage));

  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(1, address);

  radio.startListening();

  Serial.println("Receiver listening...");
}

void loop() {

  if (radio.available()) {

    memset(receivedMessage, 0, sizeof(receivedMessage));

    radio.read(receivedMessage, sizeof(receivedMessage));

    receivedMessage[31] = '\0';

    Serial.print("Received: ");
    Serial.println(receivedMessage);
  }
}
