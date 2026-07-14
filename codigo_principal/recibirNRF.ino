void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    if(trama.startsWith("#1,") && trama.endsWith("*")){
      Serial.println(trama);
      emergencias.push_back(trama);
    }
    else if(trama.startsWith("#0,") && trama.endsWith("*")){
      Serial.println(trama);
      tramas.push_back(trama);
    }
    else if (trama == "recibido"){
      Serial.println(trama);
      respuestaNRF = 1;
    }
    trama = "";
  }
}