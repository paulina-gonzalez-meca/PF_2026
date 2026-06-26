#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <Arduino.h>

class dispositivo {
  private:
    String apodoDispositivo, apodoSensor1, apodoSensor2, apodoSensor3, comando;
    String resultadoSensor1, resultadoSensor2, resultadoSensor3;
    String emergencia; // se modificó a que emergencia sea un String

  public:
    dispositivo(String cmd, String disp, String emg, String sen1, String sen2, String sen3, String res1, String res2, String res3);
    String getComando() const { return comando; }
    String getApodoDispositivo() const { return apodoDispositivo; }
    String getEmergencia() const { return emergencia; } 
    String getApodoSensor1() const { return apodoSensor1; }
    String getApodoSensor2() const { return apodoSensor2; }
    String getApodoSensor3() const { return apodoSensor3; }
    String getResultadoSensor1() const { return resultadoSensor1; }
    String getResultadoSensor2() const { return resultadoSensor2; }
    String getResultadoSensor3() const { return resultadoSensor3; }
};

#endif