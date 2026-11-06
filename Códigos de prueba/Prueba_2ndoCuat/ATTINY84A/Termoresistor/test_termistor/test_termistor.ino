
#include <Wire.h>
#include <DHT.h>

// para medir la temperatura a tiempo real
#define DHTPIN 33      // Pin digital donde está conectado el sensor
#define DHTTYPE DHT22  // Cambiar a DHT22 si usas ese modelo

#define PIN_TERMISTOR 32
#define R_FIJA 10000.0

int lecturaTermistorAnalog;
float lecturaTermistor;
float temperatura;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();
}

void loop() {
  lecturaTermistorAnalog = analogRead(PIN_TERMISTOR);
  float temperatura = dht.readTemperature();
  
  Serial.println("DHT22   Termistor");
  Serial.print(temperatura);
  Serial.print("   ");
  Serial.println(lecturaTermistorAnalog);
  Serial.println("");

  delay(500);
}
