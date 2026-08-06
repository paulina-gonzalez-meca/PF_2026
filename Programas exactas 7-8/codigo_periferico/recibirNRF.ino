void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    if(trama.startsWith("#") && trama.endsWith("*")){
      Serial.println(trama);
      tramas.push_back(trama);
      mensajesNRF.push_back("recibido");
    }
    else if (trama == "recibido"){
      Serial.println(trama);
      respuestaNRF = 1;
    }
    else if (trama == "v?"){
      Serial.println(trama);
      mensajesNRF.push_back("recibido");
    }
    trama = "";
  }
}