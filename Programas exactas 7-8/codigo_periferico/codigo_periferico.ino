#include <vector>
#include <algorithm>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DHT.h"
#include "datosPer.h"
#include "BluetoothSerial.h"
#include "BLEDevice.h"

// incluir constantes
#define ERROR "%"

#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_LECTURA 50000
#define TIEMPO_ENERGIA_PERIFERICOS 100000
#define TIEMPO_PUERTA 20000
#define TIEMPO_LED 100

#define PIN_ENERGIA 0
#define PIN_CE 10
#define PIN_CSN 5
#define PIN_S1 1
#define PIN_LED 21
#define PIN_PULSADOR 20

//BluetoothSerial SerialBT;

class per{
  public:
    String nombre;
    int tipoSensor1;
    int tipoSensor2;
    int tipoSensor3;
    uint64_t direccion;

    per(String nombrePeriferico, int tipoSensor1Per, int tipoSensor2Per, int tipoSensor3Per, uint64_t direccionPer){
      nombre = nombrePeriferico;
      tipoSensor1 = tipoSensor1Per;
      tipoSensor2 = tipoSensor2Per;
      tipoSensor3 = tipoSensor3Per;
      direccion = direccionPer;
    }
};

per periferico("perif1", 0, 0, 0, 0xF0F0F0F0E1LL);
const uint64_t direccionCentral = 0xF0F0F0F0E0LL;

RF24 radio(PIN_CE, PIN_CSN);
//DHT dht2(PIN_S2, DHT22);

char mensajeRecibidoNRF[32];
bool flagCorte = 1;
bool respuestaNRF = 0;
bool pedido = 0;
int cicloNRF = 0;
unsigned long int tiempoS1 = 0;
unsigned long int tiempoS2 = 0;
unsigned long int tiempoS3 = 0;
unsigned long int tiempoLed = 0;
unsigned long int tiempoLectura = 0;
int switchLecturas = 0;
unsigned long int timerNRF = 0;
String trama = "";
String tramaProceso = "";
String tramaPerifericoFinal = "";
int ind = 0;
bool flagBoton = 0;
bool emergencia = 0;
bool leer = 0;
bool flagNormalidad = 0;
bool flagMandarNRF = 0;
bool flagSensor1 = 0;
bool flagSensor2 = 0;
bool flagSensor3 = 0;
bool flagPulsador = 0;
char respuesta[3] = {'L', 'L', 'L'};
float lecturaS1 = 0;
float lecturaS2 = 0;
float lecturaS3 = 0;
int umbralMaxS2 = 30;
int umbralMinS2 = 10;

std::vector<String> tramas;
std::vector<String> mensajesNRF;
std::vector<datosPer> filaDatosPer;

typedef enum { //maquina de estados
  INICIO,
  APD,
  L1,
  L2,
  L3
} PASOS_DECODIFICADOR_t;
PASOS_DECODIFICADOR_t PDECO;

typedef enum {
  EMG,
  LUZ,
  SENSOR_1,
  SENSOR_2,
  SENSOR_3,
  ARMADO_ENVIADO
} ESTADOSARMADOPERIFERICO_t;
ESTADOSARMADOPERIFERICO_t estadosArmadoPeriferico = EMG;

hw_timer_t *timer = NULL; //timer

void IRAM_ATTR onTimer(); //function interrupts every 1ms

//funciones
String armadoTramaPeriferico(std::vector<datosPer> &fila);
void lecturaEnergia();
void decodificador();
void mandarNRF(String mensajeNRF);
void recibirNRF();
bool lecturaPuerta(int pinPuerta, bool flagPuerta, int timerPuerta);
float lecturaDHT();
void switchS1();
void switchS2();
void switchS3();

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  Serial.println("TEST BEGIN");
  //SerialBT.begin("pruebaESP32_per");
  Serial.begin(115200);
  delay(2000);
  Serial.println("trama: #ApodoDisp,ResSens1,ResSens2,ResSens3*");

  pinMode(PIN_ENERGIA, INPUT);
  pinMode(PIN_PULSADOR, INPUT);

  SPI.begin(9,8,6,5);
  radio.begin();
  radio.openReadingPipe(1, periferico.direccion);
  radio.openWritingPipe(direccionCentral);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  PDECO = INICIO;

  timer = timerBegin(1000000); // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); // tick every 1ms

  digitalWrite(PIN_LED, LOW);
  Serial.println("test end setup");
}

void loop() {
  if(tiempoLed >= TIEMPO_LED){
    digitalWrite(PIN_LED, LOW);
  }
  if(flagBoton == 0){
    if(digitalRead(PIN_PULSADOR)){
      flagBoton = 1;
    }
  }
  else if(flagBoton == 1){
    if(flagPulsador){
      if(digitalRead(PIN_PULSADOR)){
        mensajesNRF.push_back("probando");
        flagPulsador = 0;
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
          tramas.push_back(trama);
          trama = "";
        }
      }
    }
    if(tiempoLectura == TIEMPO_LECTURA){
      leer = 1;
      tiempoLectura = 0;
    }

    lecturaEnergia();

    if(leer == 1){
      switch(switchLecturas){
        case 0:
          if(respuesta[0]=='L'){
            switchS1();
          }
          else{
            switchLecturas += 1;
          }
        break;
        case 1:
          if(emergencia || flagNormalidad || pedido){
            pedido = 0;
            flagNormalidad = 0;
            flagMandarNRF = 1;
          }
          respuesta[0] = 'L';
          respuesta[1] = 'L';
          respuesta[2] = 'L';
          leer = 0;
          tiempoLectura = 0;
          switchLecturas = 0;
        break;
      }
    }

    if(mensajesNRF.empty() != true){
      if(timerNRF >= TIEMPO_ENVIAR_NRF){
        if(respuestaNRF == 1){
          respuestaNRF = 0;
          cicloNRF = 0;
          mensajesNRF.erase(mensajesNRF.begin());
        }
        else if(respuestaNRF == 0 && cicloNRF < 5){
          mandarNRF(mensajesNRF[0]);
          cicloNRF += 1;
          timerNRF = 0;
          if(mensajesNRF[0] == "recibido"){
            respuestaNRF = 0;
            cicloNRF = 0;
            mensajesNRF.erase(mensajesNRF.begin());
          }
        }
        else if(respuestaNRF == 0 && cicloNRF >= 5){
          cicloNRF = 0;
          mensajesNRF.erase(mensajesNRF.begin());
        }
      }
    }

    if(flagMandarNRF){
      filaDatosPer.push_back(datosPer(emergencia, flagCorte, lecturaS1, lecturaS2, lecturaS3));
      flagMandarNRF = 0;
    }
    
    tramaPerifericoFinal = armadoTramaPeriferico(filaDatosPer);

    if (tramaPerifericoFinal != ""){
      Serial.println(tramaPerifericoFinal);
      mensajesNRF.push_back(tramaPerifericoFinal);
      tramaPerifericoFinal = "";
    }
    recibirNRF();
    decodificador();
  }
}


void IRAM_ATTR onTimer() {
  tiempoS1 += 1;
  tiempoS2 += 1;
  tiempoS3 += 1;
  timerNRF += 1;
  tiempoLed += 1;
  tiempoLectura += 1;
}
