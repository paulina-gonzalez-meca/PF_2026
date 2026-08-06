float lecturaDHT(){ 
  Serial.println("lecturaDHT");
  switchLecturas += 1;
  return 0;
  /*
  if(tiempoS2 == 2000){
    float t = dht2.readTemperature();
    if(t > umbralMaxS2 || t < umbralMinS2){
      if(flagSensor2 == 0){
        emergencia = 1;
        flagSensor2 = 1;
      }
    }
    else{
      if(flagSensor2 == 1){
        flagSensor2 = 0;
        flagNormalidad = 1;
      }
    }
    tiempoS2 = 0;
    switchLecturas += 1;
    return t;
    Serial.println("LECTURA_TEMP");
  }
  */
}