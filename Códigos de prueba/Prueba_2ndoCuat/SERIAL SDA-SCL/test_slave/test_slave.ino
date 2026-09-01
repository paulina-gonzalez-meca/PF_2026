#include <Wire.h>

#define SDA 18
#define SCL 19
#define ADDRESS 0x08

void setup() {
  Serial.begin(115200);

  // CRUCIAL EN ESP32: Registrar el evento ANTES de inicializar el Wire
  Wire.onReceive(receiveEvent); 

  // EN EL ESCLAVO EL ORDEN ES: Wire.begin(SDA, SCL, DIRECCION);
  // Si pones la dirección primero, el bus se congela con Error 4.
  Wire.begin(SDA, SCL, ADDRESS); 

  Serial.println("SLAVE BOARD");
}

void loop() {
  delay(10); // Deja el loop libre para que trabaje la interrupción de I2C
}

// Función que se ejecuta automáticamente al recibir datos
void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

