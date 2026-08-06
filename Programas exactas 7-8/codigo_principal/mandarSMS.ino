void mandarSMS(String mensaje, String numero){
  switch(PSMS){
    case PASO1:
      sim800l.println("AT+CMGS=\"" + numero + "\"");
      tiempoDelay = 0;
      PSMS = PASO2;
    break;
    case PASO2:
      if(tiempoDelay >= 500){
        sim800l.print(mensaje);
        PSMS = PASO3;
      }
    break;
    case PASO3:
      if(tiempoDelay >= 1000){
        sim800l.write(0x1A);
        digitalWrite(PIN_LED1, HIGH);
        tiempoLed1 = 0;
        tiempoDelay = 0;
        timerSMS = 0;
        PSMS = PASO1;
      }
    break;
  }
}