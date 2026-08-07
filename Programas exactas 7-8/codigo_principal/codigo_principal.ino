#include <HardwareSerial.h>
#include <vector>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "BluetoothSerial.h"

#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_ENVIAR_SMS 5000
#define TIEMPO_ENERGIA_PERIFERICOS 100000
#define TIEMPO_LED 100
#define PIN_ENERGIA 2
#define PIN_PULSADOR 32
#define PIN_CE 21
#define PIN_CSN 22
#define PIN_LED1 12
#define PIN_LED2 14


HardwareSerial sim800l(2);
BluetoothSerial SerialBT;

class per{
  public:
    String nombreInterno;
    String nombreExterno;
    String nombreSensor1;
    String nombreSensor2;
    String nombreSensor3;
    bool conEnergia;
    bool vivo;
    byte direccionNRF[6];

    per(String nombrePeriferico, String nombrePerifericoExterior, String nombreSensorExterior1, String nombreSensorExterior2, String nombreSensorExterior3, bool perConEnergia, bool vivoPer, byte byteVariableNRF){
      nombreInterno = nombrePeriferico;
      nombreExterno = nombrePerifericoExterior;
      nombreSensor1 = nombreSensorExterior1;
      nombreSensor2 = nombreSensorExterior2;
      nombreSensor3 = nombreSensorExterior3;
      conEnergia = perConEnergia;
      vivo = vivoPer;
      direccionNRF[0] = 0xF0;
      direccionNRF[1] = 0xF0;
      direccionNRF[2] = 0xF0;
      direccionNRF[3] = 0xF0;
      direccionNRF[4] = byteVariableNRF;
      direccionNRF[5] = 0x00;
    }
};
const uint64_t direcciones[] = {0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL};
const uint64_t direccionCentral = 0xF0F0F0F0E0LL;
RF24 radio(PIN_CE, PIN_CSN);
char mensajeRecibidoNRF[32];
bool flagCorte = 1;
bool respuestaNRF = 0;
bool flagMensajePulsador = 0;
int cicloNRF = 0;
volatile int tiempoDelay = 0;
volatile int timerSMS = 0;
volatile int timerNRF = 0;
volatile int timerEng = 0;
volatile int tiempoLed1 = 0;
volatile int tiempoLed2 = 0;
volatile int tiempoPulsador = 0;
int indiceNum = 0;
int indiceEng = 0;
String trama = "";
String tramaProceso = "";
int ind = 0;
bool decoEmg = 0;
String respuesta[5];

std::vector<String> tramas;
std::vector<String> emergencias;
std::vector<per> perifericos;
std::vector<String> mensajesSMS;
std::vector<String> mensajesNRF;
std::vector<String> numeros;

typedef enum {
  PASO1,
  PASO2,
  PASO3
} PASOS_ENVIARSMS_t;
PASOS_ENVIARSMS_t PSMS;

typedef enum { //state machine
  EMG,
  APD,
  LUZ,
  RSLT_S1,
  RSLT_S2,
  RSLT_S3
} PASOS_DECODIFICADOR_t;

PASOS_DECODIFICADOR_t PDECO;

struct TramaPausada_t {
  PASOS_DECODIFICADOR_t pasoDeco;
  String tramaPausa;
  int indice;
  String respuestas[5];
};
TramaPausada_t tramaPausada;

hw_timer_t *timer = NULL; //timer

void IRAM_ATTR onTimer(); //function interrupts every 1ms

void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, HIGH);

  SerialBT.begin("pruebaESP32_central");
  Serial.begin(9600);
  pinMode(16, INPUT_PULLUP);
  sim800l.begin(9600, SERIAL_8N1, 16, 17);
  delay(15000);
