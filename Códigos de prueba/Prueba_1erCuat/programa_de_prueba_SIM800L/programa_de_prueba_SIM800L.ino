#include <HardwareSerial.h>
#include "BluetoothSerial.h"

HardwareSerial sim800l(2);

BluetoothSerial SerialBT;
String mensajeSerial;

// --------------------------------------------------
// CONFIGURACIÓN DEL PIN DE SIMULACIÓN
// --------------------------------------------------
const int PIN_CORTE_LUZ = 32;

const unsigned long DEBOUNCE_TIME = 100; // ms

int lecturaPin = HIGH;
int estadoPin = HIGH;

unsigned long ultimoCambioPin = 0;


// --------------------------------------------------
// FUNCIÓN PARA ENVIAR SMS
// --------------------------------------------------
void enviarSMS(String mensaje) {
  sim800l.println("AT+CMGS=\"+5491161386381\"");
  delay(500);

  sim800l.print(mensaje);
  delay(500);

  sim800l.write(0x1A);
  delay(5000);
}


// --------------------------------------------------
// SETUP
// --------------------------------------------------
void setup() {

  SerialBT.begin("ProbandoSim");
  sim800l.begin(115200, SERIAL_8N1, 16, 17);

  // GPIO 2 con pull-up interno
  pinMode(PIN_CORTE_LUZ, INPUT_PULLUP);

  SerialBT.println("---------------------------------------------");
  SerialBT.println("Iniciando...");

  delay(15000);


  // ------------------------------------------------
  // COMANDOS DE DIAGNÓSTICO
  // ------------------------------------------------
  sim800l.println("AT+IPR=115200");
  delay(500);
  sim800l.println("AT&W");
  delay(500);

  sim800l.println("AT");
  delay(500);

  sim800l.println("AT+CFUN?");
  delay(1000);

  sim800l.println("AT+CPIN?");
  delay(1000);

  sim800l.println("AT+CSCA?");
  delay(1000);

  sim800l.println("AT+CSQ");
  delay(2000);

  sim800l.println("AT+CREG?");
  delay(2000);


  // ------------------------------------------------
  // COMANDOS DE CONFIGURACIÓN
  // ------------------------------------------------

  sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  delay(1000);

  sim800l.println("AT+CMGF=1");
  delay(1000);

  sim800l.println("AT+CNMI=2,2,0,0,0");
  delay(1000);

  SerialBT.print(sim800l.readString());


  // ------------------------------------------------
  // ESTADO INICIAL DEL PIN
  // ------------------------------------------------

  estadoPin = digitalRead(PIN_CORTE_LUZ);
  lecturaPin = estadoPin;

  SerialBT.print("Estado inicial GPIO 2: ");

  if (estadoPin == LOW) {
    SerialBT.println("CORTE DE LUZ");
  } 
  else {
    SerialBT.println("LUZ ACTIVA");
  }
}


// --------------------------------------------------
// LOOP
// --------------------------------------------------
void loop() {

  // ==================================================
  // LECTURA DEL GPIO 2 CON DEBOUNCE
  // ==================================================

  int lecturaActual = digitalRead(PIN_CORTE_LUZ);

  // Detectamos que la lectura cambió
  if (lecturaActual != lecturaPin) {

    // Reiniciamos el timer de debounce
    ultimoCambioPin = millis();

    lecturaPin = lecturaActual;
  }

  // Si pasó el tiempo de debounce
  if ((millis() - ultimoCambioPin) >= DEBOUNCE_TIME) {

    // Si el estado estable cambió
    if (lecturaPin != estadoPin) {

      estadoPin = lecturaPin;

      if (estadoPin == LOW) {

        SerialBT.println("GPIO 2 LOW -> SIMULACION CORTE DE LUZ");

        enviarSMS("SIMULACION CORTE DE LUZ");

      } 
      else {

        SerialBT.println("GPIO 2 HIGH -> SIMULACION LUZ ACTIVA");

        enviarSMS("SIMULACION LUZ ACTIVA");
      }
    }
  }


  // ==================================================
  // ENVÍO DE MENSAJES DESDE SERIAL
  // ==================================================

  if (SerialBT.available() > 0) {

    mensajeSerial = SerialBT.readString();

    if (mensajeSerial.length() > 0) {

      if (mensajeSerial[0] == '%') {

        SerialBT.println(mensajeSerial);

        enviarSMS(mensajeSerial);

      }
      else {

        sim800l.print(mensajeSerial);
      }
    }
  }


  // ==================================================
  // RESPUESTA DEL SIM800L
  // ==================================================

  if (sim800l.available() > 0) {

    SerialBT.print(sim800l.readString());
  }
}