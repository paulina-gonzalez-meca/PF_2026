#include "BluetoothSerial.h"
#include <HardwareSerial.h>
#include <vector>
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>

#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_ENVIAR_SMS 5000
#define TIEMPO_ENERGIA_PERIFERICOS 100000
#define TIEMPO_LED 100
#define TIEMPO_COMPROBAR 1000
#define PIN_ENERGIA 2
#define PIN_PULSADOR 32
#define PIN_CE 21
#define PIN_CSN 22
#define PIN_LED1 12
#define PIN_LED2 14
#define TIMEOUT 2000
// RST SMS
#define PIN_RST 26                   // Pin Reset SMS
#define TIEMPO_RST 100               // Tiempo reset sms
#define TIEMPO_ESTABILIDAD_RST 3000  // Tiempo post reset
HardwareSerial sim800l(2);
BluetoothSerial SerialBT;

class per {
public:
  String nombreInterno;
  String nombreExterno;
  String nombreSensor1;
  String nombreSensor2;
  String nombreSensor3;
  bool conEnergia;
  bool vivo;
  byte direccionNRF[6];

  per(String nombrePeriferico, String nombrePerifericoExterior, String nombreSensorExterior1, String nombreSensorExterior2, String nombreSensorExterior3, bool perConEnergia, bool vivoPer, byte byteVariableNRF) {
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

const uint64_t direcciones[] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };
const uint64_t direccionCentral = 0xF0F0F0F0E0LL;
//RF24 radio(PIN_CE, PIN_CSN);
char mensajeRecibidoNRF[32];
bool flagCorte = 1;
bool respuestaNRF = 0;
bool terminarGSMS = 0;
bool flagMensajePulsador = 0;
int cicloNRF = 0;
volatile int tiempoDelay = 0;
unsigned long inicio = 0;
unsigned long tiempoInicio = 0;
bool flagSenial = 0;
volatile int timerSMS = 0;
volatile int timerNRF = 0;
volatile int timerEng = 0;
volatile int tiempoComprobar = 0;
volatile int tiempoLed1 = 0;
volatile int tiempoLed2 = 0;
volatile int tiempoPulsador = 0;
volatile int tiempoComandos = 0;  // Timer para los comandos de diagnóstico.
volatile int timerLecturaSim = 0;
volatile int timerLecturaPin = 0;
volatile int timerResetSim = 0;
int indiceNum = 0;
int indiceEng = 0;
String trama = "";
String tramaProceso = "";
String bufferRespuesta = "";
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

// Enum para gestionar los estados del SIM800L
typedef enum {
  IDLE,
  ESPERANDO_RESPUESTA
} ESTADO_SENAL_t;
ESTADO_SENAL_t SENAL;

typedef enum {
  RST,
  COOLDOWN,
  SIM_IDLE
} ESTADO_RESET_t;
ESTADO_RESET_t PRST;


typedef enum {  // state machine
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

// Máquina de estados para secuencia de diagnóstico SIM800L
typedef enum {
  GSM_RESTART,
  GSM_WAIT_INIT,
  GSM_AT,
  GSM_CFUN,
  GSM_CPIN,
  GSM_CSCA,
  GSM_CSQ,
  GSM_CREG,
  GSM_CPMS,
  GSM_CMGF,
  GSM_CNMI,
  GSM_READY
} PASOS_GSM_t;

PASOS_GSM_t pasoGSM = GSM_WAIT_INIT;

hw_timer_t *timer = NULL;  // timer

void IRAM_ATTR onTimer();  // function interrupts every 1ms
void gestionarComandosGSM();

void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_RST, OUTPUT);  // Pin Reset es output


  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, HIGH);
  digitalWrite(PIN_RST, HIGH);  // Pin reset high está desactivado

  Serial.begin(115200);
  SerialBT.begin("Central_Dev");  // Nombre del dispositivo Bluetooth
  pinMode(16, INPUT_PULLUP);
  sim800l.begin(9600, SERIAL_8N1, 16, 17);

  numeros.push_back("+5491123692363");
  // numeros.push_back("+5491161386381");

  pinMode(PIN_ENERGIA, INPUT);
  pinMode(PIN_PULSADOR, INPUT);
  // radio.begin();
  // radio.openReadingPipe(1, direccionCentral);
  // radio.setPALevel(RF24_PA_MAX);
  // radio.startListening();

  PSMS = PASO1;
  PDECO = EMG;
  PRST = SIM_IDLE;

  // Timer de sistema activado antes de iniciar el conteo de GSM
  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms
  tiempoComandos = 0;
}

