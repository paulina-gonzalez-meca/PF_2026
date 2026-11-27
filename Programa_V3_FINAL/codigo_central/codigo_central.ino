//#include "BluetoothSerial.h"
#include <HardwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

TaskHandle_t sim;

#define RX_PIN 19
#define TX_PIN 21
#define PIN_DTR 23
#define PIN_ENERGIA 35
#define PIN_CE 5
#define PIN_CSN 15
#define NRF_MOSI 2
#define NRF_MISO 16
#define NRF_CLK 17
#define TFT_BL 33
#define PIN_RST 22
#define TFT_DC 14
#define TFT_CS 32
#define TFT_MOSI 12
#define TFT_MISO 13
#define TFT_CLK 27
#define ENCODER_A_PIN 25
#define ENCODER_B_PIN 26
#define ENCODER_SW_PIN 39

#define TIEMPO_DEBOUNCE 200
#define TIEMPO_PARPADEO 500
#define TIMEOUT_AGREGAR 20000
#define TIEMPO_SENAL 300000
#define TIEMPO_RST 100               // Tiempo reset sms
#define TIEMPO_ESTABILIDAD_RST 3000  // Tiempo post reset#define TFT_DC 14
#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_ENVIAR_SMS 5000
#define TIEMPO_ENERGIA_PERIFERICOS 100000
#define TIEMPO_LED 100
#define TIEMPO_COMPROBAR 1000
#define TIMEOUT 2000
#define CONFIG_TIMEOUT 1000
#define TIEMPO_CONECTAR_PER 1000

#define ANCHO 12
#define TAM_TR 10
#define TAM_NRF 10

HardwareSerial sim800l(2);
//BluetoothSerial // SerialBT;

class per {
public:
  char nombreInterno[4] = { "" };
  char nombreExterno[5] = { "----" };
  char nombreSensor1[5] = { "TEMP" };
  char nombreSensor2[5] = { "DOOR" };
  int tipoSensor1;
  int tipoSensor2;
  String lecturaSensor1 = "-";
  String lecturaSensor2 = "-";
  int emergenciaS1 = 0;
  int emergenciaS2 = 0;
  bool flagEmgS1 = 0;
  bool flagEmgS2 = 0;
  int indiceNombrePer = 0;
  int indiceNombreS1 = 4;
  int indiceNombreS2 = 4;
  int umbralMinS1 = 0;
  int umbralMaxS1 = 25;
  int umbralMinS2 = 0;
  int umbralMaxS2 = 0;
  bool alertaS2[20] = { 0 };
  bool alertaS1[20] = { 0 };
  int bateria = 0;
  per() {
    nombreInterno;
    nombreExterno;
    nombreSensor1;
    nombreSensor2;
    tipoSensor1 = 1;
    tipoSensor2 = 2;
    lecturaSensor1;
    lecturaSensor2;
    emergenciaS1;
    emergenciaS2;
    flagEmgS1;
    flagEmgS2;
    alertaS1;
    alertaS2;
    indiceNombrePer;
    indiceNombreS1;
    indiceNombreS2;
    bateria;
  }
};

const uint64_t direccionA = 0xF0F0F0F0E0LL;
const uint64_t direccionB = 0xF0F0F0F0E1LL;

RF24 radio(PIN_CE, PIN_CSN, 8000000);

