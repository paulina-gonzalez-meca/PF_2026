void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    String tempRecibir = mensajesNRF[0].substring(0, mensajesNRF[0].indexOf(','));
    tempRecibir += ",recibido*";
    if(indiceMensajesNRF > 0 && mensajesNRF[0].length() > 0 && mensajesNRF[0].indexOf(',') != -1) {
      if (trama == tempRecibir){
        Serial.println(trama);
        respuestaNRF = 1;
        return;
      }
    }
    if(trama.startsWith("#P") && trama.endsWith("*")){
      Serial.println(trama);
      if(indiceMensajesNRFTR < TAM_TR){
        mensajesNRFTR[indiceMensajesNRFTR] = trama;
        indiceMensajesNRFTR ++;
      }
    }
    else if(trama.startsWith("#C") && trama.endsWith("*") && conectando){
      Serial.println(trama);
      tramaConectar = trama;
      mensajeRecibidoConectar = 1;
    }
    else if(trama == "OK" && conectando){
      Serial.println(trama);
      mensajeRecibidoOK = 1;
    }
    trama = "";
  }
}