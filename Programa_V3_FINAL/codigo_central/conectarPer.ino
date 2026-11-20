bool conectar(String nombreConectar, volatile unsigned long &tiempoConectar){
  conectando = 1;
  static char mensajeConectar[9];
  static char mensajeRecibidoNRF[32];
  if(tiempoConectar % TIEMPO_CONECTAR_PER == 0){
    memset(mensajeConectar, 0, sizeof(mensajeConectar));
    snprintf(mensajeConectar, sizeof(mensajeConectar), "#C,%s*", nombreConectar);
    radio.stopListening();
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    Serial.println(String(mensajeConectar));
    radio.startListening();
  }
  if(mensajeRecibidoOK){
    mensajeRecibidoOK = 0;
    conectando = 0;
    return true;
  }
  return false; 
}