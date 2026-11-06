#include "BluetoothSerial.h"
#include <HardwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define RX_PIN 19
#define TX_PIN 21
#define PIN_ENERGIA 35
#define PIN_CE 5
#define PIN_CSN 15
#define PIN_RST 22
#define TFT_DC 14
#define TFT_CS 32
#define TFT_MOSI 2
#define TFT_CLK 17
#define TFT_RST 27
#define TFT_MISO 16
#define TFT_BL 12
#define ENCODER_A_PIN 25
#define ENCODER_B_PIN 26
#define ENCODER_SW_PIN 39

#define TIEMPO_DEBOUNCE 200
#define TIEMPO_PARPADEO 500
#define TIMEOUT_AGREGAR 1000
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

#define ANCHO 18
#define TAM_TR 10
#define TAM_NRF 10

HardwareSerial sim800l(2);
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
    tipoSensor1 = 0;
    tipoSensor2 = 0;
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

const uint64_t direccion = 0xF0F0F0F0E1LL;

RF24 radio(PIN_CE, PIN_CSN);

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
int indiceNum = 0;
String trama = "";
String tramaProceso = "";
String bufferRespuesta = "";
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


void IRAM_ATTR rotacionEncoder();
void IRAM_ATTR botonEncoder();

int displayLetras(char nombre[], int &indiceNombre);
int displayNum(char numero[], int &indiceNumero);
void gestionarComandosGSM();

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  SPI.begin(TFT_CLK, TFT_MISO, TFT_MOSI);

  pinMode(PIN_RST, OUTPUT);  // Pin Reset es output
  digitalWrite(PIN_RST, HIGH);  // Pin reset high está desactivado

  SerialBT.begin("Central_Dev");  // Nombre del dispositivo Bluetooth
  sim800l.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  strcpy(usuario[indiceUsuario].numeroUs, "+5491161386381");
  indiceUsuario ++;

  pinMode(PIN_ENERGIA, INPUT_PULLUP);
  radio.begin();
  radio.openReadingPipe(1, direccion);
  radio.openWritingPipe(direccion);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(ENCODER_SW_PIN, INPUT);
  pinMode(TFT_BL, OUTPUT);

  digitalWrite(TFT_BL, HIGH);

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

  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(4);
  tft.print("USUARIOS");
  tft.setCursor(10, 70);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(4);
  tft.print("PERIFERICOS");

  strcpy(usuario[0].nombreUs, "AAAA");
  strcpy(usuario[1].nombreUs, "BBBB");
  strcpy(usuario[2].nombreUs, "CCCC");
  strcpy(usuario[3].nombreUs, "DDDD");
  strcpy(usuario[4].nombreUs, "EEEE");
  strcpy(usuario[5].nombreUs, "FFFF");
  strcpy(usuario[6].nombreUs, "GGGG");
  strcpy(usuario[7].nombreUs, "HHHH");
  strcpy(usuario[8].nombreUs, "IIII");
  strcpy(usuario[9].nombreUs, "JJJJ");
  indiceUsuario = 10;
  strcpy(perifericos[0].nombreExterno, "1111");
  strcpy(perifericos[1].nombreExterno, "2222");
  strcpy(perifericos[2].nombreExterno, "3333");
  strcpy(perifericos[3].nombreExterno, "4444");
  strcpy(perifericos[4].nombreExterno, "5555");
  strcpy(perifericos[5].nombreExterno, "6666");
  strcpy(perifericos[6].nombreExterno, "7777");
  strcpy(perifericos[7].nombreExterno, "8888");
  strcpy(perifericos[8].nombreExterno, "9999");
  strcpy(perifericos[9].nombreExterno, "0000");
  indicePerifericos = 10;
}

void loop() {
  // Manejo secuencial no bloqueante del módulo GSM
  if (terminarGSMS) {
    if(!mandandoSMS){
    verificarSenalSIM800L();
    }
  }
  gestionarComandosGSM();
  lecturaEnergia();

  display();

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
          mandarNRF(mensajesNRF[0], direccion);
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

  // lectura fuente
  /*if (timerLecturaPin >= 1000){
  int lectura = analogRead(PIN_ENERGIA);
   int voltaje = map(lectura, 0, 2735, 0, 12);
   float lecturaVoltaje = (voltaje - 0.4) * 10/43;
    SerialBT.println("V: ");
    SerialBT.println(voltaje);
    SerialBT.println(" ");

  }*/
}

