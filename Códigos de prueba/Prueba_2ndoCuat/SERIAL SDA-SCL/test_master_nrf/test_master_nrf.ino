#include <SPI.h>
#include <RF24.h>

// 1. Define los pines personalizados que quieras usar para SPI
#define CUSTOM_SCK   14
#define CUSTOM_MISO  12
#define CUSTOM_MOSI  13

// 2. Define tus pines CE y CSN
#define CE_PIN       4
#define CSN_PIN      5

// 3. Crea una instancia global para el bus SPI personalizado
SPIClass customSPI(VSPI_HOST); 

// 4. Instancia el radio pasando los pines CE, CSN y el bus SPI personalizado
RF24 radio(CE_PIN, CSN_PIN);

const byte direccion = "00001";

void setup() {
  Serial.begin(115200);
  
  // 5. Inicializa el bus SPI con tus pines modificados: begin(SCK, MISO, MOSI, SS)
  // Pasamos 0 o CSN_PIN en el último parámetro, pero la librería RF24 maneja el CSN internamente.
  customSPI.begin(CUSTOM_SCK, CUSTOM_MISO, CUSTOM_MOSI, CSN_PIN);
  
  // 6. Inicializa el módulo pasando tu objeto SPI personalizado a radio.begin()
  if (!radio.begin(&customSPI)) { 
    Serial.println("Error: El módulo NRF24L01 no responde con los nuevos pines.");
    while (1); 
  }
  
  radio.openWritingPipe(direccion);
  radio.setPALevel(RF24_PA_LOW); 
  radio.stopListening();
}

void loop() {
  const char texto[] = "verdura";
  radio.write(&texto, sizeof(texto));
  Serial.println("Mensaje enviado.");
  delay(2000);
}

