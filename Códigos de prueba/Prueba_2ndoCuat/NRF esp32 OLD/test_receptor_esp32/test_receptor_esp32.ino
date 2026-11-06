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
const uint64_t direccionConectar = 0xF0F0F0F0E0LL;

char receivedMessage[32];

void setup() {

  Serial.begin(115200);
  delay(500);

  Serial.println("Initializing NRF24L01 receiver...");
  //SPI.begin(18,19,21);

  if (!radio.begin()) {

    Serial.println("ERROR: NRF24L01 not detected.");

    while (1) {
      delay(1000);
    }
  }

  Serial.println("NRF24L01 initialized successfully.");

  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_MAX);
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
