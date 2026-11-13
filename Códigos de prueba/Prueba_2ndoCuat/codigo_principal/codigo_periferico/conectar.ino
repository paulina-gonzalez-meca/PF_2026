bool conectar(char mensajeConectar[]) {
  if ((tiempoConeccion - tiempoConeccionAnterior) % TIEMPO_CONECTAR == 0) {
    //Serial.print(".");
    //Serial.print(String(mensajeConectar));
    radio.stopListening();
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    radio.startListening();
  }
  if (radio.available() > 0) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    mensajeCentralConectar.trim();
    nombre[0] = 'P';
    if (mensajeCentralConectar.startsWith("#C,P") && mensajeCentralConectar.endsWith("*")) {
      //Serial.println(mensajeCentralConectar);
      if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1))) {
        nombre[1] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1);
        if (mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2) != '*') {
          if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2))) {
            nombre[2] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2);
          }
        }
        radio.stopListening();
        radio.write("OK", 3);
        radio.startListening();
        return true;
      }
    }
  }
  return false;
}
