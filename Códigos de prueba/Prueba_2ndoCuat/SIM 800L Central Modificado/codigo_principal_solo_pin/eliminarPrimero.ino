bool eliminarPrimero(String miLista[], int tamLista){
  static int indiceLista = 1;
  if(indiceLista < tamLista && miLista[indiceLista] != "\0" && miLista[indiceLista] != ""){
    miLista[indiceLista - 1] = miLista[indiceLista];
    indiceLista ++;
    return false;
  }
  else{
    miLista[indiceLista - 1] = "";
    indiceLista = 1;
    return true;
  }
}