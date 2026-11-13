#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "BluetoothSerial.h"

#define RX_PIN 19
#define TX_PIN 21
#define PIN_DTR 23
#define PIN_ENERGIA 35
#define PIN_CE 5
#define PIN_CSN 15
#define NRF_MOSI 2
#define NRF_MISO 16
#define NRF_CLK 17
#define PIN_BUZZER 33
#define PIN_RST 22
#define TFT_DC 14
#define TFT_CS 32
#define TFT_MOSI 12
#define TFT_MISO 13
#define TFT_CLK 27
// #define TFT_BL 12
#define ENCODER_A_PIN 25
#define ENCODER_B_PIN 26
#define ENCODER_SW_PIN 39

#define TIEMPO_DEBOUNCE 200
#define TIEMPO_PARPADEO 500
#define TIMEOUT_AGREGAR 20000
#define TIEMPO_SENAL 300000
#define TIEMPO_RST 100               // Tiempo reset sms
#define TIEMPO_ESTABILIDAD_RST 3000  // Tiempo post reset
#define TFT_DC 14
#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_ENVIAR_SMS 5000
#define TIEMPO_ENERGIA_PERIFERICOS 100000
#define TIEMPO_LED 100
#define TIEMPO_COMPROBAR 1000
#define TIMEOUT 2000
#define CONFIG_TIMEOUT 1000
#define TIEMPO_CONECTAR_PER 1000
#define ANCHO 18
#define TAM_TR 10
#define TAM_NRF 10

RF24 radio(PIN_CE, PIN_CSN);

BluetoothSerial SerialBT;

