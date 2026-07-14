void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    Serial.println(trama);
    trama.trim();
    if(trama.startsWith("#") && trama.endsWith("*")){
      tramas.push_back(trama);
      mensajesNRF.push_back("recibido");
    }
    else if (trama == "recibido"){
      respuestaNRF = 1;
    }
    else if (trama == "v?"){
      mensajesNRF.push_back("recibido");
    }
    trama = "";
  }
}