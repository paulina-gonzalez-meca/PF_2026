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
    else{
      gestionRecibir = 3;
    }
    esperarSiguiente = false;
  }
}