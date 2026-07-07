// INCOMPLETO: Código de armado de tramas de los periféricos.

// Incluir librerías y clases

#include <vector>     // para incluir vectores
#include <algorithm>  // para trabajar con los vectores en sí

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
  LUZ,
  SENSOR_1,
  SENSOR_2,
  SENSOR_3,
  ARMADO_ENVIADO
} ESTADOSARMADOPERIFERICO_t;

// Definición del estado inicial de la máquina de estados.
ESTADOSARMADOPERIFERICO_t estadosArmadoPeriferico = EMG;

// Creación de objeto con los datos del periférico:
per perifericoEjemplo("A", "Heladera", "Temperatura1", "-", "-", "Temperatura", "-", "-");
String nombrePeriferico = perifericoEjemplo.nombreInterno;
// Creación de la variable para guardar el String
String tramaPerifericoFinal = "";

// Creación de la lista que guardará los pack de datos de LECTURAS.
std::vector<datosPer> filaDatosPer;

// Ejemplo de la trama que se va a buscar armar: "trama: #EMG,ApodoDisp,Luz,ResSens1,ResSens2,ResSens3*"

// Prototipado de las funciones.
String armadoTramaPeriferico(std::vector<datosPer> &fila);

//Setup
void setup() {
  Serial.begin(115200);

  // Ejemplos de lecturas obtenidas (Emergencia,luz,sensor1,sensor2,sensor3)
  filaDatosPer.push_back(datosPer(0, 1, 20, 0, 0));
  filaDatosPer.push_back(datosPer(0, 1, 44, 0, 0));
  filaDatosPer.push_back(datosPer(1, 1, 20, 0, 0));
}

// Loop
void loop() {
  // Le pasamos la fila de datos a la función (Esto se hace en microsengundos).
  tramaPerifericoFinal = armadoTramaPeriferico(filaDatosPer);

  if (tramaPerifericoFinal != "")  //Si hay una trama con datos reales (distinto de vacío), imprimir
  {
    Serial.print("Trama final: ");
    Serial.println(tramaPerifericoFinal);
    Serial.print("Nombre del periférico: ");
    Serial.println(nombrePeriferico);
    Serial.println("---");
  }
}

// Código de la máquina de estados principal
String armadoTramaPeriferico(std::vector<datosPer> &fila) {

  // Declaramos la variable en donde estará la trama que será enviada.
  static String tramaFinal = "";

  // No se entra a la máquina de estados hasta que la lista deje de estar vacía.
  if (fila.empty()) {
    return "";  // Devuelve string vacío
  }

  // Protocolo de reordenamiento de aquellos objetos con emergencias.
  if (estadosArmadoPeriferico == EMG) {
    std::stable_partition(fila.begin(), fila.end(), [](datosPer &d) {
      return d.getEmergencia() == 1;
    });
  }

  switch (estadosArmadoPeriferico) {

    // caso de emergencia
    case EMG:

      // Si la emergencia da un valor válido
      if (fila.front().getEmergencia() == 0 || fila.front().getEmergencia() == 1) {

        // Modificamos la trama para que sea la siguiente: "#EMG,NombrePeriferico"
        // El nombre del periférico lo sacamos de una variable.
        tramaFinal = "#" + String(fila.front().getEmergencia()) + "," + nombrePeriferico + ",";
        estadosArmadoPeriferico = LUZ;
      } else {
        // Devuelve error si no hay 1 o 0
        return ERROR;
      }
      break;

    case LUZ:

      if (fila.front().getLuz() == 0 || fila.front().getLuz() == 1) {

        // Modificamos la trama para que sea la siguiente: "#EMG,NombrePeriferico"
        // El nombre del periférico lo sacamos de una variable.
        tramaFinal += String(fila.front().getLuz()) + ",";
        estadosArmadoPeriferico = SENSOR_1;
      } else {
        // Devuelve error si no hay 1 o 0
        return ERROR;
      }
      break;

    // caso del primer sensor
    case SENSOR_1:

      tramaFinal += String(fila.front().getSensor1()) + ",";
      estadosArmadoPeriferico = SENSOR_2;
      break;

    // caso del segundo sensor
    case SENSOR_2:

      tramaFinal += String(fila.front().getSensor2()) + ",";
      estadosArmadoPeriferico = SENSOR_3;
      break;

    // caso del tercer sensor
    case SENSOR_3:
      tramaFinal += String(fila.front().getSensor3()) + "*";
      estadosArmadoPeriferico = ARMADO_ENVIADO;

      break;

    // Caso armado envío
    case ARMADO_ENVIADO:
      String tramaCompleta = tramaFinal;
      tramaFinal = "";                // Limpiamos el contenedor estático para la siguiente trama
      fila.erase(fila.begin());       // Eliminamos el primer elemento (Fila/Queue) ya procesado
      estadosArmadoPeriferico = EMG;  // Reiniciamos la máquina de estados al principio
      return tramaCompleta;           // Devolvemos la trama exitosa
  }

  return ""; // Devuelve vacío en los estados intermedios.
}
