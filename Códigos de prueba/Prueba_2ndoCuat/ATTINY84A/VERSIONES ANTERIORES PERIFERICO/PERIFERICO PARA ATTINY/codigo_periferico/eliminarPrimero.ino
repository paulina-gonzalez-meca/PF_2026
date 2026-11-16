bool eliminarPrimero(char miLista[][TAM_MSG], int tamLista) {
  static uint8_t indiceLista = 1;

  if (indiceLista < tamLista && miLista[indiceLista][0] != '\0') {
    strcpy(miLista[indiceLista - 1], miLista[indiceLista]);
    indiceLista++;
    return false;
  } else {
    miLista[indiceLista - 1][0] = '\0';
    indiceLista = 1;
    return true;
  }
}