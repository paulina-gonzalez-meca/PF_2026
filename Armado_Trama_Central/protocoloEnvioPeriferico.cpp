#include "protocoloEnvioPeriferico.h"

// Constructor vacío
protocolo::protocolo(){
  apodoPeriferico = "";
  tramaFinal = "";
}

// Constructor con parámetros
protocolo::protocolo(String per, String trama) {
    apodoPeriferico = per;
    tramaFinal = trama;
    }