void loop() {
  // Manejo secuencial no bloqueante del módulo GSM
  if (terminarGSMS) {
    verificarSenalSIM800L();

    //comprobarSenalSIM800L();
    if (tiempoLed1 >= TIEMPO_LED) {
      digitalWrite(PIN_LED1, LOW);
    }
  }
  gestionarComandosGSM();


  //if(tiempoLed2 >= TIEMPO_LED){
  //  digitalWrite(PIN_LED2, LOW);
  //}
  if (digitalRead(PIN_PULSADOR) == 0 && flagMensajePulsador == 0) {
    flagMensajePulsador = 1;
    tiempoPulsador = 0;
  }
  if (flagMensajePulsador) {
    if (digitalRead(PIN_PULSADOR) && tiempoPulsador <= 1000) {
      mensajesNRF.push_back("probando NRF");
      mensajesNRF.push_back("0");
      flagMensajePulsador = 0;
    } else if (digitalRead(PIN_PULSADOR) && tiempoPulsador > 1000) {
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
        if (trama.startsWith("#1,")) {
          emergencias.push_back(trama);
        } else {
          tramas.push_back(trama);
        }
        trama = "";
      }
    }
  }
  lecturaEnergia();

  if (mensajesSMS.empty() != true) {
    if (timerSMS >= TIEMPO_ENVIAR_SMS) {
      if (indiceNum < numeros.size()) {
        mandarSMS(mensajesSMS[0], numeros[indiceNum]);
        if (PSMS == PASO1) {
          indiceNum += 1;
        }
      } else {
        indiceNum = 0;
        mensajesSMS.erase(mensajesSMS.begin());
      }
    }
  }



  /* if(mensajesNRF.empty() != true){
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
  } */
  // recibirNRF();
  //recibirSMS();
  // decodificador();
  // perifericosEnergia();

  // qué dice el sim
  /*if (timerLecturaSim >= 500) {
  
    char c = sim800l.read();
    SerialBT.write(c); // para imprimir lo q devuelve el sim

    timerLecturaSim = 0;
  }*/

  // lectura fuente
  /*if (timerLecturaPin >= 1000){
  int lectura = analogRead(PIN_ENERGIA);
   int voltaje = map(lectura, 0, 2735, 0, 12);
   float lecturaVoltaje = (voltaje - 0.4) * 10/43;
    SerialBT.print("V: ");
    SerialBT.println(voltaje);
    SerialBT.println(" ");

  }*/
}

