void lecturaEnergia(){
  int lectura = analogRead(PIN_ENERGIA);
  float voltaje = map(lectura, 0, 4095, 0, 330); //map solo funciona con números enteros y devuelve int
  voltaje = voltaje / 100;
  // SerialBT.println(voltaje);
  if(flagCorte == 0){
    if(voltaje <= 2){
      // SerialBT.print("Se envió el mensaje --> corte de energía");
      flagMensajeCorte = 1;
      flagCorte = 1;
    }
  }
  else{
    if(voltaje >= 2){
      // SerialBT.print("Se envió el mensaje --> regreso de energía");
      flagMensajeCorte = 1;
      flagCorte = 0;
    }

    if(voltaje >= 1.67){
      porcentajeBateria = 100;
    }
    else if(voltaje <= 1.1){
      porcentajeBateria = 0;
    }
    else{
      porcentajeBateria = map(voltaje*100, 110, 167, 0, 100);
    }
  }
}