bool eliminarUs(matriz miListaUs[], int tamListaUs, int &indiceEliminarUs){
  if(indiceEliminarUs < tamListaUs && miListaUs[indiceEliminarUs].nombreUs[0] != '\0'){
    strcpy(miListaUs[indiceEliminarUs - 1].numeroUs, miListaUs[indiceEliminarUs].numeroUs);
    strcpy(miListaUs[indiceEliminarUs - 1].nombreUs, miListaUs[indiceEliminarUs].nombreUs);
    miListaUs[indiceEliminarUs - 1].indiceNumeroUs = miListaUs[indiceEliminarUs].indiceNumeroUs;
    miListaUs[indiceEliminarUs - 1].indiceNombreUs = miListaUs[indiceEliminarUs].indiceNombreUs;
    indiceEliminarUs ++;
    return false;
  }
  else{
    memset(miListaUs[indiceEliminarUs - 1].numeroUs, 0, sizeof(miListaUs[indiceEliminarUs - 1].numeroUs));
    memset(miListaUs[indiceEliminarUs - 1].nombreUs, 0, sizeof(miListaUs[indiceEliminarUs - 1].nombreUs));
    strcpy(miListaUs[indiceEliminarUs - 1].numeroUs, "+");
    miListaUs[indiceEliminarUs - 1].indiceNumeroUs = 1;
    miListaUs[indiceEliminarUs - 1].indiceNombreUs = 0;
    return true;
  }
}