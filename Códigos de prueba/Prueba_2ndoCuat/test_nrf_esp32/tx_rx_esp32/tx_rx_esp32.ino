#include <SPI.h>
#include <RF24.h>
#include <string.h>

// --------------------------------------
// ESP32 PINS
// --------------------------------------
#define PIN_CE 2
#define PIN_CSN 4


RF24 radio(PIN_CE, PIN_CSN, 1000000);

// --------------------------------------
// SHARED CONFIGURATION
// --------------------------------------
const uint64_t READING_ADDRESS = 0xF0F0F0F0E1LL;
const uint64_t TRANSMISSION_ADDRESS = 0xF0F0F0F0E1LL;
const uint8_t CANAL_RF = 76;
String message = "000";
char receivedMessage[32];
String msg = "";

void setup() {

  Serial.begin(115200);

  radio.begin();

  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);

  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(sizeof(receivedMessage));

  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(1, READING_ADDRESS);
  radio.openWritingPipe(TRANSMISSION_ADDRESS);

  radio.startListening();

  Serial.println("Receiver listening...");
}

void loop() {

  if (Serial.available() > 0) {
    Serial.println("Transmisser mode");
    message = Serial.readString();
    char messageBuffer[32] = {0};
    message.toCharArray(messageBuffer, sizeof(messageBuffer));
    radio.stopListening();
    bool success = radio.write(&messageBuffer, sizeof(messageBuffer));
    Serial.println("ESP32: MESSAGE SENT");
    radio.startListening();
    Serial.println("ESP32: LISTENING");
    if (success) {
      Serial.println("success");
    } else {
      Serial.println("failed");
    }
   
  }

  if (radio.available()) {
   // receivedMessage[32] = {0}; // Reset buffer to 0

    Serial.println("Radio Mode");
    memset(receivedMessage, 0, sizeof(receivedMessage));

    radio.read(receivedMessage, sizeof(receivedMessage));

    receivedMessage[31] = '\0';

    Serial.print("Received: ");
    Serial.println(receivedMessage);
  }
}