void gestionarComandosGSM() {
  if (pasoGSM == GSM_READY) return;

  switch (pasoGSM) {
    case GSM_RESTART:  // nuevo estado de reset

      reiniciarSIM800L();  // Llama a tu función de reset por hardware

      if (PRST == IDLE) {  // Una vez terminado el ciclo de reset
        tiempoComandos = 0;
        pasoGSM = GSM_WAIT_INIT;  // Vuelve a empezar la inicialización
      }
      break;
    case GSM_WAIT_INIT:
      if (tiempoComandos >= 15000) {
        sim800l.println("AT");
        SerialBT.print("Comando AT");
        tiempoComandos = 0;
        pasoGSM = GSM_AT;
      }
      break;

    case GSM_AT:
      // Esperar OK del comando AT previo
      if (sim800l.available()) {
        String resp = sim800l.readString();
        if (resp.indexOf("OK") != -1) {
          sim800l.println("AT+CFUN?");
          SerialBT.print("Comando AT+cfun?");
          tiempoComandos = 0;
          pasoGSM = GSM_CFUN;
        }
      }

      // Fallo por Timeout: Si en 3 segundos no responde OK, mandar a reiniciar
      if (tiempoComandos >= 3000) {
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CFUN:
      if (tiempoComandos >= 1000) {
        sim800l.println("AT+CPIN?");
        SerialBT.print("Comando AT+cpin?");
        tiempoComandos = 0;
        pasoGSM = GSM_CPIN;
      }
      break;

    case GSM_CPIN:
      if (tiempoComandos >= 1000) {
        sim800l.println("AT+CSCA?");
        tiempoComandos = 0;
        pasoGSM = GSM_CSCA;
      }
      break;

    case GSM_CSCA:
      if (tiempoComandos >= 1000) {
        sim800l.println("AT+CSQ");
        tiempoComandos = 0;
        pasoGSM = GSM_CSQ;
      }
      break;

    case GSM_CSQ:
      if (tiempoComandos >= 2000) {
        sim800l.println("AT+CREG?");
        SerialBT.print("Comando AT+creg");
        tiempoComandos = 0;
        pasoGSM = GSM_CREG;
      }
      break;

    case GSM_CREG:
      // 1. Leer respuesta si está disponible
      if (sim800l.available()) {
        String resp = sim800l.readString();
        if (resp.indexOf("+CREG: 0,1") != -1 || resp.indexOf("+CREG: 0,5") != -1 || resp.indexOf("+CREG: 1") != -1 || resp.indexOf("+CREG: 5") != -1) {
          sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
          tiempoComandos = 0;
          pasoGSM = GSM_CPMS;
          break;
        }
      }
      // 2. Re-consultar AT+CREG? cada 2 segundos mientras se espera red
      static unsigned long timerReintentoCREG = 0;
      if (tiempoComandos - timerReintentoCREG >= 2000) {
        timerReintentoCREG = tiempoComandos;
        sim800l.println("AT+CREG?");
      }

      // 3. Si pasan 30 segundos acumulados sin enganchar red -> Reiniciar
      if (tiempoComandos >= 30000) {
        timerReintentoCREG = 0;
        pasoGSM = GSM_RESTART;
      }
      break;


    case GSM_CPMS:
      if (tiempoComandos >= 1000) {
        sim800l.println("AT+CMGF=1");
        SerialBT.print("Comando AT+cmgf=1");
        tiempoComandos = 0;
        pasoGSM = GSM_CMGF;
      }
      break;

    case GSM_CMGF:
      if (tiempoComandos >= 1000) {
        sim800l.println("AT+CNMI=2,2,0,0,0");
        SerialBT.print("at+cnmi=2,2,0,0,0");
        tiempoComandos = 0;
        pasoGSM = GSM_CNMI;
      }
      break;

    case GSM_CNMI:
      if (tiempoComandos >= 1000) {
        Serial.println("a");
        if (sim800l.available()) {
          SerialBT.print(sim800l.readString());
        }
        SerialBT.print("trama: #EMG,ApodoDisp,ResSens1,ResSens2,ResSens3*");
        SerialBT.print("SIM800L Ready");
        terminarGSMS = 1;
        digitalWrite(PIN_LED1, LOW);
        digitalWrite(PIN_LED2, LOW);
        pasoGSM = GSM_READY;
      }
      break;
  }
}


// Devuelve TRUE si hay señal válida, FALSE en caso contrario
// IMPORTANTE: Llamar continuamente a esta función en el loop()
bool verificarSenalSIM800L() {
  bool resultado = false;

  switch (SENAL) {
    case IDLE:
      // Paso 1: Vaciar buffer entrante viejo sin usar 'while'
      if (sim800l.available()) {
        sim800l.read();
        break;  // Sale para procesar un carácter por ciclo
      }

      // Paso 2: Enviar comando AT
      bufferRespuesta = "";
      sim800l.println("AT+CSQ");
      tiempoInicio = millis();
      SENAL = ESPERANDO_RESPUESTA;
      break;

    case ESPERANDO_RESPUESTA:
      // Paso 3: Acumular caracteres disponibles uno a uno (No bloqueante)
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;

        // Comprobar si ya recibimos la respuesta completa del comando
        if (bufferRespuesta.indexOf("OK") != -1 || bufferRespuesta.indexOf("ERROR") != -1) {
          int posicion = bufferRespuesta.indexOf("+CSQ:");

          if (posicion != -1) {
            int coma = bufferRespuesta.indexOf(',', posicion);
            if (coma != -1) {
              String rssiTexto = bufferRespuesta.substring(posicion + 6, coma);
              rssiTexto.trim();
              int rssi = rssiTexto.toInt();

              // RSSI entre 10 y 31 indica señal aceptable/buena
              if (rssi >= 10 && rssi <= 31) {
                SerialBT.print("HAY SEÑAL ESTABLE");
                digitalWrite(PIN_LED2, HIGH);
                resultado = true;
              } else {
                SerialBT.print("NO HAY SEÑAL ESTABLE");
                digitalWrite(PIN_LED2, LOW);
              }
            }
          } else {
            digitalWrite(PIN_LED2, LOW);
          }

          // Reiniciar para la próxima verificación
          SENAL = IDLE;
          return resultado;
        }
      }

      // Paso 4: Control de Timeout si el SIM800L no responde
      if (millis() - tiempoInicio >= TIMEOUT) {
        SerialBT.print("TIMEOUT");
        digitalWrite(PIN_LED2, LOW);
        SENAL = IDLE;
      }
      break;
  }

  return false;
}

// función para reiniciar el SIM800L
void reiniciarSIM800L() {
  switch (PRST) {
    case RST:
      SerialBT.print("SIM800L Reset");
      digitalWrite(PIN_RST, LOW);
      if (timerResetSim >= TIEMPO_RST) {

        timerResetSim = 0;
        PRST = COOLDOWN;
      }
      break;
    case COOLDOWN:
      digitalWrite(PIN_RST, HIGH);
      if (timerResetSim >= TIEMPO_ESTABILIDAD_RST) {

        timerResetSim = 0;
        PRST = SIM_IDLE;
      }
      break;
    case SIM_IDLE:
      break;
  }
}
void IRAM_ATTR onTimer() {
  tiempoLed1 += 1;
  tiempoLed2 += 1;
  tiempoPulsador += 1;
  tiempoDelay += 1;
  timerSMS += 1;
  timerEng += 1;
  timerNRF += 1;
  tiempoComandos += 1;  // Contador para comandos no bloqueantes
  tiempoComprobar += 1;
  //timerLecturaSim += 1;
  timerLecturaPin += 1;
  timerResetSim += 1;
}