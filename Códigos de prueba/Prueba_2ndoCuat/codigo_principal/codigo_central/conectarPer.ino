bool conectar(int nombreConectar, uint64_t direccion, volatile unsigned int &tiempoConectar){
  static char mensajeConectar[8];
  static char mensajeRecibidoNRF[32];
  switch(CONECTAR_CEN){
    case ESPERA:
      if(radio.available() > 0){
        memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
        radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
        String mensajeCentralConectar = String(mensajeRecibidoNRF);
        mensajeCentralConectar.trim();
        Serial.println(mensajeCentralConectar);

        if(mensajeCentralConectar.startsWith("#C,") && mensajeCentralConectar.endsWith("*")){
          if(isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 1))){
            perifericos[indicePerifericos].tipoSensor1 = String(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 1)).toInt();
            if(isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 3))){
              perifericos[indicePerifericos].tipoSensor2 = String(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf(',') + 3)).toInt();
              memset(mensajeConectar, 0, sizeof(mensajeConectar));
              snprintf(mensajeConectar, sizeof(mensajeConectar), "#C,P%d*", (nombreConectar + 1));
              CONECTAR_CEN = ENVIO_CONECTAR;
            }
          }
        }
      }
      return false; 
      break;
    case ENVIO_CONECTAR:
      if(tiempoConectar % TIEMPO_CONECTAR_PER == 0){
        radio.stopListening();
        radio.openWritingPipe(direccion);
        radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
        Serial.println(String(mensajeConectar));
        radio.startListening();
      }
      if(radio.available() > 0){
        memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
        radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
        String mensajeCentralConectar = String(mensajeRecibidoNRF);
        mensajeCentralConectar.trim();
        Serial.println(mensajeCentralConectar);

        if(mensajeCentralConectar == "OK"){
          CONECTAR_CEN = ESPERA;
          return true;
        }
      }
      return false; 
      break;
  }
}