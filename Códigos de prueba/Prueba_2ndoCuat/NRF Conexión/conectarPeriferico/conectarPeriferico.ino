#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 10000
#define PIN_CE PIN_PA3
#define PIN_CSN PIN_PA7
#define PIN_LED PIN_PB1


volatile int tiempoConectar = 0;
int tipoSensor1 = 1;
int tipoSensor2 = 0;
char mensaje[8];
char mensajeRecibidoNRF[32];
uint64_t direccionConectar = 0xF0F0F0F0E0LL;
char nombre[4];

hw_timer_t *timer = NULL;  // timer
void IRAM_ATTR onTimer();  // function interrupts every 1ms

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  //Serial.begin(9600);
  //SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_LED, HIGH);
  radio.begin();
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  digitalWrite(PIN_LED, LOW);

  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms

  snprintf(mensaje, sizeof(mensaje), "C,%d,%d*", tipoSensor1, tipoSensor2);
}

void loop() {
  if (tiempoConectar <= TIMEOUT_CONECTAR) {
    if (conectar(mensaje, direccionConectar)) {
      /*Serial.println("conectado");
      Serial.print(nombre[0]);
      Serial.print(nombre[1]);
      Serial.println(nombre[2]);
      */
      tiempoConectar = 0;
    }
  } else {
    //Serial.println("timeout");
    tiempoConectar = 0;
  }
}
bool conectar(char mensajeConectar[], uint64_t direccion) {
  if (tiempoConectar % TIEMPO_CONECTAR == 0) {
    radio.stopListening();
    radio.openWritingPipe(direccion);
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    radio.startListening();
  }
  if (radio.available() > 0) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    //Serial.println(mensajeCentralConectar);
    mensajeCentralConectar.trim();

    nombre[0] = 'P';
    if (mensajeCentralConectar.startsWith("#C,P") && mensajeCentralConectar.endsWith("*")) {
      if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1))) {
        nombre[1] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1);
        if (mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2) != '*') {
          if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2))) {
            nombre[2] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2);
          }
        }
        radio.stopListening();
        radio.openWritingPipe(direccion);
        radio.write("OK", 3);
        radio.startListening();
        return true;
      }
    }
  }
  return false;
}

void IRAM_ATTR onTimer() {
  tiempoConectar += 1;
}