char letras[27] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
char numeros[11] = { "1234567890" };
volatile int valorEncoder = 0;
int ultimoValorCLK = 0;
int ultimoValorDT = 0;
volatile unsigned long debounceSWEncoder = 0;
volatile unsigned long tiempoAgregarPer = 0;
volatile unsigned long parpadeo = 0;
volatile unsigned long tiempoConectar = 0;
volatile unsigned long tiempoComandoMandado = 0;
volatile unsigned long tiempoDisplayBat = 1800000;
int valorDisplay = 1;
int modificarUsuario = 0;
int modificarPer = 0;
int eliminar = 0;
int porcentajeBateria = 0;
bool seleccionar = 0;
bool flagSWEncoder = 0;
bool senalPrint = 0;
char nombreEliminar[4] = "";
char mensajeRecibidoNRF[32];
bool flagCorte = 1;
bool mandandoSMS = 0;
bool respuestaNRF = 0;
bool terminarGSMS = 0;
bool eliminarTR = 0;
bool flagMensajePulsador = 0;
bool mensajeRecibidoConectar = 0;
bool mensajeRecibidoOK = 0;
bool conectando = 0;
bool displayModoSleep = 1;
bool comandoMandadoSleep = 0;
bool flagEmg1 = 0;
bool flagEmg2 = 0;
bool flagEmg3 = 0;
bool flagEmg4 = 0;
bool flagEmg5 = 0;
bool flagEmg6 = 0;
bool flagEmg7 = 0;
bool flagEmg8 = 0;
bool flagEmg9 = 0;
bool flagEmg10 = 0;
bool flagEmg11 = 0;
bool flagEmg12 = 0;
bool flagEmg13 = 0;
bool flagEmg14 = 0;
bool flagEmg15 = 0;
bool flagEmg16 = 0;
bool flagEmg17 = 0;
bool flagEmg18 = 0;
bool flagEmg19 = 0;
bool flagEmg20 = 0;
int printEmg = 0;
bool flagPrintEmergencia = 0;
bool flagEmergencia = 0;
bool displayEncendido = 0;
int cicloNRF = 0;
int emgMandarSMS = 0;
int indicePerSMS = 0;
bool flagMensajeCorte = 0;
volatile int tiempoDelay = 0;
unsigned long inicio = 0;
bool flagSenial = 0;
volatile long tiempoDisplay = 60000;
volatile long timerSMS = 0;
volatile long timerNRF = 0;
volatile long timerEng = 0;
volatile long tiempoComprobar = 0;
volatile long tiempoPulsador = 0;
volatile long tiempoComandos = 0;
volatile long timerLecturaSim = 0;
volatile long timerLecturaPin = 0;
volatile long timerResetSim = 0;
volatile long tiempoCorte = 0;
volatile long tiempoSenal = 300000;
volatile long tiempoEmergencia = 0;
int indiceNum = 0;
String trama = "";
String tramaProceso = "";
String bufferRespuesta = "";
String tramaConectar = "";
int ind = 0;
String respuesta[5];
char numeroUsBackup[15];
char nombreUsBackup[5];
char nombrePerBackup[5];
char nombreS1Backup[5];
char nombreS2Backup[5];

per perifericos[20];
int indicePerifericos = 0;
String mensajesNRFTR[TAM_TR];
int indiceMensajesNRFTR = 0;
String mensajesNRF[TAM_NRF];
int indiceMensajesNRF = 0;
struct matriz {
  char numeroUs[15] = {"+"};
  char nombreUs[5];
  int indiceNumeroUs = 1;
  int indiceNombreUs = 0;
};
matriz usuario[20] = {};
int indiceUsuario = 0;



typedef enum {  // state machine
  ESPERA,
  ENVIO_CONECTAR
} PASOS_CONECTAR_t;
PASOS_CONECTAR_t CONECTAR_CEN;

typedef enum {
  INICIO,
  USUARIO,
  EDITAR_US,
  EDITAR_NOMBRE_US,
  EDITAR_NUM_US,
  ELIMINAR_US,
  AGREGAR_NOMBRE_US,
  AGREGAR_NUM_US,
  PERIFERICO,
  MOSTRAR_PER,
  EDITAR_PER,
  EDITAR_NOMBRE_PER,
  EDITAR_NOMBRES1_PER,
  EDITAR_ALERTAS1_PER,
  EDITAR_UMBRALS1_PER,
  EDITAR_UMBRALMAXS1_PER,
  EDITAR_UMBRALMINS1_PER,
  EDITAR_NOMBRES2_PER,
  EDITAR_ALERTAS2_PER,
  ELIMINAR_PER,
  AGREGAR_PER,
  AGREGAR_PER_NOMBRE,
  AGREGAR_PER_NS1,
  AGREGAR_PER_NS2,
  EMERGENCIA
} PASOS_DISPLAY_t;
PASOS_DISPLAY_t TFT_DISPLAY;

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
  APD,
  BAT,
  RSLT_S1,
  RSLT_S2,
} PASOS_DECODIFICADOR_t;

