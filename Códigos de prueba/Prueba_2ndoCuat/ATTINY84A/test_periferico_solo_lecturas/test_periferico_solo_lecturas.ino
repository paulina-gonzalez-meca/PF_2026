#include <RF24.h>
#include <stdint.h>

#define TIEMPO_CONECTAR 2000
#define TIMEOUT_CONECTAR 10000
// #P1,BAT,VALORSENSOR,VALORSENSOR2*

// pines para probar en esp32:
// CE --> 4
// CSN --> 5
// LED --> 2
// PULSADOR --> 16

#define PIN_PULSADOR PIN_PB0
#define PIN_LED PIN_PB1

#define PIN_CSN PIN_PA7
#define PIN_CE PIN_PA3

#define PIN_TERMISTOR PIN_PA1
#define PIN_PUERTA PIN_PA2
#define PIN_ENERGIA PIN_PA0

#define ABIERTO 1
#define CERRADO 0
#define DEBOUNCE 20

// radio configuration
RF24 radio(PIN_CE, PIN_CSN);
const uint64_t direccionConectar = 0xF0F0F0F0E0LL;
// mensaje buffer
char mensaje[8];
char mensajeRecibidoNRF[32];
char trama[32];

unsigned long tiempoActual = 0;
unsigned long tiempoAnterior = 0;

// datos que se le enviarán al central
int tipoSensor1 = 1;
int tipoSensor2 = 0;
char nombre[4];

float tempTermistor;
float porcentajeBateria;
bool puerta;  // en donde se guardará el valor del pin puerta.


bool estadoPuerta = CERRADO;
bool ultimaLecturaPuerta;
unsigned long ultimoTiempoDebounce = 0;



void setup() {
  pinMode(PIN_PUERTA, INPUT_PULLUP);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  radio.begin();
  radio.openWritingPipe(direccionConectar);
  radio.openReadingPipe(1, direccionConectar);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  digitalWrite(PIN_LED, LOW);

   tiempoAnterior = millis();
  snprintf(mensaje, sizeof(mensaje), "#C,%d,%d*", tipoSensor1, tipoSensor2);
}

void loop() {

  configuracion();
  tempTermistor = leerTemperaturaTermistor();
  puerta = lecturaPuerta();
  porcentajeBateria = lecturaBaterias();


}

float leerTemperaturaTermistor() {

  int adc = analogRead(PIN_TERMISTOR);

  // Función cuadrática obtenida en base a los valores calculados del termisor, comparados con la temperatura del DHT11
  float temperatura = 0.0000215256 * adc * adc - 0.129388 * adc + 197.010;

  float tempAjustada = temperatura - 4;  // la temperatura obtenida tenía un offset de 4°C

  return tempAjustada;
}
bool lecturaPuerta() {
  bool lecturaPinPuerta = digitalRead(PIN_PUERTA);

  if (lecturaPinPuerta != ultimaLecturaPuerta) {
    ultimoTiempoDebounce = millis();
  }

  if ((millis() - ultimoTiempoDebounce) >= DEBOUNCE) {

    if (lecturaPinPuerta != estadoPuerta) {
      estadoPuerta = lecturaPinPuerta;
    }
  }

  ultimaLecturaPuerta = lecturaPinPuerta;
  return estadoPuerta;
}
float lecturaBaterias() {
  char nivelBateria[32];
  int bateriaAnalog = analogRead(PIN_ENERGIA);
  float bateriaPin = bateriaAnalog * (3 / 1023.0);
  float bateria = (bateriaPin / 0.54) * 3;
  // lo pasé a porcentaje
  float bateriaPorcentaje = (bateria/3) * 100;
  return bateriaPorcentaje;
  //para ponerlo en el char
  //dtostrf(bateria, 4, 2, nivelBateria);  // float, minimo, decimales, char en donde se guarda
}

void configuracion() {
 tiempoActual = millis();
  if (tiempoActual - tiempoAnterior >= TIMEOUT_CONECTAR) {
  digitalWrite(PIN_LED, HIGH);
    if (conectar(mensaje, direccionConectar)) {
      Serial.println("conectado");
      Serial.print(nombre[0]);
      Serial.print(nombre[1]);
      Serial.println(nombre[2]);
      tiempoAnterior = tiempoActual;
    }
  } else {
    digitalWrite(PIN_LED, LOW);
      tiempoAnterior = tiempoActual;
    Serial.println("timeout");
  }

}

bool conectar(char mensajeConectar[], uint64_t direccion) {
  if ((tiempoActual - tiempoAnterior) % TIEMPO_CONECTAR == 0) {
    radio.stopListening();
    radio.openWritingPipe(direccion);
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    Serial.print(mensajeConectar);
    radio.startListening();
  }
  if (radio.available() > 0) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    Serial.println(mensajeCentralConectar);
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
