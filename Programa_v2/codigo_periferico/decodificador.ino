void decodificador(){
  if(tramas.empty() != true){
    switch(PDECO){
      case INICIO:
        tramaProceso = tramas[0];
        if(tramaProceso[0] == '#'){
          tramaProceso.remove(0,1);
          PDECO = APD;
        }
        else{
          tramas.erase(tramas.begin());
        }
      break;
      case APD:
        if(tramaProceso.substring(0, tramaProceso.indexOf(",")) == periferico.nombre){
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          PDECO = L1;
        }
        else{
          tramas.erase(tramas.begin());
          PDECO = INICIO;
        }
      break;
      case L1:
        if(tramaProceso.startsWith("L,")){
          respuesta[0] = 'L';
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          PDECO = L2;
        }
        else if(tramaProceso.startsWith("-,")){
          respuesta[0] = '-';
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          PDECO = L2;
        }
        else{
          tramas.erase(tramas.begin());
          PDECO = INICIO;
        }
      break;
      case L2:
        if(tramaProceso.startsWith("L,")){
          respuesta[1] = 'L';
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          PDECO = L3;
        }
        else if(tramaProceso.startsWith("-,")){
          respuesta[1] = '-';
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          PDECO = L3;
        }
        else{
          tramas.erase(tramas.begin());
          PDECO = INICIO;
        }
      break;
      case L3:
        if(tramaProceso.startsWith("L*")){
          respuesta[2] = 'L';
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          pedido = 1;
          leer = 1;
          Serial.println(respuesta[0]);
          Serial.println(respuesta[1]);
          Serial.println(respuesta[2]);
          tramas.erase(tramas.begin());
          PDECO = INICIO;
        }
        else if(tramaProceso.startsWith("-*")){
          respuesta[2] = '-';
          Serial.println(respuesta[0]);
          Serial.println(respuesta[1]);
          Serial.println(respuesta[2]);
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          pedido = 1;
          leer = 1;
          tramas.erase(tramas.begin());
          PDECO = INICIO;
        }
        else{
          respuesta[0] = 'L';
          respuesta[1] = 'L';
          respuesta[2] = 'L';
          tramas.erase(tramas.begin());
          PDECO = INICIO;
        }
      break;
    }
  }
}