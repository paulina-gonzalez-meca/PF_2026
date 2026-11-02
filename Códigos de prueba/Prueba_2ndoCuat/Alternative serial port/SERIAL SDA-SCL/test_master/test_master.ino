#include <Wire.h>

// Definir los pines y dirección del esclavo
// NO USAR PINES 16 Y 17 PARA SLAVE/MASTER
#define SDA 25
#define SCL 26
#define ADDRESS 0x08

void setup() {
  Serial.begin(115200);

  // Initialize I2C with custom pins

  // Inicializa I2C habilitando explícitamente los pull-ups internos del ESP32
  // El tercer parámetro (true) activa las resistencias internas
  Wire.begin(SDA, SCL, 100000); 
   Wire.setClock(100000); 

  Serial.println("MASTER BOARD");
}

void loop() {
  Wire.beginTransmission(ADDRESS);      // Inicio de transmisión

  Wire.print("verdura");
  
  // El 'true' fuerza al ESP32 a liberar las líneas I2C pase lo que pase
  byte error = Wire.endTransmission(true); 

  if (error == 0) {
    Serial.println("¡Enviado!");
  } else {
    Serial.print("Error: ");
    Serial.println(error);
  }

  delay(1000);
}
