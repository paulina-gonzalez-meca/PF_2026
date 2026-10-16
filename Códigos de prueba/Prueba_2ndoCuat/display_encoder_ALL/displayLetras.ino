int displayLetras(char nombre[], int &indiceNombre){
  if(flagSWEncoder){
    if(indiceNombre < 4 && valorDisplay < 27){
      nombre[indiceNombre] = letras[valorDisplay - 1];
      indiceNombre ++;
      nombre[indiceNombre] = '\0';
    }
    else if(valorDisplay == 27 && indiceNombre > 0){
      indiceNombre --;
      nombre[indiceNombre] = '\0';
    }
    else if(valorDisplay == 28 && nombre[3] != '\0'){
      flagSWEncoder = 0;
      return 1;
    }
    else if(valorDisplay == 29){
      flagSWEncoder = 0;
      return 2;
    }
    flagSWEncoder = 0;
  }
  tft.setCursor( 10, 10);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.print(nombre[0]);
  tft.print(nombre[1]);
  tft.print(nombre[2]);
  tft.print(nombre[3]);
  if(valorDisplay >= 1 && valorDisplay <= 9){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor( valorDisplay * ANCHO + 10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print(letras[valorDisplay - 1]);
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor(valorDisplay * ANCHO + 10, 35);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(letras[valorDisplay - 1]);
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 29)){
      tft.setCursor( valorDisplay * ANCHO + 10, 35);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print(letras[valorDisplay - 1]);
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 29);
    }
  }
  else if(valorDisplay >= 10 && valorDisplay <= 18){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor(valorDisplay * ANCHO - 152, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print(letras[valorDisplay - 1]);
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor(valorDisplay * ANCHO - 152, 60);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(letras[valorDisplay - 1]);
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 29)){
      tft.setCursor(valorDisplay * ANCHO - 152, 60);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print(letras[valorDisplay - 1]);
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 29);
    }
  }
  else if(valorDisplay >= 19 && valorDisplay <= 26){
    if(parpadeo >= TIEMPO_PARPADEO){
      if(seleccionar){
        tft.setCursor(valorDisplay * ANCHO - 314, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print(letras[valorDisplay - 1]);
        seleccionar = 0;
        parpadeo = 0;
      }
      else{
        tft.setCursor(valorDisplay * ANCHO - 314, 85);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.print(letras[valorDisplay - 1]);
        seleccionar = 1;
        parpadeo = 0;
      }
    }
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 29)){
      tft.setCursor(valorDisplay * ANCHO  - 314, 85);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print(letras[valorDisplay - 1]);
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 29);
    }
  }
  else if(valorDisplay == 27){
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
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 29)){
      tft.setCursor( 28, 120);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print("Borrar");
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 29);
    }
  }
  else if(valorDisplay == 28){
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
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 29)){
      tft.setCursor( 28, 145);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print("Confirmar");
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 29);
    }
  }
  else if(valorDisplay == 29){
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
    if(cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, 29)){
      tft.setCursor( 28, 170);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(3);
      tft.print("Cancelar");
      cambioEncoder = 0;
      parpadeo = TIEMPO_PARPADEO;
      seleccionar = 0;
      valorDisplay = map(valorEncoder, 0, 80, 1, 29);
    }
  }
  
  return 0;
}