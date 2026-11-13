#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t direccionA = 0xF0F0F0F0E0LL;
const uint64_t direccionB = 0xF0F0F0F0E1LL;
RF24 radio(4, 5); //CE, CSN
String message = "000";
char receivedMessage[32];
String msg = "";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setAutoAck(true);
  radio.setChannel(108);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(direccionB);
  radio.openReadingPipe(1, direccionA);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  radio.setRetries(15, 15);
}

void loop() {
  if(Serial.available() > 0){
    message = Serial.readString();
    message.trim();
    char messageBuffer[32];
    message.toCharArray(messageBuffer, sizeof(messageBuffer));
    radio.stopListening();
    bool success = radio.write(messageBuffer, strlen(messageBuffer) + 1);
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
    uint8_t len = radio.getDynamicPayloadSize();
    radio.read(&receivedMessage, len);
    msg = String(receivedMessage);
    Serial.println("Message received:" + msg);
  }
}
