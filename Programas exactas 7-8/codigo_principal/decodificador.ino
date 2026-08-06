void decodificador(){
  if(tramas.empty() != true || emergencias.empty() != true){
    if(decoEmg == 0 && emergencias.empty() != true){
      tramaPausada.pasoDeco = PDECO;
      tramaPausada.indice = ind;
      tramaPausada.tramaPausa = tramaProceso;
      tramaPausada.respuestas[0] = respuesta[0];
      tramaPausada.respuestas[1] = respuesta[1];
      tramaPausada.respuestas[2] = respuesta[2];
      tramaPausada.respuestas[3] = respuesta[3];
      tramaPausada.respuestas[4] = respuesta[4];
      tramaProceso = emergencias[0];
      respuesta[0] = "1";
      tramaProceso.remove(0,3);
      ind = 0;
      decoEmg = 1;
      PDECO = APD;
    }
    switch(PDECO){
      case EMG:
        tramaProceso = tramas[0];
        if(tramaProceso.startsWith("#0,") == true){
          respuesta[0] = "0";
          tramaProceso.remove(0,3);
          PDECO = APD;
        }
        else if(tramaProceso.startsWith("#1,") == true){
          respuesta[0] = "1";
          tramaProceso.remove(0,3);
          PDECO = APD;
          }
        else{
          tramas.erase(tramas.begin());
        }
      break;
      case APD:
        if(perifericos[ind].nombreInterno == tramaProceso.substring(0, tramaProceso.indexOf(","))){
          respuesta[1] = perifericos[ind].nombreInterno;
          tramaProceso.remove(0, tramaProceso.indexOf(",")+1);
          mensajesNRF.push_back("recibido");
          mensajesNRF.push_back(String(ind));
          PDECO = LUZ;
        }
        else {
          ind += 1;
          if(ind >= perifericos.size()){
            ind = 0;
            if(decoEmg == 0){
              tramas.erase(tramas.begin());
              PDECO = EMG;
            }
            else{
              emergencias.erase(emergencias.begin());
              if(emergencias.empty()){
                ind = tramaPausada.indice;
                tramaProceso = tramaPausada.tramaPausa;
                respuesta[0] = tramaPausada.respuestas[0];
                respuesta[1] = tramaPausada.respuestas[1];
                respuesta[2] = tramaPausada.respuestas[2];
                respuesta[3] = tramaPausada.respuestas[3];
                respuesta[4] = tramaPausada.respuestas[4];
                decoEmg = 0;
                PDECO = tramaPausada.pasoDeco;
              }
              else{
                tramaProceso = emergencias[0];
                respuesta[0] = "1";
                tramaProceso.remove(0,3);
                ind = 0;
                decoEmg = 1;
                PDECO = APD;
              }
            }
          }
        }
      break;
      case LUZ:
      if(tramaProceso.startsWith("0,") == true){
        respuesta[1] = "0";
        perifericos[ind].conEnergia = 0;
        tramaProceso.remove(0,2);
        PDECO = RSLT_S1;
      }
      else if(tramaProceso.startsWith("1,") == true){
        respuesta[1] = "1";
        perifericos[ind].conEnergia = 1;
        tramaProceso.remove(0,2);
        PDECO = RSLT_S1;
        }
      else{
        respuesta[1] = "ERROR";
        tramaProceso.remove(0,tramaProceso.indexOf(',')+1);
        PDECO = RSLT_S1;
      }
      break;
      case RSLT_S1:
        if(perifericos[ind].nombreSensor1 == "-"){
          if(tramaProceso[0] == '-' && tramaProceso[1] == ','){
            tramaProceso.remove(0, 2);
            respuesta[2] = "-";
            PDECO = RSLT_S2;
          }
          else{
            respuesta[2] = "ERROR";
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S2;
          }
        }
        else{
          if(tramaProceso[0] == '-' && tramaProceso[1] == ','){
            respuesta[2] = "-";
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S2;
          }
          else{
            respuesta[2] = tramaProceso.substring(0, tramaProceso.indexOf(','));
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S2;
          }
        }
      break;
      case RSLT_S2:
        if(perifericos[ind].nombreSensor2 == "-"){
          if(tramaProceso[0] == '-' && tramaProceso[1] == ','){
            tramaProceso.remove(0, 2);
            respuesta[3] = "-";
            PDECO = RSLT_S3;
          }
          else{
            respuesta[3] = "ERROR";
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S3;
          }
        }
        else{
          if(tramaProceso[0] == '-' && tramaProceso[1] == ','){
            respuesta[3] = "-";
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S3;
          }
          else{
            respuesta[3] = tramaProceso.substring(0, tramaProceso.indexOf(','));
            tramaProceso.remove(0, tramaProceso.indexOf(',')+1);
            PDECO = RSLT_S3;
          }
        }
      break;
      case RSLT_S3:
        if(perifericos[ind].nombreSensor3 == "-"){
          if(tramaProceso[0] == '-' && tramaProceso[1] == '*'){
            tramaProceso.remove(0, 2);
            respuesta[4] = "-";
            armarMensaje(respuesta[0], respuesta[1], perifericos[ind].nombreExterno, perifericos[ind].nombreSensor1, perifericos[ind].nombreSensor2, perifericos[ind].nombreSensor3, respuesta[2], respuesta[3], respuesta[4]);
            if(decoEmg == 0){
              tramas.erase(tramas.begin());
              ind = 0;
              PDECO = EMG;
            }
            else{
              emergencias.erase(emergencias.begin());
              if(emergencias.empty()){
                ind = tramaPausada.indice;
                tramaProceso = tramaPausada.tramaPausa;
                respuesta[0] = tramaPausada.respuestas[0];
                respuesta[1] = tramaPausada.respuestas[1];
                respuesta[2] = tramaPausada.respuestas[2];
                respuesta[3] = tramaPausada.respuestas[3];
                respuesta[4] = tramaPausada.respuestas[4];
                PDECO = tramaPausada.pasoDeco;
                decoEmg = 0;
              }
              else{
                tramaProceso = emergencias[0];
                respuesta[0] = "1";
                tramaProceso.remove(0,3);
                ind = 0;
                decoEmg = 1;
                PDECO = APD;
              }
            }
          }
          else{
            tramaProceso.remove(0, tramaProceso.indexOf('*')+1);
            respuesta[4] = "ERROR";
            armarMensaje(respuesta[0], respuesta[1], perifericos[ind].nombreExterno, perifericos[ind].nombreSensor1, perifericos[ind].nombreSensor2, perifericos[ind].nombreSensor3, respuesta[2], respuesta[3], respuesta[4]);
            if(decoEmg == 0){
              tramas.erase(tramas.begin());
              ind = 0;
              PDECO = EMG;
            }
            else{
              emergencias.erase(emergencias.begin());
              if(emergencias.empty()){
                ind = tramaPausada.indice;
                tramaProceso = tramaPausada.tramaPausa;
                respuesta[0] = tramaPausada.respuestas[0];
                respuesta[1] = tramaPausada.respuestas[1];
                respuesta[2] = tramaPausada.respuestas[2];
                respuesta[3] = tramaPausada.respuestas[3];
                respuesta[4] = tramaPausada.respuestas[4];
                PDECO = tramaPausada.pasoDeco;
                decoEmg = 0;
              }
              else{
                tramaProceso = emergencias[0];
                respuesta[0] = "1";
                tramaProceso.remove(0,3);
                ind = 0;
                decoEmg = 1;
                PDECO = APD;
              }
            }
          }
        }
        else{
          if(tramaProceso[0] == '-' && tramaProceso[1] == '*'){
            tramaProceso.remove(0, tramaProceso.indexOf('*')+1);
            respuesta[4] = "-";
            armarMensaje(respuesta[0], respuesta[1], perifericos[ind].nombreExterno, perifericos[ind].nombreSensor1, perifericos[ind].nombreSensor2, perifericos[ind].nombreSensor3, respuesta[2], respuesta[3], respuesta[4]);
            if(decoEmg == 0){
              tramas.erase(tramas.begin());
              ind = 0;
              PDECO = EMG;
            }
            else{
              emergencias.erase(emergencias.begin());
              if(emergencias.empty()){
                ind = tramaPausada.indice;
                tramaProceso = tramaPausada.tramaPausa;
                respuesta[0] = tramaPausada.respuestas[0];
                respuesta[1] = tramaPausada.respuestas[1];
                respuesta[2] = tramaPausada.respuestas[2];
                respuesta[3] = tramaPausada.respuestas[3];
                respuesta[4] = tramaPausada.respuestas[4];
                PDECO = tramaPausada.pasoDeco;
                decoEmg = 0;
              }
              else{
                tramaProceso = emergencias[0];
                respuesta[0] = "1";
                tramaProceso.remove(0,3);
                ind = 0;
                decoEmg = 1;
                PDECO = APD;
              }
            }
          }
          else{
            respuesta[4] = tramaProceso.substring(0, tramaProceso.indexOf('*'));
            tramaProceso.remove(0, tramaProceso.indexOf('*')+1);
            armarMensaje(respuesta[0], respuesta[1], perifericos[ind].nombreExterno, perifericos[ind].nombreSensor1, perifericos[ind].nombreSensor2, perifericos[ind].nombreSensor3, respuesta[2], respuesta[3], respuesta[4]);
            if(decoEmg == 0){
              tramas.erase(tramas.begin());
              ind = 0;
              PDECO = EMG;
            }
            else{
              emergencias.erase(emergencias.begin());
              if(emergencias.empty()){
                ind = tramaPausada.indice;
                tramaProceso = tramaPausada.tramaPausa;
                respuesta[0] = tramaPausada.respuestas[0];
                respuesta[1] = tramaPausada.respuestas[1];
                respuesta[2] = tramaPausada.respuestas[2];
                respuesta[3] = tramaPausada.respuestas[3];
                respuesta[4] = tramaPausada.respuestas[4];
                PDECO = tramaPausada.pasoDeco;
                decoEmg = 0;
              }
              else{
                tramaProceso = emergencias[0];
                respuesta[0] = "1";
                tramaProceso.remove(0,3);
                ind = 0;
                decoEmg = 1;
                PDECO = APD;
              }
            }
          }
        }
      break;
    }
  }
}