#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  delay(1000);
  Serial.println("\n==============================================");
  Serial.println("  Pasarela Directa PC <-> SIM800L Lista       ");
  Serial.println("==============================================");
  Serial.println("Configura el Monitor Serie en 'Ambos NL y CR' (Both NL & CR).");
}

void loop() {
  // Lee lo que escribes en la PC y lo manda al SIM800L
  if (Serial.available()) {
    char c = Serial.read();
    Serial2.write(c);
  }

  // Lee la respuesta del SIM800L y la muestra en la PC
  if (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);
  }
}