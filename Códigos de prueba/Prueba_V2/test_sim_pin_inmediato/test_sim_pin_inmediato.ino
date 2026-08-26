#include <HardwareSerial.h>

HardwareSerial sim800l(2);

#define PIN_CORTE_LUZ 2
#define DEBOUNCE_TIME 100 // ms

// Interrupciones
volatile bool cambioDetectado = false;
volatile unsigned long ultimaInterrupcion = 0;
int estadoPinActual = HIGH;

// --------------------------------------------------
// MÁQUINA DE ESTADOS PARA SMS NO BLOQUEANTE
// --------------------------------------------------
enum EstadoSMS {
  SMS_IDLE,
  SMS_ENVIAR_CMGF,
  SMS_ESPERAR_CMGF,
  SMS_ENVIAR_CMGS,
  SMS_ESPERAR_CMGS,
  SMS_ENVIAR_TEXTO,
  SMS_ESPERAR_ENVIO
};

EstadoSMS estadoEnvio = SMS_IDLE;
unsigned long timerSMS = 0;
String mensajePendiente = "";
String respuestaSIM = "";

// Función para solicitar un envío (se llama sin bloquear)
void solicitarEnvioSMS(String mensaje) {
  if (estadoEnvio == SMS_IDLE) {
    mensajePendiente = mensaje;
    estadoEnvio = SMS_ENVIAR_CMGF;
    Serial.println(">> Iniciando secuencia de envío SMS sin bloqueo...");
  } else {
    Serial.println(">> ERROR: Hay un SMS en proceso. Intento descartado.");
  }
}

// Tarea periódica de control del SIM800L (se ejecuta continuo en el loop)
void procesarEnvioSMS() {
  
  // Leer respuestas entrantes del SIM800L de forma asíncrona
  while (sim800l.available() > 0) {
    char c = sim800l.read();
    respuestaSIM += c;
    Serial.write(c); // Monitoreo en consola
  }

  switch (estadoEnvio) {

    case SMS_IDLE:
      // Nada pendiente por hacer
      respuestaSIM = "";
      break;

    case SMS_ENVIAR_CMGF: // enviar CMGF
      respuestaSIM = "";
      sim800l.println("AT+CMGF=1");
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_CMGF;
      break;

    case SMS_ESPERAR_CMGF:
      // Avanza si recibe OK o por timeout (300 ms)
      if (respuestaSIM.indexOf("OK") != -1 || (millis() - timerSMS >= 300)) {
        estadoEnvio = SMS_ENVIAR_CMGS;
      }
      break;

    case SMS_ENVIAR_CMGS:
      respuestaSIM = "";
      sim800l.println("AT+CMGS=\"+5491123692363\"");
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_CMGS;
      break;

    case SMS_ESPERAR_CMGS:
      // Avanza al recibir el prompt '>' o por timeout (1000 ms)
      if (respuestaSIM.indexOf(">") != -1 || (millis() - timerSMS >= 1000)) {
        estadoEnvio = SMS_ENVIAR_TEXTO;
      }
      break;

    case SMS_ENVIAR_TEXTO:
      respuestaSIM = "";
      sim800l.print(mensajePendiente);
      sim800l.write(0x1A); // CTRL+Z para enviar
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_ENVIO;
      break;

    case SMS_ESPERAR_ENVIO:
      // El SIM800L puede tardar unos segundos en confirmar con +CMGS: o OK
      if (respuestaSIM.indexOf("OK") != -1) {
        Serial.println("\n>> ¡SMS ENVIADO CON ÉXITO!");
        estadoEnvio = SMS_IDLE;
      } 
      // Timeout de seguridad por si falla la red (10 segundos)
      else if (millis() - timerSMS >= 10000) {
        Serial.println("\n>> TIMEOUT: El SIM800L no confirmó el envío.");
        estadoEnvio = SMS_IDLE;
      }
      break;
  }
}

// --------------------------------------------------
// INTERRUPCIÓN (ISR)
// --------------------------------------------------
void IRAM_ATTR ISR_CambioEnergia() {
  unsigned long tiempoActual = millis();
  if (tiempoActual - ultimaInterrupcion > DEBOUNCE_TIME) {
    cambioDetectado = true;
    ultimaInterrupcion = tiempoActual;
  }
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------
void setup() {
  Serial.begin(115200);
  sim800l.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(PIN_CORTE_LUZ, INPUT_PULLUP);
  estadoPinActual = digitalRead(PIN_CORTE_LUZ);

  Serial.println("Iniciando sistema...");

  // Configuración de la interrupción
  attachInterrupt(digitalPinToInterrupt(PIN_CORTE_LUZ), ISR_CambioEnergia, CHANGE);
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------
void loop() {

  // 1. Procesar la máquina de estados del SMS
  procesarEnvioSMS();

  // 2. Manejo de la interrupción del Pin de Energía
  if (cambioDetectado) {
    cambioDetectado = false;
    int nuevoEstado = digitalRead(PIN_CORTE_LUZ);

    if (nuevoEstado != estadoPinActual) {
      estadoPinActual = nuevoEstado;

      if (estadoPinActual == LOW) {
        Serial.println("¡CAMBIO DETECTADO! GPIO 2 LOW");
        solicitarEnvioSMS("SIMULACION CORTE DE LUZ");
      } else {
        Serial.println("¡CAMBIO DETECTADO! GPIO 2 HIGH");
        solicitarEnvioSMS("SIMULACION LUZ ACTIVA");
      }
    }
  }

  // 3. El microcontrolador queda libre para ejecutar otras tareas concurrentes aquí sin trabarse...
}