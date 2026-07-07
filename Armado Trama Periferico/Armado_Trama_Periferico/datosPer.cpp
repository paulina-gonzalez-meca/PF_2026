// Archivo .cpp de la clase datos del periférico

#include "datosPer.h"

// Constructor
datosPer::datosPer(int emg, int luz, int sen1, int sen2, int sen3) {
  emergencia = emg;
  estadoLuz = luz;
  sensor1 = sen1;
  sensor2 = sen2;
  sensor3 = sen3;
}