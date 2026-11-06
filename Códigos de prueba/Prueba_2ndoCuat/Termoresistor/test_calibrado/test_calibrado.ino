// Código de lectura de BMP280 para comparar su valor con el de la resistencia del termoresistor
// Falta adaptar los coeficientes A,B y C con los valores correspondientes.


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)


// Definición de pines y constantes
const int ADC_PIN = 16;        // Pin GPIO donde conectas el centro del divisor
const float R_FIJA = 10000.0;  // Valor de la resistencia fija en Ohms (10k)

// Coeficientes Steinhart-Hart estándar para un termistor NTC 10K común B=3950
const float A = 1.009249522e-03;
const float B = 2.378405444e-04;
const float C = 2.019202697e-07;
Adafruit_BMP280 bmp;  // I2C


void setup() {
  Serial.begin(115200);
  // Configuración del ADC (12 bits de resolución: 0 a 4095)

  while (!Serial) delay(100);  // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;

  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  // 1. Promediar lecturas para estabilizar el ruido del ADC
  long sumaADC = 0;
  const int muestras = 20;
  for (int i = 0; i < muestras; i++) {
    sumaADC += analogRead(ADC_PIN);
    delay(10);
  }
  float promedioADC = (float)sumaADC / muestras;

  // Evitar divisiones por cero si el cable se suelta
  if (promedioADC >= 4095) promedioADC = 4094;
  if (promedioADC <= 0) promedioADC = 1;

  // 2. Calcular la resistencia actual del termistor NTC
  // Fórmula derivada del divisor de tensión utilizando el ADC de 12 bits (4095)
  float R_ntc = R_FIJA * ((4095.0 / promedioADC) - 1.0);

  // 3. Aplicar la ecuación de Steinhart-Hart
  float logR = log(R_ntc);
  float TempK = 1.0 / (A + (B * logR) + (C * logR * logR * logR));  // Temperatura en Kelvin
  float TempC = TempK - 273.15;                                     // Conversión a Celsius

  // Mostrar resultados
  //Serial.print("Resistencia NTC: ");
  //Serial.print(R_ntc);
  // Temp del calibrado
  //Serial.print(" Ohm | Temperatura: ");
 // Serial.print(TempC);
  //Serial.println(" °C");
  
  // Temp BMP
  Serial.print(F("Temperatura = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.println();

  delay(2000);  // Esperar 1 segundo entre lecturas
}