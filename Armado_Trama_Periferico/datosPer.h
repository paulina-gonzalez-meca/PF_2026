// Archivo de encabezado para la clase de datos del periférico
// Esta clase podrá guardar: emergencia, apodo del periférico y los resultados de los sensores

#ifndef DATOSPER_H
#define DATOSPER_H

#include <Arduino.h>

class datosPer {
private:
  String emergencia, apodoPeriferico, sensor1, sensor2, sensor3;

public:
  datosPer(String emg, String periferico, String sen1, String sen2, String sen3);

  String getEmergencia() const {
    return emergencia;
  }
  String getApodoPeriferico() const {
    return apodoPeriferico;
  }
  String getSensor1() const {
    return sensor1;
  }
  String getSensor2() const {
    return sensor2;
  }
  String getSensor3() const {
    return sensor3;
  }
};

#endif