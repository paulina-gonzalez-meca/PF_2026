
#include <SPI.h>
#include <RF24.h>
#include <string.h>

// --------------------------------------
// PINES ESP32
// --------------------------------------
#define PIN_CE 2
#define PIN_CSN 4

RF24 radio(PIN_CE, PIN_CSN, 1000000);

// --------------------------------------
// CONFIGURACION COMPARTIDA
// --------------------------------------
const uint64_t address = 0xF0F0F0F0E1LL;
const uint8_t CANAL_RF = 76;

char receivedMessage[32];

void setup() {
  Serial.begin(115200);

  delay(500);

  Serial.println();
  Serial.println("Iniciando receptor NRF24L01...");

  if (!radio.begin()) {
    Serial.println("ERROR: NRF24L01 no detectado.");
    Serial.println("Revisar SPI, CE, CSN y alimentacion.");
    return;
  }

  Serial.println("NRF24L01 inicializado correctamente.");

  radio.setChannel(CANAL_RF);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);

  radio.setAutoAck(false);
  radio.disableDynamicPayloads();
  radio.setPayloadSize(32);

  radio.openReadingPipe(1, address);

  radio.startListening();

  Serial.println("Receptor escuchando...");
  Serial.println("Esperando mensajes del ATtiny84A.");
}

void loop() {
  if (radio.available()) {
    memset(receivedMessage, 0, sizeof(receivedMessage));

    radio.read(receivedMessage, sizeof(receivedMessage));

    // Asegurar terminacion de cadena.
    receivedMessage[31] = '\0';
    
    Serial.print("Mensaje recibido: ");
    Serial.println(receivedMessage);
  }
}
