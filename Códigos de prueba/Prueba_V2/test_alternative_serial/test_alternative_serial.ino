#include <Arduino.h>

// Definición de pines alternativos seguros para TX y RX
// EVITADOS: Strapping pins (0, 2, 5, 12, 15), Memoria Flash (6-11) y Solo Entrada (34, 35, 36, 37, 38 39)
#define RX_PIN 33  // Alternativas seguras: 18, 21, 22, 25, 26, 27, 32, 33
#define TX_PIN 32  // Alternativas seguras: 19, 21, 22, 25, 26, 27, 32, 33
#define UART_BAUD 9600

// Instancia de puerto serie por hardware (UART2)
HardwareSerial AlternativeSerial(2);

void setup() {
  // Puerto serie primario para monitoreo vía USB
  Serial.begin(115200);

  while (!Serial && millis() < 3000);

  Serial.println("\n=== PRUEBA DE UART PERSONALIZADO ESP32 ===");
  Serial.printf("Configurando RX_PIN = GPIO %d | TX_PIN = GPIO %d\n", RX_PIN, TX_PIN);

  // Inicialización con matriz de comunicación (Baud, Configuración, RX, TX)
  AlternativeSerial.begin(UART_BAUD, SERIAL_8N1, RX_PIN, TX_PIN);
  
  Serial.println("Puerto secundario listo. Para prueba en bucle (Loopback), puentee los pines RX y TX.");

    // Envío de datos de prueba por el pin TX definido
  String testMessage = "PING_ESP32";
  AlternativeSerial.println(testMessage);
  Serial.println("Enviado: " + testMessage);

  // Lectura de datos desde el pin RX definido
  delay(100); 
  if (AlternativeSerial.available()) {
    String response = AlternativeSerial.readStringUntil('\n');
    Serial.println("Recibido correctamente en RX: " + response);
  } else {
    Serial.println("Sin respuesta en RX (Verifique conexión con el dispositivo destino o puente Loopback)");
  }

  Serial.println("----------------------------------------");
  delay(3000);

}

void loop() {}