void recibirNRF(){
  if(radio.available() > 0){
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    trama = String(mensajeRecibidoNRF);
    trama.trim();
    if(trama.startsWith("#1,") && trama.endsWith("*")){
      digitalWrite(PIN_LED2, HIGH);
      tiempoLed2 = 0;
      Serial.println(trama);
      SerialBT.print(trama);
      emergencias.push_back(trama);
    }
    else if(trama.startsWith("#0,") && trama.endsWith("*")){
      digitalWrite(PIN_LED2, HIGH);
      tiempoLed2 = 0;
      Serial.println(trama);
      SerialBT.print(trama);
      tramas.push_back(trama);
    }
    else if (trama == "recibido"){
      digitalWrite(PIN_LED2, HIGH);
      tiempoLed2 = 0;
      Serial.println(trama);
      SerialBT.print(trama);
      respuestaNRF = 1;
    }
    else if (trama == "probando"){
      digitalWrite(PIN_LED2, HIGH);
      tiempoLed2 = 0;
      Serial.println(trama);
      SerialBT.print(trama);
      mensajesNRF.push_back("recibido");
    }
    trama = "";
  }
}