// Código de prueba con función de termoresistencia pasado a temperatura
// en °C en base a cálculos anteriores.
// Utilizar función leerTemperaturaTermistor()

#include <Wire.h>
#include <DHT.h>

#define PIN_TERMISTOR 32
#define DHTPIN 33
#define DHTTYPE DHT22  // Cambiar a DHT22 si usas ese modelo

float tempTermistor;
float temperatura;
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  dht.begin();
}

void loop() {

  float tempTermistor = leerTemperaturaTermistor();
  float temperatura = dht.readTemperature();


  Serial.println("DHT22:       Termistor:");
  Serial.print(temperatura);
  Serial.print("   ");
  Serial.println(tempTermistor);
  Serial.println("");
  delay(500);
}

// función termistor
float leerTemperaturaTermistor() {

  int adc = analogRead(PIN_TERMISTOR);

  // Función cuadrática obtenida en base a los valores calculados del termisor, comparados con la temperatura del DHT11
  float temperatura = 0.0000215256 * adc * adc - 0.129388 * adc + 197.010;  
  
  float tempAjustada = temperatura - 4; // la temperatura obtenida tenía un offset de 4°C

  return tempAjustada;
}