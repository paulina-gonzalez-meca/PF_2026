// Código de armado de tramas que funciona en base de llamado de objetos.
// En una etapa más avanzada, este será implementado en los periféricos. Estas tramas serán recibidas por el central.

#include "Dispositivo.h"
#include "Verificador.h"

#include <vector>
#include <algorithm>


#define ERROR "%"
#define BAUD 115200

typedef enum {
  COMANDO,
  DISPOSITIVO,
  EMG,
  SENSOR1,
  SENSOR2,
  SENSOR3,
  VALOR1,
  VALOR2,
  VALOR3,
  ARMADO_ENVIADO
} ESTADOSARMADO_t;

// Estado inicial de la máquina de estado
ESTADOSARMADO_t estadoArmado = COMANDO;

// CREACIÓN DE LISTAS:

// LISTA DE DISPOSITIVOS
std::vector<dispositivo> filaDispositivos;

// LISTAS DE DATOS (APODOS) (Después estarán en la base de datos)
std::vector<String> listaComandos = { "/menu", "/getdata", "-" };                                                     // Lista de comandos disponibles.
std::vector<String> listaApodosPerifericos = { "Disp_1", "Disp_2", "Disp_3" };                                        // Lista de apodos de periféricos
std::vector<String> listaApodosSensores = { "Sens_1A", "Sens_1B", "Sens_1C", "Sens_2A", "Sens_2B", "Sens_3A", "-" };  // Lista de apodos de sensores



/*

Para esta etapa de prueba, los sensores están configurados de la siguiente manera:

El primer periférico contiene 3 Sensores

El segundo periférico contiene 2 Sensores

El tercer periférico contiene 1 Sensor

*/


// Prototipo de las funciones
String armarTrama(std::vector<dispositivo> &fila);


// Setup
void setup() {
  Serial.begin(BAUD);

  // Casos de prueba válidos
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "0", "Sens_1A", "Sens_1B", "Sens_1C", "22", "50", "-1012"));  // SIN EMERGENCIA
  filaDispositivos.push_back(dispositivo("/menu", "Disp_2", "1", "Sens_2A", "Sens_2B", "-", "5", "2", "-"));          // CON EMERGENCIA

  // CASOS DE PRUEBA DE ERROR
  filaDispositivos.push_back(dispositivo("dfdsff", "Disp_3", "0", "Sens_3A", "-", "-", "1", "-", "-"));              // Error en Comando
  filaDispositivos.push_back(dispositivo("-", "APODO", "0", "Sens_1A", "Sens_1B", "Sens_1C", "22", "50", "1012"));   // Error en apodo disp
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "Q", "Sens_1A", "Sens_1B", "Sens_1C", "22", "50", "1012"));  // error en emergencia
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "0", "Sens_1", "Sens_1B", "Sens_1C", "22", "50", "1012"));   // error en apodo 1
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "0", "Sens_1A", "Sens_q", "Sens_1C", "22", "50", "1012"));   // error en apodo 2
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "0", "Sens_1A", "Sens_1B", "Sens_s", "22", "50", "1012"));   // error en apodo 3
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "0", "Sens_1A", "Sens_1B", "Sens_1C", "A", "50", "1012"));   // error en valor 1
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "0", "Sens_1A", "Sens_1B", "Sens_1C", "22", "B", "1012"));   // error en valor 2
  filaDispositivos.push_back(dispositivo("-", "Disp_1", "0", "Sens_1A", "Sens_1B", "Sens_1C", "22", "50", "-C"));    // error en valor 3
}

void loop() {
  if (!filaDispositivos.empty()) {

    // 1. Revisa si hay emergencias atrás en la fila y las mueve al frente
    aplicarPrioridadEmergencias(filaDispositivos);

    // 2. Procesa el elemento que quedó al frente (sea prioritario o normal)
    String resultado = armarTrama(filaDispositivos);

    if (resultado != "") {
      if (resultado != ERROR) {
        Serial.println("\n[ÉXITO] Se guardó la trama exitosamente:");
        Serial.println(resultado);
      } else {
        Serial.println("\n[ERROR] Trama corrupta descartada.");
      }
    }
  }
}


