#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "BluetoothSerial.h"
#include <esp_now.h>
#include <WiFi.h>

BluetoothSerial SerialBT;

//MAC 1 (poner en el 2)
uint8_t broadcastAddress[] = {0x28, 0x05, 0xa5, 0x09, 0x52, 0x24};

//MAC 2 (poner en el 1)
//uint8_t broadcastAddress[] = {0x28, 0x05, 0xa5, 0x09, 0x52, 0x04};
esp_now_peer_info_t peerInfo;

const uint64_t address = 0xF0F0F0F0E1LL;
RF24 radio(4, 5); //CE, CSN
String message = "";
char receivedMessage[32];
char incomingReadings[64];
String msg = "";

typedef enum { //state machine
  ESPNOW,
  NRF
} COMMUNICATION_t;

COMMUNICATION_t COMMUNICATION;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  SerialBT.print("\r\nLast Packet Send Status:\t");
  SerialBT.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(incomingReadings, incomingData, len);
  incomingReadings[len] = '\0';
  SerialBT.print("Message received: ");
  SerialBT.println(incomingReadings);
}

void setup() {
  SerialBT.begin("ESP32test_2"); //Bluetooth device name

  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
  COMMUNICATION = ESPNOW;

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    SerialBT.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    SerialBT.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  if(SerialBT.available() > 0){
    message = SerialBT.readString();
    message.trim();
    if(message == "ESPNOW"){
      COMMUNICATION = ESPNOW;
    }
    else if(message == "NRF"){
      COMMUNICATION = NRF;
    }
  }
  comms();
}

void comms(){
  switch(COMMUNICATION){
    case NRF:
      if(message != ""){
        char messageBuffer[32];
        message.toCharArray(messageBuffer, sizeof(messageBuffer));
        radio.stopListening();
        bool success = radio.write(messageBuffer, sizeof(messageBuffer));
        message = "";
        if(success){
          SerialBT.println("success");
        }
        else{
          SerialBT.println("failed");
        }
        radio.startListening();
      }
      if(radio.available() > 0){
        memset(receivedMessage, 0, sizeof(receivedMessage));
        radio.read(&receivedMessage, sizeof(receivedMessage));
        msg = String(receivedMessage);
        SerialBT.println("Message received:" + msg);
      }
    break;
    case ESPNOW:
    if(message != ""){
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)message.c_str(), message.length());
        message = "";

        if(result == ESP_OK){
          SerialBT.println("Sent with success");
        }
        else{
          SerialBT.println("Error sending the data");
        }
    }
    break;
  }
}