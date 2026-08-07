#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <Arduino.h>
// Se repensó la clase de dispositivo. Ahora los espacios para sensores admiten tanto lectura como envío de datos.
// El apodo del dispositivo, en este caso, corresponderá a un periférico.

class dispositivo {
private:
  String apodoDispositivo, sensor1, sensor2, sensor3;

public:
  dispositivo(String disp, String sen1, String sen2, String sen3);

  String getApodoDispositivo() const {
    return apodoDispositivo;
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