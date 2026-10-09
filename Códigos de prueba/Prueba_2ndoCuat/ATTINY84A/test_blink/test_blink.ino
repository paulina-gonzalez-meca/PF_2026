
// Pinout attiny84a
#define PIN_PULSADOR PIN_PB0
#define PIN_LED PIN_PB1
#define PIN_IRQ PIN_PB2
#define PIN_CSN PIN_PA7
#define PIN_MOSI PIN_PA6
#define PIN_CE PIN_PA3
#define PIN_SCK PIN_PA4
#define PIN_MISO PIN_PA5


  

// PIN_IRQ --> reset

void setup() {

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
}

void loop() {

  digitalWrite(PIN_LED, HIGH);
  delay(1000);
  digitalWrite(PIN_LED, LOW);
  delay(1000);
}
