#include <HardwareSerial.h>

HardwareSerial sim800l(2);

#define PIN_CORTE_LUZ 2    // GPIO 2
#define DEBOUNCE_TIME 100  // ms 

// Interrupciones
volatile bool cambioDetectado = false;
volatile unsigned long ultimaInterrupcion = 0;
int estadoPinActual = HIGH;

// Máquina de estados SMS
enum EstadoSMS {
  SMS_IDLE,
  SMS_ENVIAR_CMGF,
  SMS_ESPERAR_CMGF,
  SMS_ENVIAR_CMGS,
  SMS_ESPERAR_CMGS,
  SMS_ENVIAR_TEXTO,
  SMS_ESPERAR_ENVIO
} EstadosSMS_t;

EstadoSMS_t estadoEnvio = SMS_IDLE;
unsigned long timerSMS = 0;
String mensajePendiente = "";
String mensajeEnCola = "";  // Para no perder avisos si ocurren durante un envío
String respuestaSIM = "";

void solicitarEnvioSMS(String mensaje) {
  if (estadoEnvio == SMS_IDLE) {
    mensajePendiente = mensaje;
    estadoEnvio = SMS_ENVIAR_CMGF;
    Serial.println(">> Iniciando secuencia de envío SMS...");
  } else {
    // Si la máquina está ocupada, guardamos el último evento en cola
    mensajeEnCola = mensaje;
    Serial.println(">> MÓDEM OCUPADO: Mensaje guardado en cola.");
  }
}

void procesarEnvioSMS() {
  // Leer respuestas del SIM800L
  while (sim800l.available() > 0) {
    char c = sim800l.read();
    respuestaSIM += c;
    Serial.write(c);  // Monitoreo en consola
  }

  switch (estadoEnvio) {

    case SMS_IDLE:
      // Si quedó un mensaje pendiente en cola mientras el módem estaba ocupado, enviarlo
      if (mensajeEnCola != "") {
        mensajePendiente = mensajeEnCola;
        mensajeEnCola = "";
        estadoEnvio = SMS_ENVIAR_CMGF;
        Serial.println(">> Procesando mensaje guardado en cola...");
      } else {
        respuestaSIM = "";
      }
      break;

    case SMS_ENVIAR_CMGF:
      respuestaSIM = "";
      sim800l.println("AT+CMGF=1");
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_CMGF;
      break;

    case SMS_ESPERAR_CMGF:
      if (respuestaSIM.indexOf("OK") != -1) {
        estadoEnvio = SMS_ENVIAR_CMGS;
      } else if (millis() - timerSMS >= 1000) {  // Timeout de 1 seg
        Serial.println("\n>> ERROR: No respondió AT+CMGF=1");
        estadoEnvio = SMS_IDLE;
      }
      break;

    case SMS_ENVIAR_CMGS:
      respuestaSIM = "";
      sim800l.println("AT+CMGS=\"+5491123692363\"");
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_CMGS;
      break;

    case SMS_ESPERAR_CMGS:
      // Solo avanza si RECIBE el prompt '>'
      if (respuestaSIM.indexOf(">") != -1) {
        estadoEnvio = SMS_ENVIAR_TEXTO;
      } else if (millis() - timerSMS >= 3000) {  // Timeout de 3 seg sin recibir '>'
        Serial.println("\n>> ERROR: Timeout esperando '>' del SIM800L.");
        estadoEnvio = SMS_IDLE;
      }
      break;

    case SMS_ENVIAR_TEXTO:
      respuestaSIM = "";
      sim800l.print(mensajePendiente);
      sim800l.write(0x1A);  // CTRL+Z
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_ENVIO;
      break;

    case SMS_ESPERAR_ENVIO:
      if (respuestaSIM.indexOf("OK") != -1) {
        Serial.println("\n>> ¡SMS ENVIADO CON ÉXITO!");
        estadoEnvio = SMS_IDLE;
      } else if (millis() - timerSMS >= 10000) {  // Timeout de 10 seg
        Serial.println("\n>> TIMEOUT: El SIM800L no confirmó el envío.");
        estadoEnvio = SMS_IDLE;
      }
      break;
  }
}

// Interrupción (ISR)
void IRAM_ATTR ISR_CambioEnergia() {
  unsigned long tiempoActual = millis();
  if (tiempoActual - ultimaInterrupcion > DEBOUNCE_TIME) {
    cambioDetectado = true;
    ultimaInterrupcion = tiempoActual;
  }
}

void setup() {
  Serial.begin(115200);
  sim800l.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(PIN_CORTE_LUZ, INPUT_PULLUP);
  estadoPinActual = digitalRead(PIN_CORTE_LUZ);

  Serial.println("Iniciando sistema de monitoreo...");

  attachInterrupt(digitalPinToInterrupt(PIN_CORTE_LUZ), ISR_CambioEnergia, CHANGE);
}

void loop() {
  // 1. Procesar máquina de estados del SIM800L
  procesarEnvioSMS();

  // 2. Manejo de interrupciones
  if (cambioDetectado) {
    cambioDetectado = false;
    int nuevoEstado = digitalRead(PIN_CORTE_LUZ);

    if (nuevoEstado != estadoPinActual) {
      estadoPinActual = nuevoEstado;

      if (estadoPinActual == LOW) {
        Serial.println("¡CAMBIO DETECTADO! Corte de luz");
        solicitarEnvioSMS("ALERTA: Se ha cortado la luz.");
      } else {
        Serial.println("¡CAMBIO DETECTADO! Luz restablecida");
        solicitarEnvioSMS("AVISO: La luz ha vuelto.");
      }
    }
  }
}