//COMANDOS DE DIAGNÓSTICO
  sim800l.println("AT");
  delay(500); 
  sim800l.println("AT+CFUN?");
  delay(1000); 
  sim800l.println("AT+CPIN?");
  delay(1000); 
  sim800l.println("AT+CSCA?");
  delay(1000); 
  sim800l.println("AT+CSQ");
  delay(2000); 
  sim800l.println("AT+CREG?");
  delay(2000); 
  sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  delay(1000);
  sim800l.println("AT+CMGF=1"); //confifuracion de formato de SMS (recibir y enviar)
  delay(1000);
  sim800l.println("AT+CNMI=2,2,0,0,0"); //configuracion de mensajes recibidos
  delay(1000);
  Serial.println("a");
  if(sim800l.available()){
    Serial.print(sim800l.readString());
  }
  Serial.println("trama: #EMG,ApodoDisp,ResSens1,ResSens2,ResSens3*");

  perifericos.push_back(per("perif1", "cocina", "sensor temperatura freezer", "sensor puerta freezer", "sensor humedad freezer", 1, 1, 0xE1));
  perifericos.push_back(per("perif2", "lab", "sensor temperatura freezer 2", "-", "-", 1, 1, 0xE2));
  perifericos.push_back(per("perif3", "a", "-", "-", "-", 1, 1, 0xE3));
  numeros.push_back("+5491161386381");
  //numeros.push_back("+5491163710617");
  //numeros.push_back("+5491123692363");

  pinMode(PIN_ENERGIA, INPUT);
  pinMode(PIN_PULSADOR, INPUT);
  radio.begin();
  radio.openReadingPipe(1, direccionCentral);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  PSMS = PASO1;
  PDECO = EMG;

  timer = timerBegin(1000000); // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); // tick every 1ms

  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
}

void loop() {
  if(tiempoLed1 >= TIEMPO_LED){
    digitalWrite(PIN_LED1, LOW);
  }
  if(tiempoLed2 >= TIEMPO_LED){
    digitalWrite(PIN_LED2, LOW);
  }
  if(digitalRead(PIN_PULSADOR) == 0 && flagMensajePulsador == 0){
    flagMensajePulsador = 1;
    tiempoPulsador = 0;
  }
  if(flagMensajePulsador){
    if(digitalRead(PIN_PULSADOR) && tiempoPulsador <= 1000){
      mensajesNRF.push_back("probando NRF");
      mensajesNRF.push_back("0");
      flagMensajePulsador = 0;
    }
    else if(digitalRead(PIN_PULSADOR) && tiempoPulsador > 1000){
      mensajesSMS.push_back("probando SMS");
      flagMensajePulsador = 0;
    }
  }
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    // Start of a new packet: clear any stale text
    if (c == '#') {
      trama = "#"; 
    } 
    // Capturing body characters
    else if (trama.length() > 0) {
      trama += c;
      
      if (c == '*') {
        trama.trim();
        Serial.println(trama);
        if(trama.startsWith("#1,")){
          emergencias.push_back(trama);
        }
        else{
          tramas.push_back(trama);
        }
        trama = "";
      }
    }
  }
  lecturaEnergia();
  
  if(mensajesSMS.empty() != true){
    if(timerSMS >= TIEMPO_ENVIAR_SMS){
      if(indiceNum < numeros.size()){
        mandarSMS(mensajesSMS[0], numeros[indiceNum]);
        if(PSMS == PASO1){
          indiceNum += 1;
        }
      }
      else{
        indiceNum = 0;
        mensajesSMS.erase(mensajesSMS.begin());
      }
    }
  }
  if(mensajesNRF.empty() != true){
    if(timerNRF >= TIEMPO_ENVIAR_NRF){
      if(respuestaNRF == 1){
        respuestaNRF = 0;
        cicloNRF = 0;
        mensajesNRF.erase(mensajesNRF.begin());
        mensajesNRF.erase(mensajesNRF.begin());
      }
      else if(respuestaNRF == 0 && cicloNRF < 5){
        if(mensajesNRF.size() >= 2){
          mandarNRF(mensajesNRF[0], direcciones[mensajesNRF[1].toInt()]);
        }
      cicloNRF += 1;
      timerNRF = 0;
        if(mensajesNRF[0] == "recibido"){
          respuestaNRF = 0;
          cicloNRF = 0;
          mensajesNRF.erase(mensajesNRF.begin());
          mensajesNRF.erase(mensajesNRF.begin());
        }
      }
      else if(respuestaNRF == 0 && cicloNRF >= 5){
        cicloNRF = 0;
        if(mensajesNRF[0] == "v?"){
          perifericos[mensajesNRF[1].toInt()].vivo = 0;
        }
        mensajesNRF.erase(mensajesNRF.begin());
        mensajesNRF.erase(mensajesNRF.begin());
      }
    }
  }
  recibirNRF();
  recibirSMS();
  decodificador();
  perifericosEnergia();
}


void IRAM_ATTR onTimer() {
  tiempoLed1 += 1;
  tiempoLed2 += 1;
  tiempoPulsador += 1;
  tiempoDelay += 1;
  timerSMS += 1;
  timerEng += 1;
  timerNRF += 1;
}