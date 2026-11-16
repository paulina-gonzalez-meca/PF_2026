void enviarDatosCentral() {
  // No envía datos hasta haber recibido su ID ("P1", "P2", etc.)
  if (!flagConectado || coneccion) return;

  unsigned long ahora = millis();
  if (ahora - ultimoEnvioDatos >= TIEMPO_ENVIO_DATOS) {
    ultimoEnvioDatos = ahora;

    if (indiceMensajesNRF < TAM_NRF) {
      char *destino = mensajesNRF[indiceMensajesNRF];
      char buf[6];

      destino[0] = '#';
      destino[1] = '\0';

      strcat(destino, nombre);
      strcat(destino, ",");

      itoa(porcentajeBateria, buf, 10);
      strcat(destino, buf);
      strcat(destino, ",");

      int16_t t = tempDecimas;
      if (t < 0) {
        strcat(destino, "-");
        t = -t;
      }
      itoa(t / 10, buf, 10);
      strcat(destino, buf);
      strcat(destino, ".");
      itoa(t % 10, buf, 10);
      strcat(destino, buf);
      strcat(destino, ",");

      strcat(destino, puerta ? "1*" : "0*");

      indiceMensajesNRF++;
    }
  }
}