#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TIEMPO_LED 1000
#define PRESSED 0
#define NOT_PRESSED 1
#define PIN_SW 0
#define PIN_CE 4
#define PIN_CSN 5
// #define PIN_MISO 16
// #define PIN_MOSI 2
// #define PIN_IRQ 4
// #define PIN_SCK 17
#define PIN_LED 2

uint64_t direccionConectarA = 0xF0F0F0F0E0LL;
uint64_t direccionConectarB = 0xF0F0F0F0E1LL;
char mensajeRecibidoNRF[32];
char mensaje[] = "hola";
bool flagSW = 0;
int tiempo = 0;

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  Serial.begin(9600);
  //SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);
  SPI.begin();
  radio.begin();
  radio.setAutoAck(true);
  radio.setChannel(108);
  radio.openWritingPipe(direccionConectarB);
  radio.openReadingPipe(1, direccionConectarA);
  radio.startListening();
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SW, INPUT);

  digitalWrite(PIN_LED, LOW);
}

void loop() {
  int tiempoActual = millis();
  if(tiempoActual - tiempo >= TIEMPO_LED){
    digitalWrite(PIN_LED, LOW);
  }
  if(flagSW == 0){
    if(digitalRead(PIN_SW) == PRESSED){
      flagSW = 1;
    }
  }
  else{
    if(digitalRead(PIN_SW) == NOT_PRESSED){
      flagSW = 0;
      radio.stopListening();
      bool exito = radio.write(&mensaje, sizeof(mensaje));
      if(exito){
        Serial.println("mandando");
      }else{
        
      Serial.println("no");
      }
      radio.startListening();
    }
  }
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    mensajeCentralConectar.trim();
    if(mensajeCentralConectar == "hola"){
    Serial.println(mensajeCentralConectar);
    digitalWrite(PIN_LED, HIGH);
    tiempo = millis();
    }
  }
}
