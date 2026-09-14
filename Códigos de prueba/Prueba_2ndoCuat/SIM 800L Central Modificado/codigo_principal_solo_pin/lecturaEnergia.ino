void lecturaEnergia(){
   int lectura = analogRead(PIN_ENERGIA);
   int voltaje = map(lectura, 0, 2700, 0, 12);
   float lecturaVoltaje = (voltaje - 0.4) * 10/43;

  if(flagCorte == 0){
    if(lecturaVoltaje <= 1.63 ){
      mensajesSMS.push_back("En el central se corto la energia");
      SerialBT.print("Se envió el mensaje --> corte de energía");
      flagCorte = 1;
    }
  }
  else{
    if(lecturaVoltaje >= 2.6){
      mensajesSMS.push_back("En el central volvio la energia");
      SerialBT.print("Se envió el mensaje --> regreso de energía");
      flagCorte = 0;
    }
  }
}