void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    uint8_t len = radio.getDynamicPayloadSize();
    if (len < 1 || len >= sizeof(mensajeRecibidoNRF)) {
      radio.flush_rx();
      return;
    }
    radio.read(&mensajeRecibidoNRF, len);
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    Serial.println(trama);
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
      digitalWrite(PIN_BUZZER, HIGH);
      delay(500);
      digitalWrite(PIN_BUZZER, LOW);
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