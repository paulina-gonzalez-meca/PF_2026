void armarMensaje(String emergenciaAM, String luzAM, String nombreDispAM, String nombreSensor1AM, String nombreSensor2AM, String nombreSensor3AM, String rtaSensor1AM, String rtaSensor2AM, String rtaSensor3AM){
  String mensajeProceso;
  if(emergenciaAM == "1"){
    mensajeProceso = "Emergencia\n";
  }

  mensajeProceso += "Dispositivo: " + nombreDispAM + "\n";

  if(luzAM == "0"){
    mensajeProceso += "No hay energia\n";
  }
  else if(luzAM == "1"){
    mensajeProceso += "Hay energia\n";
  }
  else{
    mensajeProceso += "ERROR\n";
  }

  if(rtaSensor1AM != "-"){
    mensajeProceso += nombreSensor1AM + ": " + rtaSensor1AM + "\n";
  }
  if(rtaSensor2AM != "-"){
    mensajeProceso += nombreSensor2AM + ": " + rtaSensor2AM + "\n";
  }
  if(rtaSensor3AM != "-"){
    mensajeProceso += nombreSensor3AM + ": " + rtaSensor3AM + "\n";
  }
  mensajesSMS.push_back(mensajeProceso);
  Serial.println(mensajeProceso);
}