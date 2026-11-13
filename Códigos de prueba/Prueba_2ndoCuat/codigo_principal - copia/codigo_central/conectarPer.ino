bool conectar(String nombreConectar, volatile unsigned int &tiempoConectar){
  conectando = 1;
  static char mensajeConectar[8];
  static char mensajeRecibidoNRF[32];
  switch(CONECTAR_CEN){
    case ESPERA:
      if(mensajeRecibidoConectar){
        if(isDigit(tramaConectar.charAt(tramaConectar.indexOf(',') + 1))){
          perifericos[indicePerifericos].tipoSensor1 = String(tramaConectar.charAt(tramaConectar.indexOf(',') + 1)).toInt();
          if(isDigit(tramaConectar.charAt(tramaConectar.indexOf(',') + 3))){
            perifericos[indicePerifericos].tipoSensor2 = String(tramaConectar.charAt(tramaConectar.indexOf(',') + 3)).toInt();
            memset(mensajeConectar, 0, sizeof(mensajeConectar));
            snprintf(mensajeConectar, sizeof(mensajeConectar), "#C,%s*", nombreConectar);
            CONECTAR_CEN = ENVIO_CONECTAR;
          }
        }
        mensajeRecibidoConectar = 0;
      }
      return false; 
      break;
    case ENVIO_CONECTAR:
      if(tiempoConectar % TIEMPO_CONECTAR_PER == 0){
        radio.stopListening();
        radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
        Serial.println(String(mensajeConectar));
        radio.startListening();
      }
      if(mensajeRecibidoOK){
        mensajeRecibidoOK = 0;
        CONECTAR_CEN = ESPERA;
        conectando = 0;
        return true;
      }
      return false; 
      break;
  }
}