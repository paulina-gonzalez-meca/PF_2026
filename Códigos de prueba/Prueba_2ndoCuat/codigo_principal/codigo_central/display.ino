void display() {
  switch (TFT_DISPLAY) {
    case INICIO:
      if (flagSWEncoder) {
        if (valorDisplay == 2) {
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        } else {
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = USUARIO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(4);
            tft.print("USUARIOS");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(4);
            tft.print("USUARIOS");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("USUARIOS");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorEncoder = 0;
          valorDisplay = 2;
        }
      } else {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 70);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(4);
            tft.print("PERIFERICOS");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 70);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(4);
            tft.print("PERIFERICOS");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 70);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("PERIFERICOS");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorEncoder = 0;
          valorDisplay = 1;
        }
      }
      break;
    case USUARIO:
      if (flagSWEncoder) {
        if (valorDisplay <= indiceUsuario) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Numero: ");
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].numeroUs);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          modificarUsuario = valorDisplay - 1;
          strcpy(numeroUsBackup, usuario[modificarUsuario].numeroUs);
          strcpy(nombreUsBackup, usuario[modificarUsuario].nombreUs);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_US;
        } else if (valorDisplay == indiceUsuario + 1) {
          if (indiceUsuario < 20) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(28, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("ABCDEFGHI");
            tft.setCursor(28, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("JKLMNOPQR");
            tft.setCursor(28, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("STUVWXYZ");
            tft.setCursor(28, 120);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Borrar");
            tft.setCursor(28, 145);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Confirmar");
            tft.setCursor(28, 170);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            strcpy(nombreUsBackup, "   ");
            strcpy(numeroUsBackup, "             ");
            valorDisplay = 1;
            valorEncoder = 0;
            flagSWEncoder = 0;
            TFT_DISPLAY = AGREGAR_NOMBRE_US;
          }
        } else if (valorDisplay == indiceUsuario + 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("USUARIOS");
          tft.setCursor(10, 70);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("PERIFERICOS");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = INICIO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else if(valorDisplay == 1){
            valorDisplay  = indiceUsuario + 2;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
        valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indiceUsuario + 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indiceUsuario + 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder <= -1){
            valorDisplay --;
          }
          else{
            valorDisplay  = 1;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      }
      break;
    case EDITAR_US:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRE_US;
        } else if (valorDisplay == 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("123");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("456");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("7890");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NUM_US;
        } else if (valorDisplay == 3) {
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          flagSWEncoder = 0;
          valorEncoder = 0;
          TFT_DISPLAY = USUARIO;
        }
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].nombreUs);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder <= -1){
            valorDisplay = 3;
          }
          else{
            valorDisplay ++;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay == 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].numeroUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].numeroUs);
            ;
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].numeroUs);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay == 3) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_NOMBRE_US:
      {
        short int returnDisplayLetras = displayLetras(usuario[modificarUsuario].nombreUs, usuario[modificarUsuario].indiceNombreUs);
        if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
          if (returnDisplayLetras == 2) {
            strcpy(usuario[modificarUsuario].nombreUs, nombreUsBackup);
          }
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].nombreUs);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Numero: ");
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].numeroUs);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_US;
        }
        break;
      }
    case EDITAR_NUM_US:
      {
        short int returnDisplayNum = displayNum(usuario[modificarUsuario].numeroUs, usuario[modificarUsuario].indiceNumeroUs);
        if (returnDisplayNum == 1 || returnDisplayNum == 2) {
          if (returnDisplayNum == 1) {
            strcpy(numeroUsBackup, usuario[modificarUsuario].numeroUs);
          } else {
            strcpy(usuario[modificarUsuario].numeroUs, numeroUsBackup);
          }
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].nombreUs);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Numero: ");
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].numeroUs);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_US;
        }
        break;
      }
    case AGREGAR_NOMBRE_US:
      {
        short int returnDisplayLetras = displayLetras(usuario[indiceUsuario].nombreUs, usuario[indiceUsuario].indiceNombreUs);
        if (returnDisplayLetras == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("123");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("456");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("7890");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = AGREGAR_NUM_US;
        }
        if (returnDisplayLetras == 2) {
          strcpy(usuario[indiceUsuario].nombreUs, nombreUsBackup);
          usuario[indiceUsuario].indiceNombreUs = 0;
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          TFT_DISPLAY = USUARIO;
        }
        break;
      }
    case AGREGAR_NUM_US:
      {
        short int returnDisplayNum = displayNum(usuario[indiceUsuario].numeroUs, usuario[indiceUsuario].indiceNumeroUs);
        if (returnDisplayNum == 1 || returnDisplayNum == 2) {
          if (returnDisplayNum == 1) {
            indiceUsuario++;
          } else {
            strcpy(usuario[indiceUsuario].numeroUs, numeroUsBackup);
            strcpy(usuario[indiceUsuario].nombreUs, nombreUsBackup);
            usuario[indiceUsuario].indiceNumeroUs = 0;
            usuario[indiceUsuario].indiceNombreUs = 0;
          }
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          TFT_DISPLAY = USUARIO;
        }
        break;
      }
    case PERIFERICO:
      if (flagSWEncoder) {
        if (valorDisplay <= indicePerifericos) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].lecturaSensor1);
          tft.setCursor(102, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreSensor2);
          tft.setCursor(102, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].lecturaSensor2);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Editar");
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Eliminar");
          modificarPer = valorDisplay - 1;
          strcpy(nombrePerBackup, "   ");
          strcpy(nombreS1Backup, "   ");
          strcpy(nombreS2Backup, "   ");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = MOSTRAR_PER;
        } else if (valorDisplay == indicePerifericos + 1) {
          if (indicePerifericos < 20) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(28, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Buscando");
            tft.setCursor(28, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("perifericos...");
            valorDisplay = 1;
            valorEncoder = 0;
            flagSWEncoder = 0;
            tiempoAgregarPer = 0;
            TFT_DISPLAY = AGREGAR_PER;
          }
        } else if (valorDisplay == indicePerifericos + 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("USUARIOS");
          tft.setCursor(10, 70);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("PERIFERICOS");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = INICIO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else if(valorDisplay == 1){
            valorDisplay  = indicePerifericos + 2;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indicePerifericos && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 5].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 4].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indicePerifericos && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            if (indicePerifericos >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 1].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 2].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 3].nombreExterno);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indicePerifericos + 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indicePerifericos && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 5].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 4].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 4].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indicePerifericos + 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder <= -1){
            valorDisplay --;
          }
          else{
            valorDisplay  = 1;
          }
          valorEncoder = 0;
          if (valorDisplay <= indicePerifericos) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            if (indicePerifericos >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 1].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 2].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 3].nombreExterno);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      }
      break;
    case MOSTRAR_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        } else if(valorDisplay == 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_PER;
        }
        else if(valorDisplay == 3){
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("Eliminar ");
          tft.setCursor(10, 55);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Si");
          tft.setCursor(60, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("No");
          valorEncoder = 0;
          valorDisplay = 1;
          eliminar = modificarPer + 1;
          strcpy(nombreEliminar, perifericos[eliminar].nombreInterno);
          TFT_DISPLAY = ELIMINAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 3;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay == 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Editar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Editar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Editar");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay == 3) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Eliminar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Eliminar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Eliminar");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombrePerBackup, perifericos[modificarPer].nombreExterno);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRE_PER;
        }
        else if (valorDisplay == 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombreS1Backup, perifericos[modificarPer].nombreSensor1);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRES1_PER;
        }
        else if (valorDisplay == 3) {
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[0]);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[1]);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[2]);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[3]);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[4]);
          }
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_ALERTAS1_PER;
        }
        else if (valorDisplay == 4) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS1);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral max: ");
          tft.print(perifericos[modificarPer].umbralMaxS1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALS1_PER;
        }
        else if (valorDisplay == 5) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombreS2Backup, perifericos[modificarPer].nombreSensor2);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRES2_PER;
        }
        else if(valorDisplay == 6){
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[0]);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[1]);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[2]);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[3]);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[4]);
          }
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_ALERTAS2_PER;
        }
        else if (valorDisplay == 7) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS2);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral max: ");
          tft.print(perifericos[modificarPer].umbralMaxS2);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALS2_PER;
        }
        else if (valorDisplay == 8) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].lecturaSensor1);
          tft.setCursor(102, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(102, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].lecturaSensor2);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Editar");
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Eliminar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = MOSTRAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Nombre: ");
            tft.print(perifericos[modificarPer].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Nombre: ");
            tft.print(perifericos[modificarPer].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 8;
          }
          valorEncoder = 0;
        }
      }else if (valorDisplay == 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Nombre s1: ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Nombre s1: ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre s1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if (valorDisplay == 3) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if (valorDisplay == 4) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if (valorDisplay == 5) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Nombre s2: ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Nombre s2: ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre s2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if (valorDisplay == 6) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if (valorDisplay == 7) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 185);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 185);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if (valorDisplay == 8) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 210);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 210);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_NOMBRE_PER: {
      short int returnDisplayLetras = displayLetras(perifericos[modificarPer].nombreExterno, perifericos[modificarPer].indiceNombrePer);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[modificarPer].nombreExterno, nombrePerBackup);
        }
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre: ");
        tft.print(perifericos[modificarPer].nombreExterno);
        tft.setCursor(10, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S1: ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S2: ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 160);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 185);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 210);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Volver");
        valorEncoder = 0;
        valorDisplay = 1;
        TFT_DISPLAY = EDITAR_PER;
      }
      break;
    }
    case EDITAR_NOMBRES1_PER: {
      short int returnDisplayLetras = displayLetras(perifericos[modificarPer].nombreSensor1, perifericos[modificarPer].indiceNombreS1);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[modificarPer].nombreSensor1, nombreS1Backup);
        }
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre: ");
        tft.print(perifericos[modificarPer].nombreExterno);
        tft.setCursor(10, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S1: ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S2: ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 160);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 185);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 210);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Volver");
        valorEncoder = 0;
        valorDisplay = 1;
        TFT_DISPLAY = EDITAR_PER;
      }
      break;
    }
    case EDITAR_ALERTAS1_PER:
      if (flagSWEncoder) {
        if (valorDisplay <= indiceUsuario) {
          if(perifericos[modificarPer].alertaS1[valorDisplay - 1] == 1){
            perifericos[modificarPer].alertaS1[valorDisplay - 1] = 0;
          }else{
            perifericos[modificarPer].alertaS1[valorDisplay - 1] = 1;
          }
        } else if (valorDisplay == indiceUsuario + 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else if(valorDisplay == 1){
            valorDisplay = indiceUsuario + 1;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 5]);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 4]);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay]);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 1]);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 2]);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
              tft.setCursor(100, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 3]);
            }
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      }else if (valorDisplay == indiceUsuario + 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder <= -1){
            valorDisplay --;
            if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 5].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 5]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 4].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 4]);
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 3].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 4].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 4]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 3].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 3].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            }
          }
          else{
            valorDisplay = 1;
            if (valorDisplay <= indiceUsuario) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
              if (indiceUsuario >= valorDisplay + 1) {
                tft.setCursor(10, 35);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay].nombreUs);
                tft.setCursor(100, 35);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS1[valorDisplay]);
              }
              if (indiceUsuario >= valorDisplay + 2) {
                tft.setCursor(10, 60);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay + 1].nombreUs);
                tft.setCursor(100, 60);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 1]);
              }
              if (indiceUsuario >= valorDisplay + 3) {
                tft.setCursor(10, 85);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay + 2].nombreUs);
                tft.setCursor(100, 85);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 2]);
              }
              if (indiceUsuario >= valorDisplay + 4) {
                tft.setCursor(10, 110);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay + 3].nombreUs);
                tft.setCursor(100, 110);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 3]);
              }
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            }
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_UMBRALS1_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].umbralMinS1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALMINS1_PER;
        } else if(valorDisplay == 2){
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].umbralMaxS1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALMAXS1_PER;
        }
        else{
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral min: ");
            tft.print(perifericos[modificarPer].umbralMinS1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral min: ");
            tft.print(perifericos[modificarPer].umbralMinS1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 3;
          }
          valorEncoder = 0;
        }
      } else if(valorDisplay == 2){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral max: ");
            tft.print(perifericos[modificarPer].umbralMaxS1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral max: ");
            tft.print(perifericos[modificarPer].umbralMaxS1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral max: ");
          tft.print(perifericos[modificarPer].umbralMaxS1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if(valorDisplay == 3){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_UMBRALMINS1_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          if(perifericos[modificarPer].umbralMinS1 < perifericos[modificarPer].umbralMaxS1 - 1){
            perifericos[modificarPer].umbralMinS1 ++;
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].nombreSensor1);
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            tft.setCursor(28, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].umbralMinS1);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
          }
        } else if(valorDisplay == 2){
          perifericos[modificarPer].umbralMinS1 --;
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].umbralMinS1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
        }
        else{
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS1);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral max: ");
          tft.print(perifericos[modificarPer].umbralMaxS1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALS1_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 3;
          }
          valorEncoder = 0;
        }
      } else if(valorDisplay == 2){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if(valorDisplay == 3){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_UMBRALMAXS1_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          perifericos[modificarPer].umbralMaxS1 ++;
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].umbralMaxS1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
        } else if(valorDisplay == 2){
          if(perifericos[modificarPer].umbralMinS1 + 1 < perifericos[modificarPer].umbralMaxS1){
            perifericos[modificarPer].umbralMaxS1 --;
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].nombreSensor1);
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            tft.setCursor(28, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].umbralMaxS1);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
          }
        }
        else{
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS1);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral max: ");
          tft.print(perifericos[modificarPer].umbralMaxS1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALS1_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 3;
          }
          valorEncoder = 0;
        }
      } else if(valorDisplay == 2){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if(valorDisplay == 3){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_NOMBRES2_PER: {
      short int returnDisplayLetras = displayLetras(perifericos[modificarPer].nombreSensor2, perifericos[modificarPer].indiceNombreS2);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[modificarPer].nombreSensor2, nombreS2Backup);
        }
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre: ");
        tft.print(perifericos[modificarPer].nombreExterno);
        tft.setCursor(10, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S1: ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S2: ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 160);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 185);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 210);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Volver");
        valorEncoder = 0;
        valorDisplay = 1;
        TFT_DISPLAY = EDITAR_PER;
      }
      break;
    }
    case EDITAR_ALERTAS2_PER:
      if (flagSWEncoder) {
        if (valorDisplay <= indiceUsuario) {
          if(perifericos[modificarPer].alertaS2[valorDisplay - 1] == 1){
            perifericos[modificarPer].alertaS2[valorDisplay - 1] = 0;
          }else{
            perifericos[modificarPer].alertaS2[valorDisplay - 1] = 1;
          }
        } else if (valorDisplay == indiceUsuario + 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else if(valorDisplay == 1){
            valorDisplay = indiceUsuario + 1;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 5]);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 4]);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 3]);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 2]);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay --;
          }
          valorEncoder = 0;
          if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay]);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay + 1]);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay + 2]);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
              tft.setCursor(100, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay + 3]);
            }
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      }else if (valorDisplay == indiceUsuario + 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder <= -1){
            valorDisplay --;
            if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 5].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 5]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 4].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 4]);
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 3].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 3]);
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 2]);
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 4].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 4]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 3].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 3]);
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 2]);
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 3].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 3]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 2]);
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 2].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 2]);
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            }
          }
          else{
            valorDisplay = 1;
            if (valorDisplay <= indiceUsuario) {
              tft.fillScreen(ILI9341_BLACK);
              tft.setCursor(10, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay - 1].nombreUs);
              tft.setCursor(100, 10);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS2[valorDisplay - 1]);
              if (indiceUsuario >= valorDisplay + 1) {
                tft.setCursor(10, 35);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay].nombreUs);
                tft.setCursor(100, 35);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS2[valorDisplay]);
              }
              if (indiceUsuario >= valorDisplay + 2) {
                tft.setCursor(10, 60);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay + 1].nombreUs);
                tft.setCursor(100, 60);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS2[valorDisplay + 1]);
              }
              if (indiceUsuario >= valorDisplay + 3) {
                tft.setCursor(10, 85);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay + 2].nombreUs);
                tft.setCursor(100, 85);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS2[valorDisplay + 2]);
              }
              if (indiceUsuario >= valorDisplay + 4) {
                tft.setCursor(10, 110);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(usuario[valorDisplay + 3].nombreUs);
                tft.setCursor(100, 110);
                tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
                tft.setTextSize(3);
                tft.print(perifericos[modificarPer].alertaS2[valorDisplay + 3]);
              }
              tft.setCursor(140, 135);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print("Volver");
            }
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_UMBRALS2_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].umbralMinS2);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALMINS2_PER;
        } else if(valorDisplay == 2){
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print(perifericos[modificarPer].umbralMaxS2);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALMAXS2_PER;
        }
        else{
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral min: ");
            tft.print(perifericos[modificarPer].umbralMinS2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral min: ");
            tft.print(perifericos[modificarPer].umbralMinS2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 3;
          }
          valorEncoder = 0;
        }
      } else if(valorDisplay == 2){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral max: ");
            tft.print(perifericos[modificarPer].umbralMaxS2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral max: ");
            tft.print(perifericos[modificarPer].umbralMaxS2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
            tft.print("Umbral max: ");
            tft.print(perifericos[modificarPer].umbralMaxS2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if(valorDisplay == 3){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_UMBRALMINS2_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          if(perifericos[modificarPer].umbralMinS2 < perifericos[modificarPer].umbralMaxS2 - 1){
            perifericos[modificarPer].umbralMinS2 ++;
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].nombreSensor2);
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            tft.setCursor(28, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].umbralMinS2);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
          }
        } else if(valorDisplay == 2){
          perifericos[modificarPer].umbralMinS2 --;
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].umbralMinS2);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
        }
        else{
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS2);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral max: ");
          tft.print(perifericos[modificarPer].umbralMaxS2);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALS2_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 3;
          }
          valorEncoder = 0;
        }
      } else if(valorDisplay == 2){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if(valorDisplay == 3){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case EDITAR_UMBRALMAXS2_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          perifericos[modificarPer].umbralMaxS2 ++;
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
          tft.setCursor(28, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].umbralMaxS2);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
        } else if(valorDisplay == 2){
          if(perifericos[modificarPer].umbralMinS2 + 1 < perifericos[modificarPer].umbralMaxS2){
            perifericos[modificarPer].umbralMaxS2 --;
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].nombreSensor2);
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            tft.setCursor(28, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].umbralMaxS2);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
          }
        }
        else{
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral min: ");
          tft.print(perifericos[modificarPer].umbralMinS2);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("umbral max: ");
          tft.print(perifericos[modificarPer].umbralMaxS2);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_UMBRALS2_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 45);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("+");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 45);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("+");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }
          else{
            valorDisplay = 3;
          }
          valorEncoder = 0;
        }
      } else if(valorDisplay == 2){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 61);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("-");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 61);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(2);
          tft.print("-");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay ++;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }else if(valorDisplay == 3){
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          if(valorEncoder >= 3){
            valorDisplay = 1;
          }else {
            valorDisplay --;
          }
          valorEncoder = 0;
        }
      }
      break;
    case ELIMINAR_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1 || valorDisplay == 2) {
          if(valorDisplay == 2){
            if(!eliminarPer(perifericos, 20, eliminar)){
              break;
            }
            indicePerifericos --;
          }
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Si");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Si");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Si");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorEncoder = 0;
          valorDisplay = 1;
        }
      } else{
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(60, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("No");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(60, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("No");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (valorEncoder >= 3 || valorEncoder <= -3) {
          tft.setCursor(60, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("No");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorEncoder = 0;
          valorDisplay = 2;
        }
      }
      break;
    case AGREGAR_PER:
    if(indicePerifericos < 20){
      if(tiempoAgregarPer <= TIMEOUT_AGREGAR){
        if(conectar(String(perifericos[indicePerifericos].nombreInterno), direccion, tiempoConectar)){
          Serial.println("conectado");
          Serial.println(String(perifericos[indicePerifericos].tipoSensor1));
          Serial.println(String(perifericos[indicePerifericos].tipoSensor2));
          indicePerifericos ++;
          tiempoConectar = 0;
          valorEncoder = 0;
          valorDisplay = 1;
          strcpy(nombrePerBackup, "----");
          TFT_DISPLAY = AGREGAR_PER_NOMBRE;
        }
      }
      else if(tiempoAgregarPer >= TIMEOUT_AGREGAR){
        tft.setCursor(28, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("No se encontro");
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("ningun periferico");
        if(tiempoAgregarPer >= TIMEOUT_AGREGAR + 1000){
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        }
      }
    }
    break;
    case AGREGAR_PER_NOMBRE:{
      short int returnDisplayLetras = displayLetras(perifericos[indicePerifericos - 1].nombreExterno, perifericos[indicePerifericos - 1].indiceNombrePer);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[indicePerifericos - 1].nombreExterno, nombrePerBackup);
        }
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombreS1Backup, "----");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
        TFT_DISPLAY = AGREGAR_PER_NS1;
      }
    break;
    }
    case AGREGAR_PER_NS1:{
      short int returnDisplayLetras = displayLetras(perifericos[indicePerifericos - 1].nombreSensor2, perifericos[indicePerifericos - 1].indiceNombreS1);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[indicePerifericos - 1].nombreSensor1, nombrePerBackup);
        }
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombreS2Backup, "----");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
        TFT_DISPLAY = AGREGAR_PER_NS2;
      }
    break;
    }
    case AGREGAR_PER_NS2:{
    short int returnDisplayLetras = displayLetras(perifericos[modificarPer].nombreSensor2, perifericos[modificarPer].indiceNombreS1);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[modificarPer].nombreSensor2, nombrePerBackup);
        }
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre: ");
        tft.print(perifericos[modificarPer].nombreExterno);
        tft.setCursor(10, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S1: ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S2: ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 160);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 185);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 210);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Volver");
        valorEncoder = 0;
        valorDisplay = 1;
        TFT_DISPLAY = EDITAR_PER;
      }
    break;
    }
  }
}