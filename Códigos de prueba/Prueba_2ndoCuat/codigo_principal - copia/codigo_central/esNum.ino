bool esNum(String &strNumerico, int &indiceNumerico, int &valorUmbral){
  if(strNumerico.length() <= indiceNumerico){
    if(isDigit(strNumerico.charAt(indiceNumerico))){
      indiceNumerico ++;
      return false;
    }
    else{
      indiceNumerico = 0;
      strNumerico = "";
      gestionRecibir = 5;
      return false;
    }
  }
  else if(strNumerico.charAt(0) == '\n'){
    indiceNumerico = 0;
    strNumerico = "";
    gestionRecibir = 5;
    return false;
  }
  indiceNumerico = 0;
  return true;
}