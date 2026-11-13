bool conectar(const char mensajeConectar[]) {
  static bool pendienteOK = false;
  static uint8_t reintentosOK = 0;
  static unsigned long ultimoEnvioOK = 0;

  // 1. Si ya se recibió el ID ("P1"), envía "OK" hasta que la central acuse recibo
  if (pendienteOK) {
    if (reintentosOK == 0 || (millis() - ultimoEnvioOK >= 15)) {
      ultimoEnvioOK = millis();
      radio.stopListening();
      bool enviado = radio.write("OK", 3);
      radio.startListening();
      reintentosOK++;

      if (enviado || reintentosOK >= 4) {
        pendienteOK = false;
        reintentosOK = 0;
        return true;
      }
    }
    return false;
  }

  // 2. Envía solicitud de conexión ("#C,1,2*") periódicamente
  if (primerIntentoConectar || (tiempoConeccion - ultimoEnvioConectar >= TIEMPO_CONECTAR)) {
    ultimoEnvioConectar = tiempoConeccion;
    primerIntentoConectar = false;

    radio.stopListening();
    radio.write(mensajeConectar, strlen(mensajeConectar) + 1);
    radio.startListening();
  }

  // 3. Escucha respuesta de la central ("#C, P1", "#C,P1*", "#C, P12*", etc.)
  if (radio.available()) {
    memset(mensajeRecibidoNRF, 0, sizeof(mensajeRecibidoNRF));
    radio.read(mensajeRecibidoNRF, 32);
    mensajeRecibidoNRF[32] = '\0';

    char *p = mensajeRecibidoNRF + strspn(mensajeRecibidoNRF, " \t\r\n");

    if (strncmp(p, "#C,", 3) == 0) {
      p += 3;
      p += strspn(p, " "); // Tolera espacios opcionales después de la coma

      if (*p == 'P' && isdigit((unsigned char)p[1])) {
        uint8_t digitos = isdigit((unsigned char)p[2]) ? 2 : 1;
        char sig = p[1 + digitos];

        // Acepta cierre con '*', fin de cadena '\0', espacio o coma
        if (sig == '*' || sig == '\0' || sig == ' ' || sig == '\r' || sig == '\n' || sig == ',') {
          nombre[0] = 'P';
          nombre[1] = p[1];
          if (digitos == 2) {
            nombre[2] = p[2];
            nombre[3] = '\0';
          } else {
            nombre[2] = '\0';
          }

          pendienteOK = true;
          reintentosOK = 0;
        }
      }
    }
  }

  return false;
}