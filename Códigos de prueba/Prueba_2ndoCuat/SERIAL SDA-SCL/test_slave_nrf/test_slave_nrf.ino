#include <SPI.h>
#include <RF24.h>

#define CE_PIN   4
#define CSN_PIN  5

RF24 radio(CE_PIN, CSN_PIN);
const byte direccion[6] = "00001";

void setup() {
  Serial.begin(115200);
  
  if (!radio.begin()) {
    Serial.println("Error: El módulo NRF24L01 no responde.");
    while (1);
  }
  
  radio.openReadingPipe(0, direccion); // Abre el canal cero para lectura
  radio.setPALevel(RF24_PA_LOW);       // Ajusta según la distancia de prueba
  radio.startListening();              // Configura el módulo como Receptor
}

void loop() {
  if (radio.available()) {
    char texto[32] = ""; // Buffer para almacenar el mensaje (máx. 32 bytes)
    
    radio.read(&texto, sizeof(texto));
    
    Serial.print("Mensaje recibido: ");
    Serial.println(texto);
  }
}