class per {
public:
  char nombreInterno[4] = { "" };
  char nombreExterno[5] = { "----" };
  char nombreSensor1[5] = { "----" };
  char nombreSensor2[5] = { "----" };
  int tipoSensor1;
  int tipoSensor2;
  String lecturaSensor1 = "-";
  String lecturaSensor2 = "-";
  int emergenciaS1 = 0;
  int emergenciaS2 = 0;
  bool flagEmgS1 = 0;
  bool flagEmgS2 = 0;
  int indiceNombrePer = 0;
  int indiceNombreS1 = 0;
  int indiceNombreS2 = 0;
  int umbralMinS1 = 0;
  int umbralMinS2 = 0;
  int umbralMaxS1 = 0;
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

char letras[27] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
char numeros[11] = { "1234567890" };
volatile int valorEncoder = 0;
int ultimoValorCLK = 0;
int ultimoValorDT = 0;
volatile int debounceSWEncoder = 0;
volatile int tiempoAgregarPer = 0;
volatile int parpadeo = 0;
volatile unsigned int tiempoConectar = 0;
int valorDisplay = 1;
int modificarUsuario = 0;
int modificarPer = 0;
int eliminar = 0;
bool seleccionar = 0;
bool flagSWEncoder = 0;
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
int cicloNRF = 0;
int emgMandarSMS = 0;
int indicePerSMS = 0;
bool flagMensajeCorte = 0;
volatile int tiempoDelay = 0;
unsigned long inicio = 0;
bool flagSenial = 0;
volatile int timerSMS = 0;
volatile int timerNRF = 0;
volatile int timerEng = 0;
volatile int tiempoComprobar = 0;
volatile int tiempoPulsador = 0;
volatile int tiempoComandos = 0;
volatile int timerLecturaSim = 0;
volatile int timerLecturaPin = 0;
volatile int timerResetSim = 0;
volatile int tiempoCorte = 0;
volatile int tiempoSenal = 0;
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

typedef enum {  // state machine
  INICIO,
  USUARIO,
  EDITAR_US,
  EDITAR_NOMBRE_US,
  EDITAR_NUM_US,
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
  EDITAR_UMBRALS2_PER,
  EDITAR_UMBRALMINS2_PER,
  EDITAR_UMBRALMAXS2_PER,
  ELIMINAR_PER,
  AGREGAR_PER,
  AGREGAR_PER_NOMBRE,
  AGREGAR_PER_NS1,
  AGREGAR_PER_NS2
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

bool conectar(String nombreConectar, volatile unsigned int &tiempoConectar);
void recibirNRF();

// SPIClass hspi(HSPI);

void setup() {
  SerialBT.begin("Central_Dev");
  SPI.begin(NRF_CLK, NRF_MISO, NRF_MOSI);
  // hspi.begin(NRF_CLK, NRF_MISO, NRF_MOSI, PIN_CSN);
  pinMode(PIN_BUZZER, OUTPUT);
  while(!radio.begin()){}
  digitalWrite(PIN_BUZZER, LOW);
  radio.setChannel(108);
  radio.openReadingPipe(1, direccionB);
  radio.openWritingPipe(direccionA);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
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
  delay(5000);
  static char mensajeConectar[8];
  memset(mensajeConectar, 0, sizeof(mensajeConectar));
  snprintf(mensajeConectar, sizeof(mensajeConectar), "#C,%s*", String(perifericos[indicePerifericos].nombreInterno));
        SerialBT.println(mensajeConectar);
  // Timer de sistema activado antes de iniciar el conteo de GSM
  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms
  tiempoComandos = 0;
}

void loop() {
  if(indicePerifericos < 20){
    if(tiempoAgregarPer <= TIMEOUT_AGREGAR){
      if(conectar(String(perifericos[indicePerifericos].nombreInterno), tiempoConectar)){
        digitalWrite(PIN_BUZZER, HIGH);
        delay(5000);
        digitalWrite(PIN_BUZZER, LOW);
        indicePerifericos ++;
        tiempoAgregarPer = 0;
      }
    }
    else if(tiempoAgregarPer >= TIMEOUT_AGREGAR){
        digitalWrite(PIN_BUZZER, HIGH);
        delay(1000);
        digitalWrite(PIN_BUZZER, LOW);
      tiempoAgregarPer = 0;
    }
  }
  recibirNRF();
}
void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
        SerialBT.print("trama: ");
        SerialBT.println(trama);
    String tempRecibir = mensajesNRF[0].substring(0, mensajesNRF[0].indexOf(','));
    tempRecibir += ",recibido*";
    if(indiceMensajesNRF > 0 && mensajesNRF[0].length() > 0 && mensajesNRF[0].indexOf(',') != -1) {
      if (trama == tempRecibir){
        Serial.println(trama);
        SerialBT.print("trama: ");
        SerialBT.println(trama);
        respuestaNRF = 1;
        return;
      }
    }
    if(trama.startsWith("#P") && trama.endsWith("*")){
      Serial.println(trama);
        SerialBT.print("trama: ");
        SerialBT.println(trama);
      if(indiceMensajesNRFTR < TAM_TR){
        mensajesNRFTR[indiceMensajesNRFTR] = trama;
        indiceMensajesNRFTR ++;
      }
    }
    else if(trama.startsWith("#C") && trama.endsWith("*") && conectando){
      Serial.println(trama);
        SerialBT.print("trama: ");
        SerialBT.println(trama);
      tramaConectar = trama;
      mensajeRecibidoConectar = 1;
    }
    else if(trama == "OK" && conectando){
      Serial.println(trama);
        SerialBT.print("trama: ");
        SerialBT.println(trama);
      mensajeRecibidoOK = 1;
    }
    trama = "";
  }
}
bool conectar(String nombreConectar, volatile unsigned int &tiempoConectar){
  conectando = 1;
  static char mensajeConectar[8];
  static char mensajeRecibidoNRF[32];
  switch(CONECTAR_CEN){
    case ESPERA:
      if(mensajeRecibidoConectar){
        if(isDigit(tramaConectar.charAt(tramaConectar.indexOf(',') + 1))){
          perifericos[indicePerifericos].tipoSensor1 = String(tramaConectar.charAt(tramaConectar.indexOf(',') + 1)).toInt();
          if(isDigit(tramaConectar.charAt(tramaConectar.indexOf(',') + 3))){
            perifericos[indicePerifericos].tipoSensor2 = String(tramaConectar.charAt(tramaConectar.indexOf(',') + 3)).toInt();
            memset(mensajeConectar, 0, sizeof(mensajeConectar));
            snprintf(mensajeConectar, sizeof(mensajeConectar), "#C,%s*", nombreConectar.c_str());
            CONECTAR_CEN = ENVIO_CONECTAR;
          }
        }
        mensajeRecibidoConectar = 0;
      }
      return false; 
      break;
    case ENVIO_CONECTAR:
      if(tiempoConectar % TIEMPO_CONECTAR_PER == 0){
        radio.stopListening();
        radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
        Serial.println(String(mensajeConectar));
        SerialBT.print("mc: ");
        SerialBT.println(mensajeConectar);
        radio.startListening();
      }
      if(mensajeRecibidoOK){
        mensajeRecibidoOK = 0;
        CONECTAR_CEN = ESPERA;
        conectando = 0;
        return true;
      }
      return false; 
      break;
  }
}


void IRAM_ATTR onTimer() {
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
}