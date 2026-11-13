#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 10000
#define PIN_CE 4
#define PIN_CSN 5
// #define PIN_MISO 16
// #define PIN_MOSI 2
// #define PIN_IRQ 4
// #define PIN_SCK 17

volatile int tiempoConectar = 0;
int tipoSensor1 = 1;
int tipoSensor2 = 0;
char mensaje[8];
char mensajeRecibidoNRF[32];
const uint64_t direccionA = 0xF0F0F0F0E0LL;
const uint64_t direccionB = 0xF0F0F0F0E1LL;
char nombre[4];

hw_timer_t *timer = NULL;  // timer
void IRAM_ATTR onTimer();  // function interrupts every 1ms

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  Serial.begin(9600);
  // SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);
   Serial.println("SPI");

  radio.begin();
  radio.setChannel(108);
  radio.openReadingPipe(1, direccionA);
    radio.openWritingPipe(direccionB);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  Serial.println("inicializado");

  timer = timerBegin(1000000);  // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);  // tick cada 1 ms

  snprintf(mensaje, sizeof(mensaje), "#C,%d,%d*", tipoSensor1, tipoSensor2);
}

void loop() {
  if(tiempoConectar <= TIMEOUT_CONECTAR){
    if(conectar(mensaje)){
      Serial.println("conectado");
      Serial.print(nombre[0]);
      Serial.print(nombre[1]);
      Serial.println(nombre[2]);
      tiempoConectar = 0;
    }
  }
  else{
    Serial.println("timeout");
    tiempoConectar = 0;
  }
}
bool conectar(char mensajeConectar[]){
  if(tiempoConectar%TIEMPO_CONECTAR == 0){
    Serial.println(String(mensajeConectar));
    radio.stopListening();
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    radio.startListening();
  }
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    Serial.println(mensajeCentralConectar);
    mensajeCentralConectar.trim();

    nombre[0] = 'P';
    if(mensajeCentralConectar.startsWith("#C,P") && mensajeCentralConectar.endsWith("*")){
      if(isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1))){
        nombre[1] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1);
        if(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2) != '*'){
          if(isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2))){
            nombre[2] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2);
          }
        }
        radio.stopListening();
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