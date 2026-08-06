// Archivo de encabezado para la clase de datos del periférico
// Esta clase podrá guardar todo lo que sea lecturas del periférico: emergencias, luz y resultados de los sensores.

#ifndef DATOSPER_H
#define DATOSPER_H

#include <Arduino.h>

class datosPer {
private:
  bool emergencia, estadoLuz;
  float sensor1, sensor2, sensor3;

public:
  datosPer(bool emg, bool luz, float sen1, float sen2, float sen3);

  int getEmergencia() const {
    return emergencia;
  }
  int getLuz() const {
    return estadoLuz;
  }
  int getSensor1() const {
    return sensor1;
  }
  int getSensor2() const {
    return sensor2;
  }
  int getSensor3() const {
    return sensor3;
  }
};

#endif