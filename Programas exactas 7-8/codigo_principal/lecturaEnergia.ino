void lecturaEnergia(){
  if(flagCorte == 0){
    if(digitalRead(PIN_ENERGIA) == 0){
      mensajesSMS.push_back("En el central se corto la energia");
      flagCorte = 1;
    }
  }
  else{
    if(digitalRead(PIN_ENERGIA) == 1){
      mensajesSMS.push_back("En el central volvio la energia");
      flagCorte = 0;
    }
  }
}