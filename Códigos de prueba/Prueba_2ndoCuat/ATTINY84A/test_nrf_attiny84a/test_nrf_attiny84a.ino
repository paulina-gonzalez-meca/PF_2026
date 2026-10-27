
#include <RF24.h>
#include <stdint.h>
#include <string.h>

// --------------------------------------
// PINES ATtiny84A - ATtinyCore
// --------------------------------------
#define PIN_PULSADOR PIN_PB0
#define PIN_LED      PIN_PB1

#define PIN_CSN      PIN_PA7
#define PIN_CE       PIN_PA3

// SPI por hardware del ATtiny84A:
// MOSI = PA6
// MISO = PA5
// SCK  = PA4

// SPI a 1 MHz para mejorar la compatibilidad
RF24 radio(PIN_CE, PIN_CSN, 1000000);

// --------------------------------------
// CONFIGURACION NRF24L01
// --------------------------------------
const uint64_t address = 0xF0F0F0F0E1LL;

const uint8_t CANAL_RF = 76;

const uint32_t TIEMPO_REBOTE = 30;
const uint32_t TIEMPO_TIMEOUT_TX = 100;
const uint32_t TIEMPO_REINTENTO = 1000;

// --------------------------------------
// ESTADOS
// --------------------------------------
enum EstadoRadio : uint8_t {
  RADIO_INICIALIZANDO,
  RADIO_RX,
  RADIO_TX
};

EstadoRadio estadoRadio = RADIO_INICIALIZANDO;

// --------------------------------------
// VARIABLES
// --------------------------------------
char mensajeTX[32] = {};

uint8_t contadorMensajes = 0;

bool botonAnterior = HIGH;
bool botonEstable = HIGH;

bool radioInicializada = false;
bool ledError = false;

uint32_t tiempoCambioBoton = 0;
uint32_t tiempoInicioTX = 0;
uint32_t tiempoUltimoIntento = 0;
uint32_t tiempoLed = 0;

// --------------------------------------
// INICIALIZAR RADIO
// --------------------------------------
bool inicializarRadio() {
  digitalWrite(PIN_CSN, HIGH);
  digitalWrite(PIN_CE, LOW);

  if (!radio.begin()) {
    return false;
  }

  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(32);

  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);

  radio.flush_tx();
  radio.flush_rx();

  radio.startListening();

  return true;
}

// --------------------------------------
// SETUP
// --------------------------------------
void setup() {
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_CE, OUTPUT);
  pinMode(PIN_CSN, OUTPUT);

  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_CE, LOW);
  digitalWrite(PIN_CSN, HIGH);

  // El primer intento se realizará en loop().
  tiempoUltimoIntento = millis() - TIEMPO_REINTENTO;
}

// --------------------------------------
// LOOP
// --------------------------------------
void loop() {
  uint32_t ahora = millis();

  // Reintentar inicialización si falla.
  if (!radioInicializada) {
    if ((uint32_t)(ahora - tiempoUltimoIntento)
        >= TIEMPO_REINTENTO) {

      tiempoUltimoIntento = ahora;

      radioInicializada = inicializarRadio();

      if (radioInicializada) {
        estadoRadio = RADIO_RX;
        digitalWrite(PIN_LED, HIGH);
      }
    }

    // LED intermitente = radio sin inicializar.
    if (!radioInicializada) {
      if ((uint32_t)(ahora - tiempoLed) >= 250) {
        tiempoLed = ahora;
        ledError = !ledError;
        digitalWrite(PIN_LED, ledError);
      }

      return;
    }
  }

  gestionarPulsador();
  gestionarRadio();
}

// --------------------------------------
// DETECCION DEL PULSADOR
// --------------------------------------
void gestionarPulsador() {
  bool lectura = digitalRead(PIN_PULSADOR);
  uint32_t ahora = millis();

  if (lectura != botonAnterior) {
    botonAnterior = lectura;
    tiempoCambioBoton = ahora;
  }

  if ((uint32_t)(ahora - tiempoCambioBoton)
      >= TIEMPO_REBOTE) {

    if (lectura != botonEstable) {
      botonEstable = lectura;

      // Una transmisión por cada pulsación nueva.
      if (botonEstable == LOW && estadoRadio == RADIO_RX) {
        prepararTransmision();
      }
    }
  }
}

// --------------------------------------
// PREPARAR TRANSMISION
// --------------------------------------
void prepararTransmision() {
  contadorMensajes++;

  memset(mensajeTX, 0, sizeof(mensajeTX));

  // Mensajes: AT84:1, AT84:2, AT84:3...
  mensajeTX[0] = 'A';
  mensajeTX[1] = 'T';
  mensajeTX[2] = '8';
  mensajeTX[3] = '4';
  mensajeTX[4] = ':';

  uint8_t valor = contadorMensajes;
  uint8_t posicion = 5;

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

  // Iniciar transmisión sin esperar.
  radio.startWrite(mensajeTX, sizeof(mensajeTX), false);
}

// --------------------------------------
// GESTION DE RADIO
// --------------------------------------
void gestionarRadio() {
  if (estadoRadio == RADIO_TX) {
    bool tx_ok = false;
    bool tx_fail = false;
    bool tx_ds = false;

    radio.whatHappened(tx_ok, tx_fail, tx_ds);

    if (tx_ds || tx_ok || tx_fail ||
        (uint32_t)(millis() - tiempoInicioTX)
          >= TIEMPO_TIMEOUT_TX) {

      radio.startListening();
      estadoRadio = RADIO_RX;
    }

    return;
  }

  if (radio.available()) {
    char mensajeRX[32] = {};

    radio.read(mensajeRX, sizeof(mensajeRX));

    // Indicador de recepción.
    digitalWrite(PIN_LED, HIGH);
  }
}
