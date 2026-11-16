#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_ENVIO_DATOS 10000
#define TIEMPO_LEER_SENSORES 60000
#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 20000

const uint64_t direccionA = 0xF0F0F0F0E0LL;
const uint64_t direccionB = 0xF0F0F0F0E1LL;

#define TAM_NRF 5
#define TAM_MSG 20
#define PRESIONADO 0
#define NO_PRESIONADO 1

#define SW_PIN PIN_PB0
#define PIN_CE PIN_PA3
#define PIN_CSN PIN_PA7

#define PIN_TERMISTOR PIN_PA1
#define PIN_PUERTA PIN_PA2
#define PIN_ENERGIA PIN_PA0
#define PIN_LED PIN_PB1

#define ABIERTO 1
#define CERRADO 0
#define DEBOUNCE 20

#define TIPO_SENSOR_1 1
#define TIPO_SENSOR_2 2

// Variables de sensores
int16_t tempDecimas = 0;
uint8_t porcentajeBateria = 0;
bool puerta = CERRADO;

bool estadoPuerta = CERRADO;
bool ultimaLecturaPuerta = CERRADO;
unsigned long ultimoTiempoDebounce = 0;
unsigned long ultimaLecturaAnalogica = 0;
unsigned long ultimoEnvioDatos = 0;

// Variables NRF y estado
char mensajeRecibidoNRF[33];
bool respuestaNRF = 0;
uint8_t cicloNRF = 0;
unsigned long tiempoNRFAnterior = 0;
unsigned long tiempoConeccionAnterior = 0;
unsigned long tiempoNRF = 0;
unsigned long tiempoConeccion = 0;

unsigned long ultimoEnvioConectar = 0;
bool primerIntentoConectar = false;

char nombre[4] = "";
char mensaje[10] = "";

char mensajesNRF[TAM_NRF][TAM_MSG];
uint8_t indiceMensajesNRF = 0;

bool flagSW = 0;
bool coneccion = 0;
bool flagConectado = 0;

RF24 radio(PIN_CE, PIN_CSN);

// Prototipos
bool conectar(const char mensajeConectar[]);
bool eliminarPrimero(char miLista[][TAM_MSG], int tamLista);
void mandarNRF(const char mensajeEnviar[]);
void enviarDatosCentral();
int16_t leerTemperaturaTermistor();
bool lecturaPuerta();
uint8_t lecturaBaterias();

void setup() {
  pinMode(SW_PIN, INPUT);
  pinMode(PIN_PUERTA, INPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);
  memset(mensajesNRF, 0, sizeof(mensajesNRF));

  radio.begin();
  radio.setAutoAck(true);
  radio.setRetries(15, 15);
  radio.setChannel(108);
  radio.setCRCLength(RF24_CRC_16);
  radio.openReadingPipe(1, direccionA);
  radio.openWritingPipe(direccionB);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  // Arma "#C,1,2*" sin usar snprintf para ahorrar Flash
  char buf[4];
  strcpy(mensaje, "#C,");
  itoa(TIPO_SENSOR_1, buf, 10);
  strcat(mensaje, buf);
  strcat(mensaje, ",");
  itoa(TIPO_SENSOR_2, buf, 10);
  strcat(mensaje, buf);
  strcat(mensaje, "*");
  digitalWrite(PIN_LED, LOW);
  tiempoNRFAnterior = millis();
}

void loop() {
  // 1. Lectura de sensores (puerta en cada ciclo para debounce exacto; analógicos cada 500 ms)
  puerta = lecturaPuerta();

  if (millis() - ultimaLecturaAnalogica >= TIEMPO_LEER_SENSORES) {
    ultimaLecturaAnalogica = millis();
    tempDecimas = leerTemperaturaTermistor();
    porcentajeBateria = lecturaBaterias();
  }


  // 2. Pulsador de emparejamiento
  if (!flagConectado) {
    if (!flagSW) {
      if (digitalRead(SW_PIN) == PRESIONADO) {
        flagSW = 1;
      }
    } else {
      if (digitalRead(SW_PIN) == NO_PRESIONADO) {
        tiempoConeccionAnterior = millis();
        primerIntentoConectar = true;
        coneccion = 1;
        flagSW = 0;
      }
    }
  }
  // 3. Proceso de emparejamiento con la central
  if (coneccion) {
    flagConectado = 1;
    tiempoConeccion = millis();
    digitalWrite(PIN_LED, HIGH);

    if (tiempoConeccion - tiempoConeccionAnterior <= TIMEOUT_CONECTAR) {
      if (conectar(mensaje)) {
        coneccion = 0;
        digitalWrite(PIN_LED, LOW);
        ultimoEnvioDatos = millis();
      }
    } else {
      coneccion = 0;
      digitalWrite(PIN_LED, LOW);
      flagConectado = 0;
    }
  } else if (flagConectado) {
    // 3b. Escucha continua fuera del emparejamiento para vaciar la FIFO y detectar ACKs
    if (radio.available()) {
      memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
      radio.read(mensajeRecibidoNRF, 32);
      mensajeRecibidoNRF[32] = '\0';

      char *inicio = mensajeRecibidoNRF + strspn(mensajeRecibidoNRF, " \t\r\n");
      size_t len = strlen(inicio);
      // Quita espacios/saltos al final sin bucles
      if (len > 0 && (inicio[len - 1] == '\n' || inicio[len - 1] == '\r' || inicio[len - 1] == ' ')) {
        inicio[strcspn(inicio, " \t\r\n")] = '\0';
        len = strlen(inicio);
      }

      bool terminaEnRecibido = (len >= 9 && strcmp(inicio + len - 9, "recibido*") == 0);
      if (terminaEnRecibido || strcmp(inicio, "OK") == 0) {
        respuestaNRF = 1;
      }
    }
  }
  // 4. Encolar datos periódicos cuando ya está conectado
  enviarDatosCentral();

  // 5. Despacho de cola NRF no bloqueante
  if (indiceMensajesNRF >= 1) {
    tiempoNRF = millis();
    if (respuestaNRF == 1) {
      // Elimina inmediatamente al recibir confirmación de la central
      if (eliminarPrimero(mensajesNRF, TAM_NRF)) {
        indiceMensajesNRF--;
        respuestaNRF = 0;
        cicloNRF = 0;
        tiempoNRFAnterior = tiempoNRF;
      }
    } else if (tiempoNRF - tiempoNRFAnterior >= TIEMPO_ENVIAR_NRF) {
      if (cicloNRF < 5) {
        mandarNRF(mensajesNRF[0]);
        cicloNRF++;
        tiempoNRFAnterior = tiempoNRF;
      } else {
        if (eliminarPrimero(mensajesNRF, TAM_NRF)) {
          indiceMensajesNRF--;
          respuestaNRF = 0;
          cicloNRF = 0;
          tiempoNRFAnterior = tiempoNRF;
        }
      }
    }
  }
}