// Archivo .cpp de la clase datos del periférico

#include "datosPer.h"

// Constructor
datosPer::datosPer(bool emg, bool luz, float sen1, float sen2, float sen3) {
  emergencia = emg;
  estadoLuz = luz;
  sensor1 = sen1;
  sensor2 = sen2;
  sensor3 = sen3;
}