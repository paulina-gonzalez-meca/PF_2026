#include "Dispositivo.h" // <-- Ojo que coincida con el nombre del archivo

dispositivo::dispositivo(String cmd, String disp, String emg, String sen1, String sen2, String sen3, String res1, String res2, String res3) {
    comando = cmd;
    apodoDispositivo = disp;
    emergencia = emg;
    apodoSensor1 = sen1;
    apodoSensor2 = sen2;
    apodoSensor3 = sen3;
    resultadoSensor1 = res1;
    resultadoSensor2 = res2;
    resultadoSensor3 = res3;
}