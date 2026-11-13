
#include <nRF24L01.h>
#include <RF24.h>

#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 10000
#define PIN_CE PIN_PA3
#define PIN_CSN PIN_PA7
#define PIN_LED PIN_PB1

unsigned long tiempoInicio = 0;
unsigned long ultimoEnvio = 0;


int tipoSensor1 = 1;
int tipoSensor2 = 0;
char mensaje[10];
char mensajeRecibidoNRF[32];
uint64_t direccionConectar = 0xF0F0F0F0E0LL;
char nombre[4];

RF24 radio(PIN_CE, PIN_CSN);

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW); // Sube a RF24_PA_MAX solo cuando tengas capacitor en VCC/GND
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(1, direccionConectar);
  radio.startListening();

  snprintf(mensaje, sizeof(mensaje), "#C,%d,%d*", tipoSensor1, tipoSensor2);

  digitalWrite(PIN_LED, LOW);
  tiempoInicio = millis();
  ultimoEnvio = tiempoInicio - TIEMPO_CONECTAR;
}

void loop() {

  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoInicio < TIMEOUT_CONECTAR) {
    digitalWrite(PIN_LED, HIGH);
    if (conectar(mensaje, direccionConectar)) {

      digitalWrite(PIN_LED, LOW);
    }
  } else {
    digitalWrite(PIN_LED, LOW);
    tiempoInicio = millis();
  }
}

bool conectar(char mensajeConectar[], uint64_t direccion) {
  unsigned long ahora = millis();

  if (ahora - ultimoEnvio >= TIEMPO_CONECTAR) {
    ultimoEnvio = ahora;
    radio.stopListening();
    radio.openWritingPipe(direccion);
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    radio.startListening();
  }

  if (radio.available()) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF) - 1);

    // Validación directa sin while: acepta "#C,P<1 dígito>*" o "#C,P<2 dígitos>*"
    if (strncmp(mensajeRecibidoNRF, "#C,P", 4) == 0 && isDigit(mensajeRecibidoNRF[4])) {
      bool formatoValido = false;
      memset(nombre, 0, sizeof(nombre));
      nombre[0] = 'P';
      nombre[1] = mensajeRecibidoNRF[4];

      if (mensajeRecibidoNRF[5] == '*') {
        formatoValido = true;
      } else if (isDigit(mensajeRecibidoNRF[5]) && mensajeRecibidoNRF[6] == '*') {
        nombre[2] = mensajeRecibidoNRF[5];
        formatoValido = true;
      }

      if (formatoValido) {
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