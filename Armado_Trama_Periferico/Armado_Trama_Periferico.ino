// INCOMPLETO: Código de armado de tramas de los periféricos.

// Incluir librerías y clases

#include <vector>
#include <algorithm>

#include "datosPer.h"

// incluir constantes
#define ERROR "%"

// Definición de clase del periférico
class per {
public:
  String nombreInterno;
  String nombreExterior;
  String tipoSensor1;
  String tipoSensor2;
  String tipoSensor3;
  String nombreSensor1;
  String nombreSensor2;
  String nombreSensor3;

  per(String nombrePeriferico, String nombrePerifericoExterior, String nombreSensorExterior1, String nombreSensorExterior2, String nombreSensorExterior3, String tipoSensor1Per, String tipoSensor2Per, String tipoSensor3Per) {
    nombreInterno = nombrePeriferico;
    nombreExterior = nombrePerifericoExterior;
    nombreSensor1 = nombreSensorExterior1;
    nombreSensor2 = nombreSensorExterior2;
    nombreSensor3 = nombreSensorExterior3;
    tipoSensor1 = tipoSensor1Per;
    tipoSensor2 = tipoSensor2Per;
    tipoSensor3 = tipoSensor3Per;
  }
};

// Creación de estados para la máquina de estados principal.
typedef enum {
  EMG,
  PERIFERICO,
  SENSOR_1,
  SENSOR_2,
  SENSOR_3,
  ARMADO_ENVIADO
} ESTADOSARMADOPERIFERICO_t;

// Definición del estado inicial de la máquina de estados.
ESTADOSARMADOPERIFERICO_t estadosArmadoPeriferico = EMG;

// Creación de objeto con los datos del periférico:
per perifericoEjemplo ("A","Heladera","Temperatura1","-","-","Temperatura","-","-");

// Creación de lista que funcionará como una fila


// Ejemplo de la trama que se va a buscar armar: "trama: #EMG,ApodoDisp,ResSens1,ResSens2,ResSens3*"

//Setup
void setup() {
  Serial.begin(115200);
  datosPer pruebaInicial("0","A","20","-","-");
}

// Loop
void loop() {
  // put your main code here, to run repeatedly:
}

// Código de la máquina de estados principal
String armadoTramaPeriferico(std::vector<per> &fila) {

// Declaramos la variable en donde estará la trama que será enviada.
static String tramaFinal = "";

  switch (estadosArmadoPeriferico) {
    // caso de emergencia
    case EMG:

      break;

    // caso de periférico
    case PERIFERICO:

      break;

    // caso del primer sensor
    case SENSOR_1:

      break;

    // caso del segundo sensor
    case SENSOR_2:
      break;

    // caso del tercer sensor
    case SENSOR_3:

      break;
  }
}
