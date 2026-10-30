#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t address = 0xF0F0F0F0E1LL;
const uint8_t CANAL_RF = 76;

RF24 radio(2, 4); //CE, CSN
String message = "000";
char receivedMessage[32];
String msg = "";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(32);

  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(address);

  radio.stopListening();
}

void loop() {
  if(Serial.available() > 0){
    message = Serial.readString();
    char messageBuffer[32];
    message.toCharArray(messageBuffer, sizeof(messageBuffer));
    radio.stopListening();
    bool success = radio.write(&messageBuffer, sizeof(messageBuffer));
    if(success){
      Serial.println("success");
    }
    else{
      Serial.println("failed");
    }
    radio.startListening();
  }
  if(radio.available() > 0){
    memset(receivedMessage, 0, sizeof(receivedMessage));
    radio.read(&receivedMessage, sizeof(receivedMessage));
    msg = String(receivedMessage);
    // Serial.println("Message received:" + msg);
  }
}
