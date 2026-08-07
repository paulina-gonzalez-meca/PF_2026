#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PIN_LED 26
#define TIEMPO_MSJ 5000

const uint64_t address = 0xF0F0F0F0E1LL;
RF24 radio(4, 5); //CE, CSN
String message = "000";
char receivedMessage[32];
String msg = "";
int timerMSJ = 0;
int timerLed = 0;

hw_timer_t *timer = NULL; //timer
void IRAM_ATTR onTimer(); //function interrupts every 1ms

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(PIN_LED, OUTPUT);

  timer = timerBegin(1000000); // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); // tick every 1ms
}

void loop() {
  if(timerLed += 1000){
    digitalWrite(PIN_LED, LOW);
  }
  if(timerMSJ == TIEMPO_MSJ){
    message = "prueba";
    char messageBuffer[32];
    message.toCharArray(messageBuffer, sizeof(messageBuffer));
    radio.stopListening();
    bool success = radio.write(&messageBuffer, sizeof(messageBuffer));
    if(success){
      Serial.println("success");
      digitalWrite(PIN_LED, HIGH);
    }
    else{
      Serial.println("failed");
    }
    radio.startListening();
  }
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
    Serial.println("Message received:" + msg);
  }
}

void IRAM_ATTR onTimer() {
  timerMSJ += 1;
  timerLed += 1;
}