PASOS_DECODIFICADOR_t PDECO;

// Máquina de estados para secuencia de diagnóstico SIM800L
typedef enum {
  GSM_RESTART,
  GSM_INICIO,
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

PASOS_GSM_t pasoGSM = GSM_INICIO;

hw_timer_t *timer = NULL;  // timer
void IRAM_ATTR onTimer();  // function interrupts every 1ms


void IRAM_ATTR rotacionEncoder();
void IRAM_ATTR botonEncoder();

int displayLetras(char nombre[], int &indiceNombre);
int displayNum(char numero[], int &indiceNumero);
void gestionarComandosGSM();
SPIClass hspi(HSPI);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  analogReadResolution(12);
  hspi.begin(NRF_CLK, NRF_MISO, NRF_MOSI, PIN_CSN);
  SPI.begin(TFT_CLK, TFT_MISO, TFT_MOSI, TFT_CS);

  pinMode(PIN_RST, OUTPUT);  // Pin Reset es output
  pinMode(PIN_DTR, OUTPUT);
  digitalWrite(PIN_DTR, LOW);
  digitalWrite(PIN_RST, HIGH);  // Pin reset high está desactivado

  // SerialBT.begin("Central_Dev");  // Nombre del dispositivo Bluetooth
  sim800l.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  strcpy(usuario[indiceUsuario].numeroUs, "+5491161386381");
  strcpy(usuario[indiceUsuario].nombreUs, "AYEE");
  indiceUsuario ++;
  strcpy(perifericos[indicePerifericos].nombreExterno, "PER1");
  indicePerifericos ++;

  pinMode(PIN_ENERGIA, INPUT);
  pinMode(TFT_BL, OUTPUT);
  radio.begin(&hspi);
  radio.setAutoAck(true);
  radio.setChannel(108);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, direccionB);
  radio.openWritingPipe(direccionA);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
  Serial.begin(9600);
  tft.begin(1000000);
  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);

  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(ENCODER_SW_PIN, INPUT);

  digitalWrite(TFT_BL, LOW);

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), rotacionEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_SW_PIN), botonEncoder, FALLING);

  TFT_DISPLAY = INICIO;
  PSMS = PASO1;
  PDECO = APD;
  PRST = RST;

  // Timer de sistema activado antes de iniciar el conteo de GSM
  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms
  tiempoComandos = 0;

  ultimoValorCLK = digitalRead(ENCODER_A_PIN);
  ultimoValorDT = digitalRead(ENCODER_B_PIN);
  strcpy(perifericos[0].nombreInterno, "P1");
  strcpy(perifericos[1].nombreInterno, "P2");
  strcpy(perifericos[2].nombreInterno, "P3");
  strcpy(perifericos[3].nombreInterno, "P4");
  strcpy(perifericos[4].nombreInterno, "P5");
  strcpy(perifericos[5].nombreInterno, "P6");
  strcpy(perifericos[6].nombreInterno, "P7");
  strcpy(perifericos[7].nombreInterno, "P8");
  strcpy(perifericos[8].nombreInterno, "P9");
  strcpy(perifericos[9].nombreInterno, "P10");
  strcpy(perifericos[10].nombreInterno, "P11");
  strcpy(perifericos[11].nombreInterno, "P12");
  strcpy(perifericos[12].nombreInterno, "P13");
  strcpy(perifericos[13].nombreInterno, "P14");
  strcpy(perifericos[14].nombreInterno, "P15");
  strcpy(perifericos[15].nombreInterno, "P16");
  strcpy(perifericos[16].nombreInterno, "P17");
  strcpy(perifericos[17].nombreInterno, "P18");
  strcpy(perifericos[18].nombreInterno, "P19");
  strcpy(perifericos[19].nombreInterno, "P20");

  tft.sendCommand(0x10);

  xTaskCreatePinnedToCore(
  simCode,
  "sim",
  10000,
  NULL,
  1,
  &sim,
  0
);
}

