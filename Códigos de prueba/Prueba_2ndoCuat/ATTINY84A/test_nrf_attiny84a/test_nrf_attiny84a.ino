#include <SPI.h>
#include <RF24.h>
#include <stdint.h>
#include <string.h>

// --------------------------------------
// PINES ATtiny84A - ATtinyCore
// --------------------------------------
#define PIN_PULSADOR PIN_PB0
#define PIN_LED PIN_PB1
#define PIN_IRQ PIN_PB2

#define PIN_CSN PIN_PA7
#define PIN_CE PIN_PA3

// SPI por hardware:
// MOSI = PA6
// MISO = PA5
// SCK  = PA4

RF24 radio(PIN_CE, PIN_CSN);

// --------------------------------------
// CONFIGURACION DE RADIO
// --------------------------------------
const uint64_t address = 0xF0F0F0F0E1LL;

const uint8_t CANAL_RF = 76;

const uint32_t TIEMPO_REBOTE = 30;
const uint32_t TIEMPO_TIMEOUT_TX = 100;

// --------------------------------------
// ESTADOS
// --------------------------------------
enum EstadoRadio : uint8_t {
  RADIO_RX,
  RADIO_TX
};

EstadoRadio estadoRadio = RADIO_RX;

// --------------------------------------
// VARIABLES
// --------------------------------------
char mensajeTX[32] = {};
char mensajeRX[32] = {};

uint8_t contadorMensajes = 0;

bool botonAnterior = HIGH;
bool botonEstable = HIGH;

uint32_t tiempoCambioBoton = 0;
uint32_t tiempoInicioTX = 0;

bool radioInicializada = false;

// --------------------------------------
// INICIALIZACION
// --------------------------------------
void setup() {
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_IRQ, INPUT_PULLUP);

  digitalWrite(PIN_LED, LOW);

  radioInicializada = radio.begin();

  if (!radioInicializada) {
    digitalWrite(PIN_LED, HIGH);
    return;
  }

  radio.setChannel(76);
  radio.setPALevel(RF24_PA_MIN);
  radio.setAutoAck(false);
  radio.disableDynamicPayloads();
  radio.setPayloadSize(32);

  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);

  radio.startListening();
  /*if (radioInicializada) {
    radio.setChannel(CANAL_RF);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);

    // Para una prueba sencilla sin ACK automatico.
    radio.setAutoAck(false);

    radio.disableDynamicPayloads();
    radio.setPayloadSize(32);

    radio.openWritingPipe(address);
    radio.openReadingPipe(1, address);


    radio.startListening();
  } else {
    // Error de inicializacion: LED encendido.
    digitalWrite(PIN_LED, HIGH);
  } */
}

// --------------------------------------
// LOOP PRINCIPAL
// --------------------------------------
void loop() {
  if (!radioInicializada) {
    return;
  }

  gestionarPulsador();
  gestionarRadio();
}

// --------------------------------------
// DETECCION DE PULSACION
// --------------------------------------
void gestionarPulsador() {
  bool lectura = digitalRead(PIN_PULSADOR);

  if (lectura != botonAnterior) {
    botonAnterior = lectura;
    tiempoCambioBoton = millis();
  }

  if ((uint32_t)(millis() - tiempoCambioBoton) >= TIEMPO_REBOTE) {
    if (lectura != botonEstable) {
      botonEstable = lectura;

      // Detectar solo el flanco de pulsacion.
      if (botonEstable == LOW && estadoRadio == RADIO_RX) {
        prepararTransmision();
      }
    }
  }
}

// --------------------------------------
// PREPARAR Y COMENZAR TRANSMISION
// --------------------------------------
void prepararTransmision() {
  contadorMensajes++;

  memset(mensajeTX, 0, sizeof(mensajeTX));

  // Ejemplo: AT84:1, AT84:2...
  mensajeTX[0] = 'A';
  mensajeTX[1] = 'T';
  mensajeTX[2] = '8';
  mensajeTX[3] = '4';
  mensajeTX[4] = ':';

  uint8_t valor = contadorMensajes;
  uint8_t posicion = 5;

  // Convertir contador a texto sin sprintf.
  if (valor >= 100) {
    mensajeTX[posicion++] = '0' + valor / 100;
    valor %= 100;
  }

  if (valor >= 10 || posicion > 5) {
    mensajeTX[posicion++] = '0' + valor / 10;
    valor %= 10;
  }

  mensajeTX[posicion++] = '0' + valor;

  radio.stopListening();

  estadoRadio = RADIO_TX;
  tiempoInicioTX = millis();

  // Iniciar TX sin esperar su finalizacion.
  radio.startWrite(mensajeTX, sizeof(mensajeTX), false);
}

// --------------------------------------
// GESTION DE TRANSMISION Y RECEPCION
// --------------------------------------
void gestionarRadio() {
  if (estadoRadio == RADIO_TX) {
    bool tx_ok = false;
    bool tx_fail = false;
    bool tx_ds = false;

    // Consulta y limpieza de flags IRQ del NRF.
    radio.whatHappened(tx_ok, tx_fail, tx_ds);

    if (tx_ds || tx_ok || tx_fail || (uint32_t)(millis() - tiempoInicioTX) >= TIEMPO_TIMEOUT_TX) {

      radio.startListening();
      estadoRadio = RADIO_RX;
    }

    return;
  }

  // Leer como maximo un paquete por iteracion.
  if (radio.available()) {
    memset(mensajeRX, 0, sizeof(mensajeRX));
    radio.read(mensajeRX, sizeof(mensajeRX));

    procesarMensaje(mensajeRX);
  }
}

// --------------------------------------
// PROCESAR MENSAJE RECIBIDO
// --------------------------------------
void procesarMensaje(const char *mensaje) {
  // LED encendido al recibir un mensaje.
  // En este ejemplo queda encendido hasta el
  // siguiente cambio de estado de la aplicacion.
  digitalWrite(PIN_LED, HIGH);

  // Agregar aqui la logica de tu aplicacion.
  // Ejemplo: reconocer mensajes "ESP32:OK".
  if (strcmp(mensaje, "ESP32:OK") == 0) {
    digitalWrite(PIN_LED, LOW);
  }
}