void gestionarComandosGSM() {
  if (pasoGSM == GSM_READY) return;

  switch (pasoGSM) {
    case GSM_RESTART:  // nuevo estado de reset
      SerialBT.flush();
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
        SerialBT.println("Comando AT");
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
          SerialBT.println(bufferRespuesta);
          sim800l.println("AT+CFUN?");
          SerialBT.println("Comando AT+cfun?");
          bufferRespuesta = "";
          tiempoComandos = 0;
          pasoGSM = GSM_CFUN;
        }
      }
      // Fallo por Timeout: Si en 3 segundos no responde OK, mandar a reiniciar
      else if (tiempoComandos >= 3000) {
        SerialBT.println(bufferRespuesta);
        pasoGSM = GSM_RESTART;
      }
      break;

    case GSM_CFUN:
      if (sim800l.available()) {
        char c = sim800l.read();
        bufferRespuesta += c;
      }
      if (bufferRespuesta.indexOf("OK") != -1) {
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CPIN?");
        SerialBT.println("Comando AT+cpin?");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CPIN;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
      SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CFUN?");
        SerialBT.println("Comando AT+cfun?");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        SerialBT.println(bufferRespuesta);
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
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSCA?");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CSCA;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CPIN?");
        SerialBT.println("Comando AT+cpin?");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        SerialBT.println(bufferRespuesta);
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
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSQ");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CSQ;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSCA?");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        SerialBT.println(bufferRespuesta);
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
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CREG?");
        SerialBT.println("Comando AT+creg");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CREG;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CSQ");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= TIMEOUT){
        SerialBT.println(bufferRespuesta);
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
        SerialBT.println(bufferRespuesta);
        bufferRespuesta = "";
        timerReintentoCREG = tiempoComandos;
        sim800l.println("AT+CREG?");
      }
      if (bufferRespuesta.indexOf("+CREG: 0,1") != -1 || bufferRespuesta.indexOf("+CREG: 0,5") != -1 || bufferRespuesta.indexOf("+CREG: 1") != -1 || bufferRespuesta.indexOf("+CREG: 5") != -1) {
        SerialBT.println(bufferRespuesta);
        bufferRespuesta = "";
        sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
        tiempoComandos = 0;
        pasoGSM = GSM_CPMS;
        break;
      } else if (bufferRespuesta.indexOf("OK") != -1 || bufferRespuesta.indexOf("ERROR") != -1) {
        SerialBT.println(bufferRespuesta);
        bufferRespuesta = "";
      }

      // 3. Si pasan 30 segundos acumulados sin enganchar red -> Reiniciar
      else if (tiempoComandos >= 30000) {
        SerialBT.println(bufferRespuesta);
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
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CMGF=1");
        SerialBT.println("Comando AT+cmgf=1");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CMGF;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        SerialBT.println(bufferRespuesta);
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
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CNMI=2,2,0,0,0");
        SerialBT.println("at+cnmi=2,2,0,0,0");
        bufferRespuesta = "";
        tiempoComandos = 0;
        pasoGSM = GSM_CNMI;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CMGF=1");
        SerialBT.println("Comando AT+cmgf=1");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        SerialBT.println(bufferRespuesta);
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
        SerialBT.println(bufferRespuesta);
        SerialBT.println("trama: #EMG,ApodoDisp,ResSens1,ResSens2,ResSens3*");
        SerialBT.println("SIM800L Ready");
        bufferRespuesta = "";
        terminarGSMS = 1;
        pasoGSM = GSM_READY;
      }
      else if (bufferRespuesta.indexOf("ERROR") != -1){
        SerialBT.println(bufferRespuesta);
        sim800l.println("AT+CNMI=2,2,0,0,0");
        SerialBT.println("at+cnmi=2,2,0,0,0");
        bufferRespuesta = "";
        tiempoComandos = 0;
      }
      else if(tiempoComandos >= CONFIG_TIMEOUT){
        SerialBT.println(bufferRespuesta);
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
                //SerialBT.println("HAY SEÑAL ESTABLE");
                resultado = true;
              } else {
                //SerialBT.println("NO HAY SEÑAL ESTABLE");
              }
            }
          } else {
          }

          // Reiniciar para la próxima verificación
          SENAL = IDLE;
          return resultado;
        }
      }

      // Paso 4: Control de Timeout si el SIM800L no responde
      if (tiempoComandos >= TIMEOUT) {
        SerialBT.println("TIMEOUT");
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
      SerialBT.println("SIM800L Reset");
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
  }

  ultimoValorCLK = valorCLK;
  ultimoValorDT = valorDT;
}

void botonEncoder() {
  if (debounceSWEncoder >= TIEMPO_DEBOUNCE) {
    debounceSWEncoder = 0;
    flagSWEncoder = 1;
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
}