void loop() {                                                        
  lecturaEnergia();
  if(flagEmergencia == 0 && (displayModoSleep || TFT_DISPLAY == INICIO || TFT_DISPLAY == MOSTRAR_PER || TFT_DISPLAY == USUARIO || TFT_DISPLAY == PERIFERICO || TFT_DISPLAY == EDITAR_PER || TFT_DISPLAY == EDITAR_US)){
    if(perifericos[0].emergenciaS1 > 0 || perifericos[0].emergenciaS2 > 0){
      if(!flagEmg1){
        printEmg = 0;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg1 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
        comandoMandadoSleep = 0;
      }
    }
    else{
      if(flagEmg1){
        flagEmg1 = 0;
      }
    }
    if(perifericos[1].emergenciaS1 > 0 || perifericos[1].emergenciaS2 > 0){
      if(!flagEmg2){
        TFT_DISPLAY = EMERGENCIA;
        printEmg = 1;
        flagEmg2 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg2){
        flagEmg2 = 0;
      }
    }
    if(perifericos[2].emergenciaS1 > 0 || perifericos[2].emergenciaS2 > 0){
      if(!flagEmg3){
        printEmg = 2;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg3 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg3){
        flagEmg3 = 0;
      }
    }
    if(perifericos[3].emergenciaS1 > 0 || perifericos[3].emergenciaS2 > 0){
      if(!flagEmg4){
        printEmg = 3;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg4 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg4){
        flagEmg4 = 0;
      }
    }
    if(perifericos[4].emergenciaS1 > 0 || perifericos[4].emergenciaS2 > 0){
      if(!flagEmg5){
        printEmg = 4;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg5 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg5){
        flagEmg5 = 0;
      }
    }
    if(perifericos[5].emergenciaS1 > 0 || perifericos[5].emergenciaS2 > 0){
      if(!flagEmg6){
        printEmg = 5;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg6 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg6){
        flagEmg6 = 0;
      }
    }
    if(perifericos[6].emergenciaS1 > 0 || perifericos[6].emergenciaS2 > 0){
      if(!flagEmg7){
        printEmg = 6;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg7 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg7){
        flagEmg7 = 0;
      }
    }
    if(perifericos[7].emergenciaS1 > 0 || perifericos[7].emergenciaS2 > 0){
      if(!flagEmg8){
        printEmg = 7;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg8 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg8){
        flagEmg8 = 0;
      }
    }
    if(perifericos[8].emergenciaS1 > 0 || perifericos[8].emergenciaS2 > 0){
      if(!flagEmg9){
        printEmg = 8;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg9 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg9){
        flagEmg9 = 0;
      }
    }
    if(perifericos[9].emergenciaS1 > 0 || perifericos[9].emergenciaS2 > 0){
      if(!flagEmg10){
        printEmg = 9;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg10 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg10){
        flagEmg10 = 0;
      }
    }
    if(perifericos[10].emergenciaS1 > 0 || perifericos[10].emergenciaS2 > 0){
      if(!flagEmg11){
        printEmg = 10;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg11 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg11){
        flagEmg11 = 0;
      }
    }
    if(perifericos[11].emergenciaS1 > 0 || perifericos[11].emergenciaS2 > 0){
      if(!flagEmg12){
        printEmg = 11;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg12 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg12){
        flagEmg12 = 0;
      }
    }
    if(perifericos[12].emergenciaS1 > 0 || perifericos[12].emergenciaS2 > 0){
      if(!flagEmg13){
        printEmg = 12;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg13 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg13){
        flagEmg13 = 0;
      }
    }
    if(perifericos[13].emergenciaS1 > 0 || perifericos[13].emergenciaS2 > 0){
      if(!flagEmg14){
        printEmg = 13;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg14 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg14){
        flagEmg14 = 0;
      }
    }
    if(perifericos[14].emergenciaS1 > 0 || perifericos[14].emergenciaS2 > 0){
      if(!flagEmg15){
        printEmg = 14;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg15 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg15){
        flagEmg15 = 0;
      }
    }
    if(perifericos[15].emergenciaS1 > 0 || perifericos[15].emergenciaS2 > 0){
      if(!flagEmg16){
        printEmg = 15;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg16 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg16){
        flagEmg16 = 0;
      }
    }
    if(perifericos[16].emergenciaS1 > 0 || perifericos[16].emergenciaS2 > 0){
      if(!flagEmg17){
        printEmg = 16;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg17 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg17){
        flagEmg17 = 0;
      }
    }
    if(perifericos[17].emergenciaS1 > 0 || perifericos[17].emergenciaS2 > 0){
      if(!flagEmg18){
        printEmg = 17;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg18 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg18){
        flagEmg18 = 0;
      }
    }
    if(perifericos[18].emergenciaS1 > 0 || perifericos[18].emergenciaS2 > 0){
      if(!flagEmg19){
        printEmg = 18;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg19 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
      }
    }
    else{
      if(flagEmg19){
        flagEmg19 = 0;
      }
    }
    if(perifericos[19].emergenciaS1 > 0 || perifericos[19].emergenciaS2 > 0){
      if(!flagEmg20){
        printEmg = 19;
        TFT_DISPLAY = EMERGENCIA;
        flagEmg20 = 1;
        flagEmergencia = 1;
        flagPrintEmergencia = 1;
        tiempoDisplay = 0;
        comandoMandadoSleep = 0;
      }
    }
    else{
      if(flagEmg20){
        flagEmg20 = 0;
      }
    }
  }
  if(tiempoDisplay >= 10000){
    if(!displayModoSleep){
      tft.sendCommand(0x10);
      digitalWrite(TFT_BL, LOW);
      displayModoSleep = 1;
      comandoMandadoSleep = 0;
    }
  }
  else{
    if(displayModoSleep){
      if(!comandoMandadoSleep){
        tft.sendCommand(0x11);
        comandoMandadoSleep = 1;
        tiempoComandoMandado = 0;
      }
      if(tiempoComandoMandado >= 120){
        digitalWrite(TFT_BL, HIGH);
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 10);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(1);
        tft.print("bat:");
        if(flagCorte){
          tft.print(porcentajeBateria);
          tft.print("%");
        }
        else{
          tft.print("Cargando");
        }
        tft.setCursor(100, 10);
        tft.print("Senial:");
        if(senalPrint){
          tft.print("Hay");
        }
        else{
        tft.print("No hay");
        }
        if(TFT_DISPLAY != EMERGENCIA){
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("USUARIOS");
          tft.setCursor(10, 95);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("PERIFERICOS");
          TFT_DISPLAY = INICIO;
        }
        displayModoSleep = 0;
      }
    }
    if(tiempoComandoMandado >= 120){
      display();
    }
  }

  if(indiceMensajesNRF >= 1 && !mandandoSMS){
    if(timerNRF >= TIEMPO_ENVIAR_NRF){
      if(respuestaNRF == 1){
        if(eliminarPrimero(mensajesNRF, TAM_NRF)){
          indiceMensajesNRF --;
          respuestaNRF = 0;
          cicloNRF = 0;
        }
      }
      else if(respuestaNRF == 0 && cicloNRF < 5){
        if(indiceMensajesNRF >= 1){
          mandarNRF(mensajesNRF[0]);
        }
        cicloNRF += 1;
        timerNRF = 0;
        if(mensajesNRF[0].endsWith("recibido*")){
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
  recibirNRF();
  decodificador();
  umbral();
  // lectura fuente
  /*if (timerLecturaPin >= 1000){
  int lectura = analogRead(PIN_ENERGIA);
   int voltaje = map(lectura, 0, 2735, 0, 12);
   float lecturaVoltaje = (voltaje - 0.4) * 10/43;
    // SerialBT.println("V: ");
    // SerialBT.println(voltaje);
    // SerialBT.println(" ");

  }*/
}

void simCode(void *pvParameters){
  for (;;) {
    bool simActivo = 0;
    if (terminarGSMS) {
      manejoMandarSMS();
      if(!mandandoSMS){
        if(tiempoSenal >= TIEMPO_SENAL){
          verificarSenalSIM800L();
          simActivo = 1;
        }
      }
      else{
        simActivo = 1;
      }
    }
    else{
      simActivo = 1;
    }
    gestionarComandosGSM();
    if (!simActivo && pasoGSM == GSM_READY && !mandandoSMS) {
      vTaskDelay(pdMS_TO_TICKS(10));
    } else {
      vTaskDelay(pdMS_TO_TICKS(2));
    }
  }
}

void gestionarComandosGSM() {
  if (pasoGSM == GSM_READY) return;

  switch (pasoGSM) {
    case GSM_RESTART:  // nuevo estado de reset
      // SerialBT.flush();
      reiniciarSIM800L();  // Llama a tu función de reset por hardware

      if (PRST == SIM_IDLE) {  // Una vez terminado el ciclo de reset
        tiempoComandos = 0;
        pasoGSM = GSM_INICIO;  // Vuelve a empezar la inicialización
      }
      break;
    case GSM_INICIO:
      if (tiempoComandos >= 15000) {
        bufferRespuesta = "";
        sim800l.println("AT");
        // SerialBT.println("Comando AT");
        tiempoComandos = 0;
        pasoGSM = GSM_AT;
      }
      break;

    case GSM_AT:
      // Esperar OK del comando AT previo
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;

        if (bufferRespuesta.indexOf("OK") != -1) {
          // SerialBT.println(bufferRespuesta);
          sim800l.println("AT+CFUN?");
          // SerialBT.println("Comando AT+cfun?");
          bufferRespuesta = "";
          tiempoComandos = 0;
          pasoGSM = GSM_CFUN;
        }
      }
      // Fallo por Timeout: Si en 3 segundos no responde OK, mandar a reiniciar
      else if (tiempoComandos >= 3000) {
        // SerialBT.println(bufferRespuesta);
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CFUN:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }
      if (bufferRespuesta.indexOf("OK") != -1) {
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CPIN?");
        // SerialBT.println("Comando AT+cpin?");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CPIN;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
      // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CFUN?");
        // SerialBT.println("Comando AT+cfun?");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        // SerialBT.println(bufferRespuesta);
        tiempoComandos = 0;
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CPIN:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }

      if (bufferRespuesta.indexOf("OK") != -1) {
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSCA?");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CSCA;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CPIN?");
        // SerialBT.println("Comando AT+cpin?");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        // SerialBT.println(bufferRespuesta);
        tiempoComandos = 0;
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CSCA:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }

      if (bufferRespuesta.indexOf("OK") != -1) {
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSQ");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CSQ;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSCA?");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        // SerialBT.println(bufferRespuesta);
        tiempoComandos = 0;
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CSQ:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }

      if (bufferRespuesta.indexOf("OK") != -1) {
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CREG?");
        // SerialBT.println("Comando AT+creg");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CREG;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSQ");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= TIMEOUT){
        // SerialBT.println(bufferRespuesta);
        tiempoComandos = 0;
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CREG:
      // 1. Leer respuesta si está disponible
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }
      // 2. Re-consultar AT+CREG? cada 2 segundos mientras se espera red
      static unsigned long timerReintentoCREG = 0;
      if (tiempoComandos - timerReintentoCREG >= 2000) {
        // SerialBT.println(bufferRespuesta);
        bufferRespuesta = "";
        timerReintentoCREG = tiempoComandos;
        sim800l.println("AT+CREG?");
      }
      if (bufferRespuesta.indexOf("+CREG: 0,1") != -1 || bufferRespuesta.indexOf("+CREG: 0,5") != -1 || bufferRespuesta.indexOf("+CREG: 1") != -1 || bufferRespuesta.indexOf("+CREG: 5") != -1) {
        // SerialBT.println(bufferRespuesta);
        bufferRespuesta = "";
        sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
        tiempoComandos = 0;
        pasoGSM = GSM_CPMS;
        break;
      } else if (bufferRespuesta.indexOf("OK") != -1 || bufferRespuesta.indexOf("ERROR") != -1) {
        // SerialBT.println(bufferRespuesta);
        bufferRespuesta = "";
      }

      // 3. Si pasan 30 segundos acumulados sin enganchar red -> Reiniciar
      else if (tiempoComandos >= 30000) {
        // SerialBT.println(bufferRespuesta);
        timerReintentoCREG = 0;
        pasoGSM = GSM_RESTART;
      }
      break;


    case GSM_CPMS:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }

      if (bufferRespuesta.indexOf("OK") != -1) {
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CMGF=1");
        // SerialBT.println("Comando AT+cmgf=1");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CMGF;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        // SerialBT.println(bufferRespuesta);
        tiempoComandos = 0;
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CMGF:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }

      if (bufferRespuesta.indexOf("OK") != -1) {
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CNMI=2,2,0,0,0");
        // SerialBT.println("at+cnmi=2,2,0,0,0");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CNMI;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CMGF=1");
        // SerialBT.println("Comando AT+cmgf=1");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        // SerialBT.println(bufferRespuesta);
        tiempoComandos = 0;
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CNMI:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }

      if (bufferRespuesta.indexOf("OK") != -1) {
        // SerialBT.println(bufferRespuesta);
        // SerialBT.println("trama: #EMG,ApodoDisp,ResSens1,ResSens2,ResSens3*");
        // SerialBT.println("SIM800L Ready");
        bufferRespuesta = "";
        terminarGSMS = 1;
        sim800l.println("AT+CSCLK=1");
        vTaskDelay(pdMS_TO_TICKS(50));
        digitalWrite(PIN_DTR, HIGH);
        pasoGSM = GSM_READY;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        // SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CNMI=2,2,0,0,0");
        // SerialBT.println("at+cnmi=2,2,0,0,0");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        // SerialBT.println(bufferRespuesta);
        tiempoComandos = 0;
        pasoGSM = GSM_RESTART;
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
      digitalWrite(PIN_DTR, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));
      // Paso 2: Enviar comando AT
      bufferRespuesta = "";
      sim800l.println("AT+CSQ");
      tiempoComandos = 0;
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
                // SerialBT.println("HAY SEÑAL ESTABLE");
                senalPrint = 1;
                resultado = true;
              } else {
                // SerialBT.println("NO HAY SEÑAL ESTABLE");
                senalPrint = 0;
              }
            }
          }
          // Reiniciar para la próxima verificación
          SENAL = IDLE;
          digitalWrite(PIN_DTR, HIGH);
          tiempoSenal = 0;
          return resultado;
        }
      }

      // Paso 4: Control de Timeout si el SIM800L no responde
      if (tiempoComandos >= TIMEOUT) {
        // SerialBT.println("TIMEOUT");
        digitalWrite(PIN_DTR, HIGH);
        tiempoSenal = 0;
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
      // SerialBT.println("SIM800L Reset");
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

void rotacionEncoder() {
  int valorCLK = digitalRead(ENCODER_A_PIN);
  int valorDT = digitalRead(ENCODER_B_PIN);

  if (valorCLK != ultimoValorCLK) {
    if (valorDT != valorCLK) {
      valorEncoder--;
    } else {
      valorEncoder++;
    }
    tiempoDisplay = 0;
  }

  ultimoValorCLK = valorCLK;
  ultimoValorDT = valorDT;
}

void botonEncoder() {
  if (debounceSWEncoder >= TIEMPO_DEBOUNCE) {
    debounceSWEncoder = 0;
    flagSWEncoder = 1;
    tiempoDisplay = 0;
  }
}

void IRAM_ATTR onTimer() {
  tiempoEmergencia += 1;
  tiempoDisplayBat += 1;
  tiempoDisplay += 1;
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
  tiempoCorte += 1;
  tiempoAgregarPer += 1;
  debounceSWEncoder += 1;
  parpadeo += 1;
  tiempoConectar += 1;
  tiempoSenal += 1;
  tiempoComandoMandado += 1;
}