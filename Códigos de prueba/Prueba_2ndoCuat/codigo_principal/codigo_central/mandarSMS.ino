bool mandarSMS(String mensaje, String numero){
  switch(PSMS){
    case PASO1:
      digitalWrite(PIN_DTR, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));
      sim800l.println("AT+CMGS=\"" + numero + "\"");
      tiempoDelay = 0;
      PSMS = PASO2;
      return false;
    break;
    case PASO2:
      if(tiempoDelay >= 500){
        sim800l.print(mensaje);
        PSMS = PASO3;
      }
      return false;
    break;
    case PASO3:
      if(tiempoDelay >= 1000){
        sim800l.write(0x1A);
        SerialBT.println("mandando SMS");
        tiempoDelay = 0;
        timerSMS = 0;
        PSMS = PASO1;
        digitalWrite(PIN_DTR, HIGH);
        return true;
      }
      return false;
    break;
  }
}

void manejoMandarSMS(){
  switch (emgMandarSMS){
    case 0:
      if(flagMensajeCorte){
        if(flagCorte){
          if (timerSMS >= TIEMPO_ENVIAR_SMS) {
            if (indiceNum < indiceUsuario) {
              mandandoSMS = 1;
              if(mandarSMS("En el central se corto la energia", usuario[indiceNum].numeroUs)){
                indiceNum += 1;
              }
              return;
            }
            else{
              mandandoSMS = 0;
              flagMensajeCorte = 0;
              indiceNum = 0;
            }
          }
        }
        else{
          if (timerSMS >= TIEMPO_ENVIAR_SMS) {
            if (indiceNum < indiceUsuario) {
              mandandoSMS = 1;
              if(mandarSMS("En el central volvio la energia", usuario[indiceNum].numeroUs)){
                indiceNum += 1;
              }
              break;
            }
            else{
              mandandoSMS = 0;
              flagMensajeCorte = 0;
              indiceNum = 0;
            }
          }
        }
      }
      emgMandarSMS = 1;
    break;
    case 1:
      if(indicePerifericos >= indicePerSMS + 1){
        if(!perifericos[indicePerSMS].flagEmgS1){
          if(perifericos[indicePerSMS].emergenciaS1 != 0){
            if (timerSMS >= TIEMPO_ENVIAR_SMS) {
              if (indiceNum < indiceUsuario) {
                if(perifericos[indicePerSMS].alertaS1[indiceNum] == 1){
                  mandandoSMS = 1;
                  if(perifericos[indicePerSMS].tipoSensor1 == 1){
                    if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa temperatura esta fuera de rango: " + perifericos[indicePerSMS].lecturaSensor1, usuario[indiceNum].numeroUs)){
                      indiceNum += 1;
                    }
                  }
                  else if(perifericos[indicePerSMS].tipoSensor1 == 2){
                    if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa puerta estuvo abierta por mas de 30s", usuario[indiceNum].numeroUs)){
                      indiceNum += 1;
                    }
                  }
                }
                break;
              }
              else{
                mandandoSMS = 0;
                indiceNum = 0;
                perifericos[indicePerSMS].flagEmgS1 = 1;
              }
            }
          }
        }
        else if(perifericos[indicePerSMS].emergenciaS1 == 0){
          if (timerSMS >= TIEMPO_ENVIAR_SMS) {
            if (indiceNum < indiceUsuario) {
              if(perifericos[indicePerSMS].alertaS1[indiceNum] == 1){
                mandandoSMS = 1;
                if(perifericos[indicePerSMS].tipoSensor1 == 1){
                  if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa temperatura esta de vuelta en el rango: " + perifericos[indicePerSMS].lecturaSensor1, usuario[indiceNum].numeroUs)){
                    indiceNum += 1;
                  }
                }
                else if(perifericos[indicePerSMS].tipoSensor1 == 2){
                  if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa puerta esta cerrada", usuario[indiceNum].numeroUs)){
                    indiceNum += 1;
                  }
                }
              }
              break;
            }
            else{
              mandandoSMS = 0;
              indiceNum = 0;
              perifericos[indicePerSMS].flagEmgS1 = 0;
            }
          }
        }
      }
      emgMandarSMS = 2;
    break;
    case 2:
      if(indicePerifericos >= indicePerSMS + 1){
        if(!perifericos[indicePerSMS].flagEmgS2){
          if(perifericos[indicePerSMS].emergenciaS2 != 0){
            if (timerSMS >= TIEMPO_ENVIAR_SMS) {
              if (indiceNum < indiceUsuario) {
                if(perifericos[indicePerSMS].alertaS2[indiceNum] == 1){
                  mandandoSMS = 1;
                  if(perifericos[indicePerSMS].tipoSensor2 == 1){
                    if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa temperatura esta fuera de rango: " + perifericos[indicePerSMS].lecturaSensor2, usuario[indiceNum].numeroUs)){
                      indiceNum += 1;
                    }
                  }
                  else if(perifericos[indicePerSMS].tipoSensor2 == 2){
                    if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa puerta estuvo abierta por mas de 30s", usuario[indiceNum].numeroUs)){
                      indiceNum += 1;
                    }
                  }
                }
                break;
              }
              else{
                indiceNum = 0;
                mandandoSMS = 0;
                perifericos[indicePerSMS].flagEmgS2 = 1;
              }
            }
          }
        }
        else if(perifericos[indicePerSMS].emergenciaS2 == 0){
          if (timerSMS >= TIEMPO_ENVIAR_SMS) {
            if (indiceNum < indiceUsuario) {
              if(perifericos[indicePerSMS].alertaS2[indiceNum] == 1){
                mandandoSMS = 1;
                if(perifericos[indicePerSMS].tipoSensor2 == 1){
                  if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa temperatura esta devuelta en el rango: " + perifericos[indicePerSMS].lecturaSensor2, usuario[indiceNum].numeroUs)){
                    indiceNum += 1;
                  }
                }
                else if(perifericos[indicePerSMS].tipoSensor2 == 2){
                  if(mandarSMS(String(perifericos[indicePerSMS].nombreExterno) + "\nLa puerta esta cerrada", usuario[indiceNum].numeroUs)){
                    indiceNum += 1;
                  }
                }
              }
              break;
            }
            else{
              indiceNum = 0;
              mandandoSMS = 0;
              perifericos[indicePerSMS].flagEmgS2 = 0;
            }
          }
        }
      }
      emgMandarSMS = 0;
      if(indicePerSMS < 19){
        indicePerSMS ++;
      }
      else{
        indicePerSMS = 0;
      }
    break;
  }
}