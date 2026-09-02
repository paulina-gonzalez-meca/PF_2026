#include <SPI.h>
#include <RF24.h>

// 1. Pines personalizados SPI
#define CUSTOM_SCK   18
#define CUSTOM_MISO  19
#define CUSTOM_MOSI  21

// 2. Pines de control del NRF24L01
#define CE_PIN       25
#define CSN_PIN      26

// 3. Instancia del bus SPI y del Radio NRF24
SPIClass customSPI(2); // Se puede cambiar por 2 (HSPI) o 3 (VSPI) 
RF24 radio(CE_PIN, CSN_PIN);

// Dirección de tubería de 5 bytes
const byte direccion[6] = "00001";

void setup() {
  Serial.begin(115200);
  
  // Inicializa bus SPI (Maestro)
  customSPI.begin(CUSTOM_SCK, CUSTOM_MISO, CUSTOM_MOSI, CSN_PIN);
  
  // Inicializa el módulo radio NRF24 (Esclavo SPI)
  if (!radio.begin(&customSPI)) { 
    Serial.println("Error: El módulo NRF24L01 no responde.");
    while (1); 
  }
  
  radio.setPALevel(RF24_PA_LOW);
  
  // Configura la misma dirección para enviar y recibir
  radio.openWritingPipe(direccion);
  radio.openReadingPipe(1, direccion);
}

void loop() {
  // --- FASE 1: EMISOR (TX) ---
  radio.stopListening(); // Modos TX
  
  char textoEnviado[] = "verdura";
  bool reporte = radio.write(&textoEnviado, sizeof(textoEnviado));
  
  if (reporte) {
    Serial.print("[TX] Mensaje enviado correctamente: ");
    Serial.println(textoEnviado);
  } else {
    Serial.println("[TX] Fallo al enviar el mensaje.");
  }

  // --- FASE 2: RECEPTOR (RX) ---
  radio.startListening(); // Cambia a modo RX
  
  // Espera un breve tiempo para escuchar confirmación o mensajes devueltos
  unsigned long tiempoInicio = millis();
  char textoRecibido[32] = {0};

  while (millis() - tiempoInicio < 200) { 
    if (radio.available()) {
      radio.read(&textoRecibido, sizeof(textoRecibido));
      Serial.print("[RX] Mensaje recibido: ");
      Serial.println(textoRecibido);
      break;
    }
  }

  delay(2000); // Pausa de 2 segundos antes del siguiente ciclo
}