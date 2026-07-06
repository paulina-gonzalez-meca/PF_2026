// INCOMPLETO: Código de armado de tramas de los periféricos.

// Incluir librerías.

#include <vector>
#include <algorithm>

// Creación de estados para la máquina de estados principal.
typedef enum {  
  EMG,
  PERIFERICO,
  SENSOR1,
  SENSOR2,
  SENSOR3,
  ARMADO_ENVIADO
} ESTADOSARMADOPERIFERICO_t;

// Definición del estado inicial de la máquina de estados.
ESTADOSARMADOPERIFERICO_t estadosArmadoPeriferico = EMG;


// Ejemplo de la trama que se va a buscar armar: "trama: #EMG,ApodoDisp,ResSens1,ResSens2,ResSens3*"

//Setup
void setup() {
  Serial.begin(115200);

}

// Loop
void loop() {
  // put your main code here, to run repeatedly:

}
