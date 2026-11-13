#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define TIMEOUT_CONECTAR 10000
#define TIEMPO_CONECTAR_PER 1000
#define PIN_CE 5
#define PIN_CSN 15
#define PIN_MISO 16
#define PIN_MOSI 2
#define PIN_IRQ 4
#define PIN_SCK 17

class per {
public:
  char nombreInterno[4] = { "" };
  char nombreExterno[5] = { "-" };
  char nombreSensor1[5] = { "1" };
  char nombreSensor2[5] = { "2" };
  int tipoSensor1;
  int tipoSensor2;
  int lecturaSensor1;
  int lecturaSensor2;
  int indiceNombrePer = 0;
  int indiceNombreS1 = 0;
  int indiceNombreS2 = 0;
  bool alertaS2[20] = { 0 };
  bool alertaS1[20] = { 0 };
  per() {
    nombreInterno;
    nombreExterno;
    nombreSensor1;
    nombreSensor2;
    tipoSensor1 = 0;
    tipoSensor2 = 0;
    alertaS1;
    alertaS2;
    indiceNombrePer;
    indiceNombreS1;
    indiceNombreS2;
  }
};

typedef enum {  // state machine
  ESPERA,
  ENVIO_CONECTAR
} PASOS_CONECTAR_t;
PASOS_CONECTAR_t CONECTAR_CEN;

volatile unsigned int tiempoConectar = 0;
int tipoSensor1 = 0;
int tipoSensor2 = 0;
int indicePeriferico = 0;
uint64_t direccionConectar = 0xF0F0F0F0E0LL;

hw_timer_t *timer = NULL;  // timer
void IRAM_ATTR onTimer();  // function interrupts every 1ms

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  CONECTAR_CEN = ESPERA;
  Serial.begin(9600);

  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  radio.begin();
  radio.setChannel(76);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();
  Serial.println("NRF conectada");

  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms
}

void loop() {
  if (indicePeriferico < 20) {
    if (tiempoConectar <= TIMEOUT_CONECTAR) {
      if (conectar(indicePeriferico, direccionConectar, tiempoConectar)) {
        Serial.println("conectado");
        Serial.println(String(tipoSensor1));
        Serial.println(String(tipoSensor2));
        indicePeriferico += 1;
        tiempoConectar = 0;
      }
    } else {
      Serial.println("timeout");
      tiempoConectar = 0;
    }
  }
}
bool conectar(int nombreConectar, uint64_t direccion, volatile unsigned int &tiempoConectar) {
  static char mensajeConectar[8];
  static char mensajeRecibidoNRF[32];
  switch (CONECTAR_CEN) {
    case ESPERA:
      if (radio.available() > 0) {
        memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
        radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
        String mensajeCentralConectar = String(mensajeRecibidoNRF);
        mensajeCentralConectar.trim();
        Serial.println(mensajeCentralConectar);

        if (mensajeCentralConectar.startsWith("#C,") && mensajeCentralConectar.endsWith("*")) {
          if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 1))) {
            tipoSensor1 = String(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 1)).toInt();
            if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 3))) {
              tipoSensor2 = String(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 3)).toInt();
              memset(mensajeConectar, 0, sizeof(mensajeConectar));
              snprintf(mensajeConectar, sizeof(mensajeConectar), "#C,P%d*", (nombreConectar + 1));
              CONECTAR_CEN = ENVIO_CONECTAR;
            }
          }
        }
      }
      return false;
      break;
    case ENVIO_CONECTAR:
      if (tiempoConectar % TIEMPO_CONECTAR_PER == 0) {
        radio.stopListening();
        radio.openWritingPipe(direccion);
        radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
        Serial.println(String(mensajeConectar));
        radio.startListening();
      }
      if (radio.available() > 0) {
        memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
        radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
        String mensajeCentralConectar = String(mensajeRecibidoNRF);
        mensajeCentralConectar.trim();
        Serial.println(mensajeCentralConectar);

        if (mensajeCentralConectar == "OK") {
          CONECTAR_CEN = ESPERA;
          return true;
        }
      }
      return false;
      break;
  }
}

void IRAM_ATTR onTimer() {
  tiempoConectar += 1;
}