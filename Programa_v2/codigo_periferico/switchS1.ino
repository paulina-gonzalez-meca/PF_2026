void switchS1(){
  delay(1000);
  switch(periferico.tipoSensor1){
    case 0:
      switchLecturas += 1;
    break;
    case 1:
      lecturaS1 = lecturaPuerta(PIN_S1, flagSensor1, tiempoS1);
    break;
    case 2:
      lecturaS1 = lecturaDHT();
    break;
  }
}