String armadoTramaPeriferico(std::vector<datosPer> &fila) {

  // Declaramos la variable en donde estará la trama que será enviada.
  static String tramaFinal = "";

  // No se entra a la máquina de estados hasta que la lista deje de estar vacía.
  if (fila.empty()) {
    return "";  // Devuelve string vacío
  }

  // Protocolo de reordenamiento de aquellos objetos con emergencias.
  if (estadosArmadoPeriferico == EMG) {
    std::stable_partition(fila.begin(), fila.end(), [](datosPer &d) {
      return d.getEmergencia() == 1;
    });
  }

  switch (estadosArmadoPeriferico) {

    // caso de emergencia
    case EMG:

      // Si la emergencia da un valor válido
      if (fila.front().getEmergencia() == 0 || fila.front().getEmergencia() == 1) {

        // Modificamos la trama para que sea la siguiente: "#EMG,NombrePeriferico"
        // El nombre del periférico lo sacamos de una variable.
        tramaFinal = "#" + String(fila.front().getEmergencia()) + "," + periferico.nombre + ",";
        estadosArmadoPeriferico = LUZ;
      } else {
        // Devuelve error si no hay 1 o 0
        return ERROR;
      }
      break;

    case LUZ:

      if (fila.front().getLuz() == 0 || fila.front().getLuz() == 1) {

        // Modificamos la trama para que sea la siguiente: "#EMG,NombrePeriferico"
        // El nombre del periférico lo sacamos de una variable.
        tramaFinal += String(fila.front().getLuz()) + ",";
        estadosArmadoPeriferico = SENSOR_1;
      } else {
        // Devuelve error si no hay 1 o 0
        return ERROR;
      }
      break;

    // caso del primer sensor
    case SENSOR_1:

      tramaFinal += String(fila.front().getSensor1()) + ",";
      estadosArmadoPeriferico = SENSOR_2;
      break;

    // caso del segundo sensor
    case SENSOR_2:

      tramaFinal += String(fila.front().getSensor2()) + ",";
      estadosArmadoPeriferico = SENSOR_3;
      break;

    // caso del tercer sensor
    case SENSOR_3:
      tramaFinal += String(fila.front().getSensor3()) + "*";
      estadosArmadoPeriferico = ARMADO_ENVIADO;

      break;

    // Caso armado envío
    case ARMADO_ENVIADO:
      String tramaCompleta = tramaFinal;
      tramaFinal = "";                // Limpiamos el contenedor estático para la siguiente trama
      fila.erase(fila.begin());       // Eliminamos el primer elemento (Fila/Queue) ya procesado
      estadosArmadoPeriferico = EMG;  // Reiniciamos la máquina de estados al principio
      return tramaCompleta;           // Devolvemos la trama exitosa
  }

  return ""; // Devuelve vacío en los estados intermedios.
}