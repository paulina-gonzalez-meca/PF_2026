#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TIEMPO_MANDAR 5000
#define TIEMPO_S 2000
#define TFT_DC 14
#define TFT_CS 32
// #define TFT_RST 27
#define PIN_CE 5
#define PIN_CSN 15
#define PIN_MISO 16
#define PIN_MOSI 2
#define PIN_IRQ 4
#define PIN_SCK 17
#define TFT_MOSI 12
#define TFT_MISO 13
#define TFT_CLK 27
// #define TFT_BL 12
#define PIN_BZZ 33

volatile int tiempo = 0;
volatile int tiempoS = 0;
uint64_t direccionConectarA = 0xF0F0F0F0E0LL;
uint64_t direccionConectarB = 0xF0F0F0F0E1LL;
char mensajeRecibidoNRF[32];
char mensaje[] = "hola";

hw_timer_t *timer = NULL;  // timer
void IRAM_ATTR onTimer();  // function interrupts every 1ms

RF24 radio(PIN_CE, PIN_CSN);

SPIClass hspi(HSPI);
Adafruit_ILI9341 tft = Adafruit_ILI9341(&hspi, TFT_DC, TFT_CS);

void setup() {
  Serial.begin(9600);
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);
  hspi.begin(TFT_CLK, TFT_MISO, TFT_MOSI);

  radio.begin(&SPI);
  radio.setAutoAck(true);
  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(direccionConectarA);
  radio.openReadingPipe(1, direccionConectarB);
  radio.startListening();

  tft.begin(8000000);
  // pinMode(TFT_BL, OUTPUT);
  // digitalWrite(TFT_BL, HIGH);
  pinMode(PIN_BZZ, OUTPUT);
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms
}

void loop() {
  if (tiempoS >= TIEMPO_S) {
    tft.fillScreen(ILI9341_BLACK);
    digitalWrite(PIN_BZZ, LOW);
    tiempoS = 0;
  }
  if (tiempo >= TIEMPO_MANDAR) {
    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(4);
    tft.print("enviando");
    radio.stopListening();
    radio.write(&mensaje, sizeof(mensaje));
    radio.startListening();
    tiempo = 0;
    Serial.println("enviando");
    tiempoS = 0;
  }
  if (radio.available() > 0) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    mensajeCentralConectar.trim();
    if (mensajeCentralConectar == "hola") {
      Serial.println(mensajeCentralConectar);
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(10, 10);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(4);
      tft.print(mensajeCentralConectar);
      digitalWrite(PIN_BZZ, HIGH);
    tiempoS = 0;
    }
  }
}

void IRAM_ATTR onTimer() {
  tiempo += 1;
  tiempoS += 1;
}