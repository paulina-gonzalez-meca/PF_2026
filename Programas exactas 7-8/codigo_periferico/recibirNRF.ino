void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    if(trama.startsWith("#") && trama.endsWith("*")){
      digitalWrite(PIN_LED, HIGH);
      tiempoLed = 0;
      Serial.println(trama);
      SerialBT.print(trama);
      tramas.push_back(trama);
      mensajesNRF.push_back("recibido");
    }
    else if (trama == "recibido"){
      digitalWrite(PIN_LED, HIGH);
      tiempoLed = 0;
      Serial.println(trama);
      SerialBT.print(trama);
      respuestaNRF = 1;
    }
    else if (trama == "v?" || trama == "probando"){
      digitalWrite(PIN_LED, HIGH);
      tiempoLed = 0;
      Serial.println(trama);
      SerialBT.print(trama);
      mensajesNRF.push_back("recibido");
    }
    trama = "";
  }
}