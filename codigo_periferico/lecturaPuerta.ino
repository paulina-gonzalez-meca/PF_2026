bool lecturaPuerta(int pinPuerta, bool flagPuerta, int timerPuerta){
  if(flagPuerta == 0){
    if(digitalRead(pinPuerta) == 1 && timerPuerta >= TIEMPO_PUERTA){
      emergencia = 1;
      flagSensor1 = 0;
    }
    if(digitalRead(pinPuerta) == 0){
      tiempoS1 = 0;
    }
  }
  else{
    if(digitalRead(pinPuerta) == 0){
      flagSensor1 = 1;
      flagNormalidad = 1;
    }
  }
  switchLecturas += 1;
  Serial.println("LECTURA_PUERTA");
  return digitalRead(pinPuerta);
}