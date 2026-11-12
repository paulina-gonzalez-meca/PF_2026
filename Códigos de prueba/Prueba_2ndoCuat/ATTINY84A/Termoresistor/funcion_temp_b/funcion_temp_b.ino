#include <Wire.h>
#include <DHT.h>

#define PIN_TERMISTOR 32 // PIN_PA1
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

  float tempTermistor = leerTemperatura();
  float temperatura = dht.readTemperature();


  Serial.println("DHT22:       Termistor:");
  Serial.print(temperatura);
  Serial.print("   ");
  Serial.println(tempTermistor);
  Serial.println("");
  delay(500);
}
float leerTemperatura() {
  int lecturaADC = analogRead(PIN_TERMISTOR); // lectura attiny
  float lecturaADC12Bit = lecturaADC*(4095 / 1023); // la medición de temperatura se hizo con 12bits de ADC.
  // El attiny84a lee hasta 10bits de ADC
  // Casos de error
  if (lecturaADC12Bit <= 0) return -999.0;
  if (lecturaADC12Bit >= 4095) return 999.0; 

  // Valores calculados en base de los datos obtenidos.
  // T(adc) = a * adc^2 + b * adc + c

  const float a =  0.0000854f;
  const float b = -0.3802f;
  const float c =  424.1f;

  // Se forma una función cuadrática
  // que imita aproximadamente el comportamiento del termistor.
  float tempC = a * (lecturaADC12Bit * lecturaADC12Bit) + b *lecturaADC12Bit + c;
  float tempCorregida = tempC + 10; // offset de 10°c
  return tempCorregida;
}