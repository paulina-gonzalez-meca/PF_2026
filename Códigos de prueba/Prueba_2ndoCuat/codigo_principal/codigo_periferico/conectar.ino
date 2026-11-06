bool conectar(char mensajeConectar[], uint64_t direccion) {
  if (tiempoConeccion - tiempoConeccionAnterior % TIEMPO_CONECTAR == 0) {
    radio.stopListening();
    radio.openWritingPipe(direccion);
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    radio.startListening();
  }
  if (radio.available() > 0) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(&mensajeRecibidoNRF, sizeof(mensajeRecibidoNRF));
    String mensajeCentralConectar = String(mensajeRecibidoNRF);
    Serial.println(mensajeCentralConectar);
    mensajeCentralConectar.trim();

    nombre[0] = 'P';
    if (mensajeCentralConectar.startsWith("#C,P") && mensajeCentralConectar.endsWith("*")) {
      if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1))) {
        nombre[1] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 1);
        if (mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2) != '*') {
          if (isDigit(mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2))) {
            nombre[2] = mensajeCentralConectar.charAt(mensajeCentralConectar.indexOf('P') + 2);
          }
        }
        radio.stopListening();
        radio.openWritingPipe(direccion);
        radio.write("OK", 3);
        radio.startListening();
        return true;
      }
    }
  }
  return false;
}