String armarTrama(std::vector<dispositivo> &fila) {
  // 'static' para que no se destruyan ni pierdan su valor entre llamadas de la función
  static String tramaActual = "";

  if (fila.empty()) return "";

  switch (estadoArmado) {

    case COMANDO:
      if (std::find(listaComandos.begin(), listaComandos.end(), fila.front().getComando()) != listaComandos.end()) {
        tramaActual = "#" + fila.front().getComando() + ",";
        estadoArmado = DISPOSITIVO;
      } else {
        // Si el comando está mal, limpiamos, removemos de la fila y devolvemos ERROR
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case DISPOSITIVO:
      // Verificamos el apodo del dispositivo en la lista correcta
      if (std::find(listaApodosPerifericos.begin(), listaApodosPerifericos.end(), fila.front().getApodoDispositivo()) != listaApodosPerifericos.end()) {
        tramaActual += fila.front().getApodoDispositivo() + ",";
        estadoArmado = EMG;
      } else {
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case EMG:{
              // Guardamos el texto de la emergencia
      String valorEmg = fila.front().getEmergencia();

      // Validamos si el String coincide con un formato booleano real
      if (valorEmg == "true" || valorEmg == "false" || valorEmg == "1" || valorEmg == "0") {
        tramaActual += valorEmg + ",";
        estadoArmado = SENSOR1;  // Avanza si es válido
      } else {
        // PROTOCOLO DE ERROR: Si llegó "dfdsff" o cualquier basura, limpiamos y descartamos
        tramaActual = "";
        fila.erase(fila.begin());  // Sacamos el dispositivo corrupto de la fila
        return ERROR;              // Devolvemos "%"
      }
      break;
    }



    case SENSOR1:
      if (std::find(listaApodosSensores.begin(), listaApodosSensores.end(), fila.front().getApodoSensor1()) != listaApodosSensores.end()) {
        tramaActual += fila.front().getApodoSensor1() + ",";
        estadoArmado = SENSOR2;
      } else {
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case SENSOR2:
      if (std::find(listaApodosSensores.begin(), listaApodosSensores.end(), fila.front().getApodoSensor2()) != listaApodosSensores.end()) {
        tramaActual += fila.front().getApodoSensor2() + ",";
        estadoArmado = SENSOR3;
      } else {
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case SENSOR3:
      if (std::find(listaApodosSensores.begin(), listaApodosSensores.end(), fila.front().getApodoSensor3()) != listaApodosSensores.end()) {
        tramaActual += fila.front().getApodoSensor3() + ",";
        estadoArmado = VALOR1;
      } else {
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case VALOR1:
      if (esNumerico(fila.front().getResultadoSensor1())) {
        tramaActual += fila.front().getResultadoSensor1() + ",";
        estadoArmado = VALOR2;
      } else {
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case VALOR2:
      if (esNumerico(fila.front().getResultadoSensor2())) {
        tramaActual += fila.front().getResultadoSensor2() + ",";
        estadoArmado = VALOR3;
      } else {
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case VALOR3:
      if (esNumerico(fila.front().getResultadoSensor3())) {
        tramaActual += fila.front().getResultadoSensor3();
        estadoArmado = ARMADO_ENVIADO;
      } else {
        tramaActual = "";
        estadoArmado = COMANDO;
        fila.erase(fila.begin());
        return ERROR;
      }
      break;

    case ARMADO_ENVIADO:
      String tramaFinalizada = tramaActual;

      // PRIMERO limpiamos los estados y removemos de la fila
      fila.erase(fila.begin());
      tramaActual = "";
      estadoArmado = COMANDO;

      // SEGUNDO devolvemos la trama finalizada
      return tramaFinalizada;
  }

  return "";  // Retorna vacío si la trama aún está en proceso de armado
}

void aplicarPrioridadEmergencias(std::vector<dispositivo> &fila) {
  // Regla de oro: SOLO reordenamos si la máquina de estados está lista para empezar
  // un nuevo dispositivo (COMANDO) y si hay elementos suficientes para comparar.
  if (estadoArmado != COMANDO || fila.size() <= 1) {
    return; 
  }

  // Buscamos desde el segundo elemento (índice 1) en adelante.
  // Si el primero (índice 0) ya tuviera emergencia, ya está en su lugar.
  for (size_t i = 1; i < fila.size(); i++) {
    String emg = fila[i].getEmergencia();
    
    if (emg == "true" || emg == "1") {
      Serial.print("\n🚨 [PROTOCOLO DE EMERGENCIA] Saltando fila para: ");
      Serial.println(fila[i].getApodoDispositivo());

      // 1. Clonamos temporalmente el dispositivo con emergencia
      dispositivo dispPrioritario = fila[i];
      
      // 2. Lo borramos de su posición actual en la fila
      fila.erase(fila.begin() + i);
      
      // 3. Lo insertamos al principio de todo (índice 0)
      fila.insert(fila.begin(), dispPrioritario);
      
      break; // Salimos del bucle; ya pusimos la emergencia más vieja al frente
    }
  }
}