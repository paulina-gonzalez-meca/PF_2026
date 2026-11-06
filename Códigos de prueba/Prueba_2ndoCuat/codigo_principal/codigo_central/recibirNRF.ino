void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    if(trama.startsWith("#P") && trama.endsWith("*")){
      Serial.println(trama);
      if(indiceMensajesNRFTR < TAM_TR){
        mensajesNRFTR[indiceMensajesNRFTR] = trama;
        indiceMensajesNRFTR ++;
      }
    }
    else if (trama.startsWith(mensajesNRF[0].substring(0, mensajesNRF[0].indexOf(',')))){
      Serial.println(trama);
      respuestaNRF = 1;
    }
    trama = "";
  }
}