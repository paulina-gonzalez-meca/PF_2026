// Código de prueba armado con I.A.

#include "Dispositivo.h"

#define BAUD 115200

// Función que recibe el objeto y retorna el String con la trama
String armarTrama(const dispositivo &d) {
    String trama = "#";
    trama += d.getApodoDispositivo() + ",";
    trama += String(d.getEmergencia()) + ","; // Convertimos el bool a "1" o "0"
    trama += d.getApodoSensor1() + ",";
    trama += d.getApodoSensor2() + ",";
    trama += d.getApodoSensor3() + ",";
    trama += d.getResultadoSensor1() + ",";
    trama += d.getResultadoSensor2() + ",";
    trama += d.getResultadoSensor3() + "*";
    
    return trama;
}

void setup() {
    Serial.begin(BAUD);

    // Creamos un objeto de prueba
    dispositivo miDispositivo("P1", 0, "S1", "S2", "S3", "-", "-", "-");

    // Llamamos a la función pasándole nuestro objeto
    String resultado = armarTrama(miDispositivo);

    // Imprimimos el resultado en el monitor serie
    Serial.println(resultado); 
    // Imprimirá: #Disp01,1,Temp,Hum,Pres,25C,60%,1013hPa
}

void loop() {
    // Vacío por ahora
}