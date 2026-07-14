void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    Serial.println(trama);
    trama.trim();
    if(trama.startsWith("#1,") && trama.endsWith("*")){
    emergencias.push_back(trama);
    }
    else if(trama.startsWith("#0,") && trama.endsWith("*")){
      tramas.push_back(trama);
    }
    else if (trama == "recibido"){
      respuestaNRF = 1;
    }
    trama = "";
  }
}