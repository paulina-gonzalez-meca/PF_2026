#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TIEMPO_MANDAR 5000
#define TFT_DC 14
#define TFT_CS 32
#define TFT_RST 27
#define PIN_CE 5
#define PIN_CSN 15
#define PIN_MISO 16
#define PIN_MOSI 2
#define PIN_IRQ 4
#define PIN_SCK 17
#define TFT_BL 12

volatile int tiempo = 0;
uint64_t direccionConectar = 0xF0F0F0F0E0LL;
char mensajeRecibidoNRF[32];
char mensaje[] = "hola";

hw_timer_t *timer = NULL;  // timer
void IRAM_ATTR onTimer();  // function interrupts every 1ms

RF24 radio(PIN_CE, PIN_CSN);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  radio.begin();
  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
  tft.begin();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms
}

void loop() {
  if(tiempo >= TIEMPO_MANDAR){
    radio.stopListening();
    radio.write(mensaje, strlen(mensaje) + 1);
    radio.startListening();
    tiempo = 0;
  }
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    mensajeCentralConectar.trim();
    if(mensajeCentralConectar == "hola"){
      Serial.println(mensajeCentralConectar);
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(10, 10);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(4);
      tft.print(mensajeCentralConectar);
    }
  }
}

void IRAM_ATTR onTimer() {
  tiempo += 1;
}