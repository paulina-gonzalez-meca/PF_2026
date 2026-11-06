#include <nRF24L01.h>
#include <RF24.h>

#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 20000
const uint64_t direccion = 0xF0F0F0F0E1LL;
#define TAM_NRF 10
#define PRESIONADO 0
#define NO_PRESIONADO 1
#define SW_PIN 0
#define PIN_CE /*PIN_PA3*/4
#define PIN_CSN /*PIN_PA7*/5
#define PIN_LED /*PIN_PB1*/2

char mensajeRecibidoNRF[32];
bool respuestaNRF = 0;
int cicloNRF = 0;
unsigned long tiempoNRFAnterior = 0;
unsigned long tiempoConeccionAnterior = 0;
unsigned long tiempoNRF = 0;
unsigned long tiempoConeccion = 0;
char nombre[4];
char mensaje[8];
int tipoSensor1 = 1;
int tipoSensor2 = 2;
String mensajesNRF[TAM_NRF];
String trama = "";
int indiceMensajesNRF = 0;
bool flagSW = 0;
bool coneccion = 0;
bool flagConectado = 0;

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT);
  pinMode(PIN_LED, OUTPUT);

  radio.begin();
  radio.openReadingPipe(1, direccion);
  radio.openWritingPipe(direccion);
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  snprintf(mensaje, sizeof(mensaje), "#C,%d,%d*", tipoSensor1, tipoSensor2);
  tiempoNRFAnterior = millis();
}

void loop() {
  if(!flagConectado){
    if(!flagSW){
      if(digitalRead(SW_PIN) == PRESIONADO){
        Serial.println("PRESIONANDO");
        flagSW = 1;
      }
    }
    else{
      if(digitalRead(SW_PIN) == NO_PRESIONADO){
        Serial.println("NO PRESIONADO");
        tiempoConeccionAnterior = millis();
        coneccion = 1;
        flagSW = 0;
      }
    }
  }

  if(coneccion){
    flagConectado = 1;
    tiempoConeccion = millis();
    digitalWrite(PIN_LED, HIGH);
    if(tiempoConeccion - tiempoConeccionAnterior <= TIMEOUT_CONECTAR){
      if (conectar(mensaje, direccion)) {
        
        Serial.println("CONECTANDO");
      }
    }
    else{
      coneccion = 0;
        Serial.println("timeout");
      digitalWrite(PIN_LED, LOW);
        flagConectado = 0;
    }
  }

  if(indiceMensajesNRF >= 1){
    tiempoNRF = millis();
    if(tiempoNRF - tiempoNRFAnterior >= TIEMPO_ENVIAR_NRF){
      if(respuestaNRF == 1){
        if(eliminarPrimero(mensajesNRF, TAM_NRF)){
          indiceMensajesNRF --;
          respuestaNRF = 0;
          cicloNRF = 0;
        }
      }
      else if(respuestaNRF == 0 && cicloNRF < 5){
        if(indiceMensajesNRF >= 1){
          mandarNRF(mensajesNRF[0], direccion);
        }
        cicloNRF += 1;
        tiempoNRF = millis();
        if(mensajesNRF[0].endsWith("recibido*") || mensajesNRF[0] == "OK"){
          respuestaNRF = 1;
        }
      }
      else if(respuestaNRF == 0 && cicloNRF >= 5){
        if(eliminarPrimero(mensajesNRF, TAM_NRF)){
          indiceMensajesNRF --;
          respuestaNRF = 0;
          cicloNRF = 0;
        }
      }
    }
  }
}