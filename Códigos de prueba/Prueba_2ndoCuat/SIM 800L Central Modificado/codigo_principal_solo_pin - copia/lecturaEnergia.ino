void lecturaEnergia(){
  if(tiempoCorte >= 200){
    if(flagCorte == 0){
      if(!digitalRead(PIN_ENERGIA)){
        tiempoCorte = 0;
        if(indiceMensajesSMS < TAM_SMS){
          mensajesSMS[indiceMensajesSMS] = "En el central se corto la energia";
          indiceMensajesSMS ++;
          SerialBT.print("Se envió el mensaje --> corte de energía");
        }
        flagCorte = 1;
      }
    }
    else{
      if(digitalRead(PIN_ENERGIA)){
        tiempoCorte = 0;
        if(indiceMensajesSMS < TAM_SMS){
          mensajesSMS[indiceMensajesSMS] = "En el central volvio la energia";
          indiceMensajesSMS ++;
          SerialBT.print("Se envió el mensaje --> regreso de energía");
        }
        flagCorte = 0;
      }
    }
  }
  /*
   int lectura = analogRead(PIN_ENERGIA);
   int voltaje = map(lectura, 0, 2700, 0, 12);
   float lecturaVoltaje = (voltaje - 0.4) * 10/43;

  if(flagCorte == 0){
    if(lecturaVoltaje <= 1.86 ){
      mensajesSMS[indiceMensajesSMS] = "En el central se corto la energia";
      indiceMensajesSMS ++;
      SerialBT.print("Se envió el mensaje --> corte de energía");
      flagCorte = 1;
    }
  }
  else{
    if(lecturaVoltaje >= 2.6){
      mensajesSMS[indiceMensajesSMS] = "En el central volvio la energia";
      indiceMensajesSMS ++;
      SerialBT.print("Se envió el mensaje --> regreso de energía");
      flagCorte = 0;
    }
  }
  */
}