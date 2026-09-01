#include <SPI.h>
#include <RF24.h>

// Definición de pines CE y CSN asignados en el ESP32
#define CE_PIN   4
#define CSN_PIN  5

RF24 radio(CE_PIN, CSN_PIN);

// Dirección del canal de comunicación (Debe ser idéntica en TX y RX)
const byte direccion[6] = "00001";

void setup() {
  Serial.begin(115200);
  
  if (!radio.begin()) {
    Serial.println("Error: El módulo NRF24L01 no responde. Revisa las conexiones.");
    while (1); 
  }
  
  radio.openWritingPipe(direccion); // Abre el canal para escribir datos
  
  // Nivel de potencia: Para pruebas cercanas usa RF24_PA_LOW. 
  // Para máxima distancia usa RF24_PA_MAX (requiere alimentación externa estable).
  radio.setPALevel(RF24_PA_LOW); 
  
  radio.stopListening(); // Configura el módulo como Transmisor
}

void loop() {
  const char texto[] = "Hola Mundo desde ESP32";
  
  Serial.print("Enviando datos: ");
  Serial.println(texto);
  
  // Transmisión del paquete
  bool exito = radio.write(&texto, sizeof(texto));
  
  if (exito) {
    Serial.println("Mensaje enviado con éxito.");
  } else {
    Serial.println("Fallo en el envío. Verifica ruido o alimentación.");
  }
  
  delay(2000); // Envía cada 2 segundos
}
