#include "BluetoothSerial.h"

#define PIN_LED 26
#define TIEMPO_MSJ 5000

BluetoothSerial SerialBT;

int timerMSJ = 0;
int timerLed = 0;

hw_timer_t *timer = NULL; //timer
void IRAM_ATTR onTimer(); //function interrupts every 1ms

void setup() {
  Serial.begin(9600);
  SerialBT.begin("pruebaESP32"); //Bluetooth device name

  pinMode(PIN_LED, OUTPUT);

  timer = timerBegin(1000000); // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); // tick every 1ms
}

void loop() {
  if(timerLed >= 1000){
    digitalWrite(PIN_LED, LOW);
  }
  if(timerMSJ == TIEMPO_MSJ){
    SerialBT.print("prueba");
    digitalWrite(PIN_LED, HIGH);
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}

void IRAM_ATTR onTimer() {
  timerMSJ += 1;
  timerLed += 1;
}
