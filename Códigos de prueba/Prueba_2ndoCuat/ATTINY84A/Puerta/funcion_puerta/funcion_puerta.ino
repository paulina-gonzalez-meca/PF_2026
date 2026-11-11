#define PIN_PUERTA 4  // después cambiar por el pin final, simular con pulsador
#define ABIERTO 1
#define CERRADO 0
#define DEBOUNCE 20

bool puerta;  // en donde se guardará el valor del pin puerta.

bool estadoPuerta = CERRADO;
bool ultimaLecturaPuerta;
unsigned long ultimoTiempoDebounce = 0;

void setup() {
  pinMode(PIN_PUERTA, INPUT);
  Serial.begin(115200);
}

void loop() {
  bool puerta = lecturaPuerta();
  Serial.println(puerta);
}

bool lecturaPuerta() {
  bool lecturaPinPuerta = digitalRead(PIN_PUERTA);

  if (lecturaPinPuerta != ultimaLecturaPuerta) {
    ultimoTiempoDebounce = millis();
  }

  if ((millis() - ultimoTiempoDebounce) >= DEBOUNCE) {

    if (lecturaPinPuerta != estadoPuerta) {
      lecturaPinPuerta = estadoPuerta;

      return estadoPuerta;
    }
  }

  ultimaLecturaPuerta = lecturaPinPuerta;
}
