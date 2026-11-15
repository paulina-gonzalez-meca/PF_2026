void recibirSMS(){
  if (sim800l.available()) {
    char c = sim800l.read();
    if (c == '\n') {
      simBuffer.trim(); 
      
      if (simBuffer.length() > 0) {
        verificarMensajeSMS(simBuffer);
      }
      
      simBuffer = ""; 
    } else if (c != '\r') {
      simBuffer += c;
    }
  }
}

void verificarMensajeSMS(String mensajeRecibidoSMS) {
  if (mensajeRecibidoSMS.startsWith("+CMT:")) {
    esperarSiguiente = true;
  } 
  else if (esperarSiguiente) {
    if(mensajeRecibidoSMS == "/menu"){
      gestionRecibir = 1;
    }
    else if(mensajeRecibidoSMS == "/status"){
      gestionRecibir = 2;
    }
    else if(mensajeRecibidoSMS.startsWith("/Nombre:")){
      if(mensajeRecibidoSMS.substring(mensajeRecibidoSMS.indexOf(':') + 1).length() == 4){
        strcpy(perifericos[0].nombreExterno,mensajeRecibidoSMS.substring(8).c_str());
        gestionRecibir = 3;
      }
      else{
        gestionRecibir = 5;
      }
    }
    else if(mensajeRecibidoSMS.startsWith("/valorUmbral")){
      gestionRecibir = 4;
    }
    else if(mensajeRecibidoSMS.startsWith("/UmbralMin:")){
      if(mensajeRecibidoSMS.substring(mensajeRecibidoSMS.indexOf(':') + 1) != ""){
        indiceEsNumMin = 0;
        mensajeRecibidoNumMin = mensajeRecibidoSMS.substring(mensajeRecibidoSMS.indexOf(':') + 1);
      }
      else{
        gestionRecibir = 5;
      }
    }
    else if(mensajeRecibidoSMS.startsWith("/UmbralMax:")){
      if(mensajeRecibidoSMS.substring(mensajeRecibidoSMS.indexOf(':') + 1) != ""){
        indiceEsNumMax = 0;
        mensajeRecibidoNumMax = mensajeRecibidoSMS.substring(mensajeRecibidoSMS.indexOf(':') + 1);
      }
      else{
        gestionRecibir = 5;
      }
    }
    else if(mensajeRecibidoSMS.startsWith("/Numero:")){
      if(mensajeRecibidoSMS.substring(mensajeRecibidoSMS.indexOf(':') + 1) != ""){
        strcpy(usuario[indiceUsuario].numeroUs, mensajeRecibidoSMS.substring(mensajeRecibidoSMS.indexOf(':') + 1).c_str());
        indiceUsuario ++;
        gestionRecibir = 3;
      }
      else{
        gestionRecibir = 5;
      }
    }
    else{
      gestionRecibir = 5;
    }
    esperarSiguiente = false;
  }
}