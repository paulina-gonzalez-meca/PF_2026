void lecturaEnergia(){
   int lectura = analogRead(PIN_ENERGIA);
   int voltaje = map(lectura, 0, 2700, 0, 12);
   float lecturaVoltaje = (voltaje - 0.4) * 10/43;

  if(flagCorte == 0){
    if(lecturaVoltaje <= 1.86 ){
      SerialBT.print("Se envió el mensaje --> corte de energía");
      flagMensajeCorte = 1;
      flagCorte = 1;
    }
  }
  else{
    if(lecturaVoltaje >= 2.6){
      SerialBT.print("Se envió el mensaje --> regreso de energía");
      flagMensajeCorte = 1;
      flagCorte = 0;
    }
  }
}