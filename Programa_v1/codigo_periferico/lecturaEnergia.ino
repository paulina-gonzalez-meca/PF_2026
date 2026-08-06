void lecturaEnergia(){
  if(flagCorte){
    if(digitalRead(PIN_ENERGIA) == 0){
      emergencia = 1;
      flagCorte = 0;
    }
  }
  else{
    if(digitalRead(PIN_ENERGIA) == 1){
      flagNormalidad = 1;
      flagCorte = 1;
    }
  }
}