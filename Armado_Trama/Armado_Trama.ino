// Código de prueba hecho con IA, ahora implementando la máquina de estados.

#include "Dispositivo.h"
#include <vector>  // implenentación de la librería Vector para la lista de espera

#define ERROR % // definir el caracter que representa error.
#define BAUD 115200 // definición de BAUD

// Definición de estados
typedef enum {
  DISPOSITIVO,
  EMG,
  SENSOR1,
  SENSOR2,
  SENSOR3,
  VALOR1,
  VALOR2,
  VALOR3,
  ARMADO_ENVIADO  // Estado final para transmitir y limpiar
} ESTADOSARMADO_t;

ESTADOSARMADO_t estadoArmado = DISPOSITIVO;  // Estado inicial
String tramaActual = "";                     // Aquí se irá armando la trama paso a paso
std::vector<dispositivo> filaDispositivos;   // Nuestra fila de objetos



void setup() {
  Serial.begin(BAUD);

  // Objetos de prueba
  filaDispositivos.push_back(dispositivo("Disp_A", false, "Temp", "Hum", "Pres", "22", "50", "1012"));
  filaDispositivos.push_back(dispositivo("Disp_B", true, "Volt", "Curr", "Freq", "5", "2", "60"));  // Con emergencia
  filaDispositivos.push_back(dispositivo("Disp_C", false, "Lux", "UV", "Irrad", "300", "1", "15"));
}

void loop() {
  //if (armarTrama != ERROR) {
    String resultado = armarTrama(filaDispositivos);
    Serial.println("Se guardó la trama exitosamente");
    Serial.println(resultado);
  //} else {
   // Serial.println("error");
  //}
}

String armarTrama(std::vector<dispositivo> &fila) {
    switch (estadoArmado) {
        
        case DISPOSITIVO:
            // Si la fila está vacía, no hacemos nada y salimos del switch
            if (filaDispositivos.empty()) {
                break; 
            }
            // Si hay alguien en la fila, empezamos a armar su trama
            tramaActual = "#" + filaDispositivos.front().getApodoDispositivo() + ",";
            estadoArmado = EMG; // Siguiente estado
            break;

        case EMG:
            // Añadimos la emergencia a la trama
            tramaActual += String(filaDispositivos.front().getEmergencia()) + ",";
            estadoArmado = SENSOR1;
            break;

        case SENSOR1:
            tramaActual += filaDispositivos.front().getApodoSensor1() + ",";
            estadoArmado = SENSOR2;
            break;

        case SENSOR2:
            tramaActual += filaDispositivos.front().getApodoSensor2() + ",";
            estadoArmado = SENSOR3;
            break;

        case SENSOR3:
            tramaActual += filaDispositivos.front().getApodoSensor3() + ",";
            estadoArmado = VALOR1;
            break;

        case VALOR1:
            tramaActual += filaDispositivos.front().getResultadoSensor1() + ",";
            estadoArmado = VALOR2;
            break;

        case VALOR2:
            tramaActual += filaDispositivos.front().getResultadoSensor2() + ",";
            estadoArmado = VALOR3;
            break;

        case VALOR3:
            // El último valor no lleva coma al final
            tramaActual += filaDispositivos.front().getResultadoSensor3();
            estadoArmado = ARMADO_ENVIADO;
            break;

        case ARMADO_ENVIADO:
            // Aquí la trama está lista. La enviamos (por ejemplo, por Serial)
            Serial.print("Trama Lista y Enviada: ");
            Serial.println(tramaActual);

            // IMPORTANTE: Sacamos al dispositivo procesado de la fila
            filaDispositivos.erase(filaDispositivos.begin());

            // Reseteamos las variables para el siguiente dispositivo de la fila
            tramaActual = "";
            estadoArmado = DISPOSITIVO;
            break;
    }
}