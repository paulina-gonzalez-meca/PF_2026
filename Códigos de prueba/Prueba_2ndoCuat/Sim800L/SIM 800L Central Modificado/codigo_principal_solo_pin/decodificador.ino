void decodificador(){
  if(eliminarTR){
    if(eliminarPrimero(mensajesNRFTR, TAM_TR)){
      indiceMensajesNRFTR --;
      eliminarTR = 0;
    }
    return;
  }
  if(indiceMensajesNRFTR >= 1){
    if(ind == 0 && PDECO == APD){
      tramaProceso = mensajesNRFTR[0];
    }
    switch(PDECO){
      case APD:
        if(String(perifericos[ind].nombreInterno) == tramaProceso.substring(1, tramaProceso.indexOf(","))){
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          String tempStr = "#";
          tempStr += String(perifericos[ind].nombreInterno);
          tempStr += ",recibido*";
          mensajesNRF[indiceMensajesNRF] = tempStr;
          indiceMensajesNRF++;
          PDECO = BAT;
        }
        else {
          ind += 1;
          if(ind >= indicePerifericos){
            ind = 0;
            eliminarTR = 1;
          }
        }
      break;
      case BAT:
      if(isDigit(tramaProceso.charAt(0))){
        if(isDigit(tramaProceso.charAt(1))){
          if(isDigit(tramaProceso.charAt(2))){
            if(tramaProceso.charAt(3) == ','){
              perifericos[ind].bateria = tramaProceso.substring(0, tramaProceso.indexOf(',')).toInt();
              tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
              PDECO = RSLT_S1;
            }
            else{
              perifericos[ind].bateria = -1;
              tramaProceso.remove(0,tramaProceso.indexOf(',')+1);
              PDECO = RSLT_S1;
            }
          }
          else if(tramaProceso.charAt(2) == ','){
            perifericos[ind].bateria = tramaProceso.substring(0, tramaProceso.indexOf(',')).toInt();
            tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
            PDECO = RSLT_S1;
          }
          else{
            perifericos[ind].bateria = -1;
            tramaProceso.remove(0,tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S1;
          }
        }
        else if(tramaProceso.charAt(1) == ','){
          perifericos[ind].bateria = tramaProceso.substring(0, tramaProceso.indexOf(',')).toInt();
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          PDECO = RSLT_S1;
        }
        else{
          perifericos[ind].bateria = - 1;
          tramaProceso.remove(0,tramaProceso.indexOf(',')+1);
          PDECO = RSLT_S1;
        }
      }
      else{
        respuesta[1] = "ERROR";
        tramaProceso.remove(0,tramaProceso.indexOf(',')+1);
        PDECO = RSLT_S1;
      }
      break;
      case RSLT_S1:
        if(perifericos[ind].tipoSensor1 == 0){
          if(tramaProceso[0] == '-' && tramaProceso[1] == ','){
            tramaProceso.remove(0, 2);
            perifericos[ind].lecturaSensor1 = "-";
            PDECO = RSLT_S2;
          }
          else{
            perifericos[ind].lecturaSensor1 = "ERROR";
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S2;
          }
        }
        else{
          if(tramaProceso[0] == '-' && tramaProceso[1] == ','){
            perifericos[ind].lecturaSensor1 = "-";
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S2;
          }
          else{
            perifericos[ind].lecturaSensor1 = tramaProceso.substring(0, tramaProceso.indexOf(','));
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S2;
          }
        }
      break;
      case RSLT_S2:
        if(perifericos[ind].tipoSensor2 == 0){
          if(tramaProceso[0] == '-' && tramaProceso[1] == '*'){
            tramaProceso.remove(0, 2);
            perifericos[ind].lecturaSensor2 = "-";
            eliminarTR = 1;
            indpr = ind;
            ind = 0;
            end = 1;
            PDECO = APD;
          }
          else{
            perifericos[ind].lecturaSensor2 = "ERROR";
            tramaProceso.remove(0, tramaProceso.indexOf('*')+1);
            eliminarTR = 1;
            indpr = ind;
            ind = 0;
            end = 1;
            PDECO = APD;
          }
        }
        else{
          if(tramaProceso[0] == '-' && tramaProceso[1] == '*'){
            perifericos[ind].lecturaSensor2 = "-";
            tramaProceso.remove(0, tramaProceso.indexOf('*')+1);
            eliminarTR = 1;
            indpr = ind;
            ind = 0;
            end = 1;
            PDECO = APD;
          }
          else{
            perifericos[ind].lecturaSensor2 = tramaProceso.substring(0, tramaProceso.indexOf('*'));
            tramaProceso.remove(0, tramaProceso.indexOf('*')+1);
            eliminarTR = 1;
            indpr = ind;
            ind = 0;
            end = 1;
            PDECO = APD;
          }
        }
      break;
    }
  }
}