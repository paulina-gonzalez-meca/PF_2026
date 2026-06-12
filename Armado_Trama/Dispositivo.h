#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <Arduino.h>

class dispositivo {
  private:
    String apodoDispositivo, apodoSensor1, apodoSensor2, apodoSensor3;
    String resultadoSensor1, resultadoSensor2, resultadoSensor3;
    bool emergencia;

  public:
    dispositivo(String disp, bool emg, String sen1, String sen2, String sen3, String res1, String res2, String res3);

    String getApodoDispositivo() const { return apodoDispositivo; }
    bool getEmergencia() const { return emergencia; }
    String getApodoSensor1() const { return apodoSensor1; }
    String getApodoSensor2() const { return apodoSensor2; }
    String getApodoSensor3() const { return apodoSensor3; }
    String getResultadoSensor1() const { return resultadoSensor1; }
    String getResultadoSensor2() const { return resultadoSensor2; }
    String getResultadoSensor3() const { return resultadoSensor3; }
};

#endif