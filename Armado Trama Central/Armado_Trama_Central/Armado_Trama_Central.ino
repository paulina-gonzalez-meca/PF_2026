// CÓDIGO DE ARMADO Y ENVÍO DE TRAMAS PARA EL CENTRAL A LOS PERIFÉRICOS

/*
Se volvió a pensar el código de decodificador de periféricos para que este solo sea utilizado en el central, a diferencia del anterior que era aplicable tanto para periféricos
como para centrales, pero esto no era conveniente ya que en ambos casos iban a quedar cosas de más.
*/

#include <vector>
#include <algorithm>

#include "Dispositivo.h"
#include "protocoloEnvioPeriferico.h"

// definir el caracter que representa que devolvió error.
#define ERROR "%"

// Nuevos estados. Los estados de sensores ahora sirven para lectura
typedef enum {

  DISPOSITIVO,
  SENSOR1,
  SENSOR2,
  SENSOR3,
  ARMADO_ENVIADO
} ESTADOSARMADO_t;

bool flagArmadoTrama = 0;

// LISTA DE DISPOSITIVOS
std::vector<dispositivo> filaDispositivos;
ESTADOSARMADO_t estadoArmado = DISPOSITIVO;

// LISTAS DE DATOS (APODOS) (Después estarán en la base de datos)
std::vector<String> listaApodosDispositivos = { "A", "B", "C" };                        // Lista de apodos de periféricos/dispositivos
std::vector<String> listaModosSensores = { "L", "-" };  // Lista de modo para lectura sensores. "L" corresponde a que habrá lectura, mientras que "-" indica que no habrá lectura/no hay sensor presente en ese lugar


void setup() {
  Serial.begin(115200);


  // Casos de tramas (No deberían devolver error)
  filaDispositivos.push_back(dispositivo("A", "L", "L", "L"));  
  filaDispositivos.push_back(dispositivo("A", "L", "L", "-"));  
  filaDispositivos.push_back(dispositivo("B", "L", "-", "-"));  
  filaDispositivos.push_back(dispositivo("C", "-", "-", "-"));  
}

void loop() {

  // ... tu código donde gestionas el vector 'miFilaDeDispositivos' ...

  // 1. Llamamos a la función y guardamos el resultado directamente en una variable de tipo protocolo
  protocolo resultadoTrama = armarTrama(filaDispositivos);

  
  if (resultadoTrama.getTramaFinal() == "ERROR") {
    Serial.println("Ocurrió un error en el armado de la trama.");
  } else if (resultadoTrama.getTramaFinal() != "") {
    
    Serial.print("Periférico: ");
    Serial.println(resultadoTrama.getApodoPeriferico());
    Serial.print("Trama construida: ");
    Serial.println(resultadoTrama.getTramaFinal());
  } else {
    // Si entró aquí, es porque devolvió protocolo() vacío (sigue procesando la máquina de estados)
    //Serial.println("Trama en proceso... esperando siguientes ejecuciones.");
  }

  // delay(1000);
}

// Función que contiene la máquina de estados principal.

protocolo armarTrama(std::vector<dispositivo>& fila) {

  // Valores que serán puestos dentro del objeto que devolverá la función.
  static String perifericoActual = "";
  static String tramaActual = "";

  // Devuelve vacío si la lista está vacía.
  if (filaDispositivos.empty()) {

    return protocolo();  // Retorna un objeto protocolo vacío
  }


  switch (estadoArmado) {

    case DISPOSITIVO:

      if (std::find(listaApodosDispositivos.begin(), listaApodosDispositivos.end(), filaDispositivos.front().getApodoDispositivo()) != listaApodosDispositivos.end()) {
        perifericoActual = filaDispositivos.front().getApodoDispositivo();  // Si el primer valor del objeto corresponde al apodo de un periférico existente, lo guardamos en nuestra variable que indicará el periférico que tendrá que ser analizado.
        estadoArmado = SENSOR1;
      } else {
        // Si el primer valor correspondiente no corresponde a nada, se devuelve un objeto cuyos valores indican error.
        // Se reinicia la lectura.
        tramaActual = "";
        perifericoActual = "";
        estadoArmado = DISPOSITIVO;
        filaDispositivos.erase(filaDispositivos.begin());
        return protocolo("ERROR", "ERROR");  // Objeto devuelto.
      }

      break;

    case SENSOR1:

      if (std::find(listaModosSensores.begin(), listaModosSensores.end(), filaDispositivos.front().getSensor1()) != listaModosSensores.end()) {
        // Si el segundo valor del objeto corresponde al nombre de un sensor existente, lo guardamos en nuestra trama.
        tramaActual += "#" + filaDispositivos.front().getSensor1() + ",";
        estadoArmado = SENSOR2;
      } else {
        tramaActual = "";
        perifericoActual = "";
        estadoArmado = DISPOSITIVO;
        filaDispositivos.erase(filaDispositivos.begin());
        return protocolo("ERROR", "ERROR");
      }

      break;

    case SENSOR2:

      if (std::find(listaModosSensores.begin(), listaModosSensores.end(), filaDispositivos.front().getSensor2()) != listaModosSensores.end()) {
        tramaActual += filaDispositivos.front().getSensor2() + ",";
        estadoArmado = SENSOR3;
      } else {
        tramaActual = "";
        perifericoActual = "";
        estadoArmado = DISPOSITIVO;
        filaDispositivos.erase(filaDispositivos.begin());
        return protocolo("ERROR", "ERROR");
      }

      break;

    case SENSOR3:
      if (std::find(listaModosSensores.begin(), listaModosSensores.end(), filaDispositivos.front().getSensor3()) != listaModosSensores.end()) {
        tramaActual += filaDispositivos.front().getSensor3() + ",";
        estadoArmado = ARMADO_ENVIADO;
      } else {
        tramaActual = "";
        perifericoActual = "";
        estadoArmado = DISPOSITIVO;
        filaDispositivos.erase(filaDispositivos.begin());
        return protocolo("ERROR", "ERROR");
      }

      break;

    case ARMADO_ENVIADO:
      // Se crea el objeto que será devuelto por la función.
      protocolo protocoloPeriferico(perifericoActual, tramaActual);

      // Se eliminan las variables estáticas y se reinicia la función
      filaDispositivos.erase(filaDispositivos.begin());
      tramaActual = "";
      perifericoActual = "";
      estadoArmado = DISPOSITIVO;

      // Devolución del objeto final.
      return protocoloPeriferico;

      
  }
  return protocolo();  // se devuelve un objeto vacío mientras se está enviando una trama.
}