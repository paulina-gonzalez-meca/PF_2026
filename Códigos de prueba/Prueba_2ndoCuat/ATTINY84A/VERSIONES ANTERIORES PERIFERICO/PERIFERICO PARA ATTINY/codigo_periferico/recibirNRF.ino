void recibirNRF(){/*
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    String tramaTemp = "#";
    tramaTemp += String(nombre);
    tramaTemp += ",recibido*";
    if(trama == tramaTemp){
      Serial.println(trama);
      respuestaNRF = 1;
    }
    trama = "";
  }*/
}