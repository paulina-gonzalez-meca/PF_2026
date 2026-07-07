#ifndef PROTOCOLOENVIOPERIFERICO_H
#define PROTOCOLOENVIOPERIFERICO_H

#include <Arduino.h>
// Se hizo una nueva clase para la función de envío periférico central

class protocolo {
private:
  String apodoPeriferico, tramaFinal;

public:

// Constructor sin parámetros para poder crear objetos de esta clase vacíos.
  protocolo();

// Constructor con parámetros.
  protocolo(String apodoPeriferico, String tramaFinal);

  String getTramaFinal() const {
    return tramaFinal;
  }
  String getApodoPeriferico() const {
    return apodoPeriferico;
  }

};

#endif