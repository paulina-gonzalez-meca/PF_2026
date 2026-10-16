int displayNum(char numero[], int &indiceNumero){
  if(flagSWEncoder){
    if(indiceNumero < 13 && valorDisplay < 11){
      numero[indiceNumero] = numeros[valorDisplay - 1];
      indiceNumero ++;
      numero[indiceNumero] = '\0';
    }
    else if(valorDisplay == 11 && indiceNumero > 0){
      indiceNumero --;
      numero[indiceNumero] = '\0';
    }
    else if(valorDisplay == 12 && numero[12] != '\0'){
      flagSWEncoder = 0;
      return 1;
    }
    else if(valorDisplay == 13){
      flagSWEncoder = 0;
      return 2;
    }
    flagSWEncoder = 0;
  }
  tft.setCursor( 10, 10);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.print(numero[0]);
  tft.print(numero[1]);
  tft.print(numero[2]);
  tft.print(numero[3]);
  tft.print(numero[4]);
  tft.print(numero[5]);
  tft.print(numero[6]);
  tft.print(numero[7]);
  tft.print(numero[8]);
  tft.print(numero[9]);
  tft.print(numero[10]);
  tft.print(numero[11]);
  tft.print(numero[12]);
  tft.print(numero[13]);
  if(valorDisplay >= 1 && valorDisplay <= 3){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor( valorDisplay * ANCHO + 10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print(numeros[valorDisplay - 1]);
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor(valorDisplay * ANCHO + 10, 35);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(numeros[valorDisplay - 1]);
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 13)){
      tft.setCursor( valorDisplay * ANCHO + 10, 35);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print(numeros[valorDisplay - 1]);
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 13);
    }
  }
  else if(valorDisplay >= 4 && valorDisplay <= 6){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor(valorDisplay * ANCHO - 44, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print(numeros[valorDisplay - 1]);
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor(valorDisplay * ANCHO - 44, 60);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(numeros[valorDisplay - 1]);
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 13)){
      tft.setCursor(valorDisplay * ANCHO - 44, 60);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print(numeros[valorDisplay - 1]);
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 13);
    }
  }
  else if(valorDisplay >= 7 && valorDisplay <= 10){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor(valorDisplay * ANCHO - 98, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print(numeros[valorDisplay - 1]);
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor(valorDisplay * ANCHO - 98, 85);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(numeros[valorDisplay - 1]);
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 13)){
      tft.setCursor(valorDisplay * ANCHO  - 98, 85);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print(numeros[valorDisplay - 1]);
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 13);
    }
  }
  else if(valorDisplay == 11){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor( 28, 120);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Borrar");
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor( 28, 120);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print("Borrar");
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 13)){
      tft.setCursor( 28, 120);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print("Borrar");
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 13);
    }
  }
  else if(valorDisplay == 12){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor( 28, 145);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Confirmar");
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor( 28, 145);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print("Confirmar");
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 13)){
      tft.setCursor( 28, 145);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print("Confirmar");
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 13);
    }
  }
  else if(valorDisplay == 13){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor( 28, 170);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Cancelar");
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor( 28, 170);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print("Cancelar");
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 13)){
      tft.setCursor( 28, 170);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print("Cancelar");
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 13);
    }
  }
  
  return 0;
}