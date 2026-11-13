int16_t leerTemperaturaTermistor() {
  int32_t adc = analogRead(PIN_TERMISTOR);
  // Equivale a (0.0000215256*adc^2 - 0.129388*adc + 197.010 - 4.0) * 10
  int32_t tempDecimas = (215L * adc * adc - 1293880L * adc + 1930100000L) / 1000000L;
  return (int16_t)tempDecimas;
}

bool lecturaPuerta() {
  bool lecturaPinPuerta = digitalRead(PIN_PUERTA);

  if (lecturaPinPuerta != ultimaLecturaPuerta) {
    ultimoTiempoDebounce = millis();
  }

  if ((millis() - ultimoTiempoDebounce) >= DEBOUNCE) {
    if (lecturaPinPuerta != estadoPuerta) {
      estadoPuerta = lecturaPinPuerta;
    }
  }

  ultimaLecturaPuerta = lecturaPinPuerta;
  return estadoPuerta;
}

uint8_t lecturaBaterias() {
  return (uint8_t)((analogRead(PIN_ENERGIA) * 100UL) / 1023UL);
}