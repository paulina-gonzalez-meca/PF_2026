void perifericosEnergia(){
  if(timerEng >= TIEMPO_ENERGIA_PERIFERICOS){
    if(indiceEng < perifericos.size()){
      if(perifericos[indiceEng].conEnergia == 0 && perifericos[indiceEng].vivo == 1){
        mensajesNRF.push_back("v?");
        mensajesNRF.push_back(String(indiceEng));
        indiceEng += 1;
      }
      else{
        indiceEng += 1;
      }
    }
    else{
      indiceEng = 0;
      timerEng = 0;
    }
  }
}