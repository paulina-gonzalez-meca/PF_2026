#include <HardwareSerial.h>
#include <Preferences.h>

HardwareSerial sim800l(2);
Preferences preferences;

#define PIN_CORTE_LUZ 2
#define DEBOUNCE_TIME 150  // ms

// CONFIGURACIÓN DE REINTENTOS
const int MAX_REINTENTOS = 3;
const unsigned long PAUSA_REINTENTO = 1000;
int intentosActuales = 0;

volatile bool cambioDetectado = false;
volatile unsigned long ultimaInterrupcion = 0;
int estadoPinActual = HIGH;

enum EstadoSMS {
  SMS_IDLE,
  SMS_ENVIAR_CMGF,
  SMS_ESPERAR_CMGF,
  SMS_ENVIAR_CMGS,
  SMS_ESPERAR_CMGS,
  SMS_ENVIAR_TEXTO,
  SMS_ESPERAR_ENVIO,
  SMS_REINTENTAR
};

EstadoSMS estadoEnvio = SMS_IDLE;
unsigned long timerSMS = 0;
String mensajePendiente = "";
String respuestaSIM = "";

void solicitarEnvioSMS(String mensaje) {
  if (estadoEnvio == SMS_IDLE) {
    mensajePendiente = mensaje;
    intentosActuales = 0;
    estadoEnvio = SMS_ENVIAR_CMGF;
    Serial.println(">> Iniciando secuencia de envío SMS...");
  } else {
    Serial.println(">> ERROR: SMS en proceso. Intento descartado.");
  }
}

void manejarFallaSMS(String razon) {
  intentosActuales++;
  Serial.print("\n>> FALLO EN SMS (");
  Serial.print(razon);
  Serial.print("). Intento ");
  Serial.print(intentosActuales);
  Serial.print(" de ");
  Serial.println(MAX_REINTENTOS);

  if (intentosActuales < MAX_REINTENTOS) {
    sim800l.write(0x1B);
    timerSMS = millis();
    estadoEnvio = SMS_REINTENTAR;
  } else {
    Serial.println(">> ERROR CRÍTICO: Máximo de reintentos alcanzado.");
    estadoEnvio = SMS_IDLE;
  }
}

void procesarEnvioSMS() {
  while (sim800l.available() > 0) {
    char c = sim800l.read();
    respuestaSIM += c;
    Serial.write(c);
  }

  switch (estadoEnvio) {
    case SMS_IDLE:
      respuestaSIM = "";
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
      } else if (millis() - timerSMS >= 2000) {
        manejarFallaSMS("Timeout en CMGF");
      }
      break;

    case SMS_ENVIAR_CMGS:
      respuestaSIM = "";
      sim800l.println("AT+CMGS=\"+5491123692363\"");
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_CMGS;
      break;

    case SMS_ESPERAR_CMGS:
      if (respuestaSIM.indexOf(">") != -1) {
        estadoEnvio = SMS_ENVIAR_TEXTO;
      } else if (millis() - timerSMS >= 5000) {
        manejarFallaSMS("Timeout esperando '>'");
      }
      break;

    case SMS_ENVIAR_TEXTO:
      respuestaSIM = "";
      sim800l.print(mensajePendiente);
      sim800l.write(0x1A);
      timerSMS = millis();
      estadoEnvio = SMS_ESPERAR_ENVIO;
      break;

    case SMS_ESPERAR_ENVIO:
      if (respuestaSIM.indexOf("+CMGS:") != -1 || respuestaSIM.indexOf("OK") != -1) {
        Serial.println("\n>> ¡SMS ENVIADO CON ÉXITO!");
        estadoEnvio = SMS_IDLE;
      } else if (respuestaSIM.indexOf("ERROR") != -1) {
        manejarFallaSMS("Error devuelto por SIM800L");
      } else if (millis() - timerSMS >= 15000) {
        manejarFallaSMS("Timeout en envío");
      }
      break;

    case SMS_REINTENTAR:
      if (millis() - timerSMS >= PAUSA_REINTENTO) {
        Serial.println(">> Reintentando envío...");
        estadoEnvio = SMS_ENVIAR_CMGF;
      }
      break;
  }
}

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

  // NOTA: Si al cortar la luz el pin queda desconectado (flotante), 
  // usa un pulldown físico (resistencia de 10k a GND) y cambia a INPUT_PULLDOWN.
  pinMode(PIN_CORTE_LUZ, INPUT_PULLUP);
  
  // Esperar a que la tensión del pin se estabilice al arrancar
  delay(500); 
  estadoPinActual = digitalRead(PIN_CORTE_LUZ);

  Serial.println("\n--- INICIANDO SISTEMA ---");
  Serial.print("Estado inicial del GPIO2: ");
  Serial.println(estadoPinActual == LOW ? "LOW (Corte de Luz)" : "HIGH (Luz OK)");

  preferences.begin("energia", false);
  int ultimoEstadoGuardado = preferences.getInt("estado", HIGH);

  // Dar tiempo para que el módulo SIM800L se registre completamente en la red GSM
  delay(4000); 

  // LOGICA DE DECISIÓN AL ARRANCAR
  if (estadoPinActual == LOW) {
    // Si arranca en LOW, SIEMPRE fuerza el envío del mensaje de corte
    Serial.println(">> ALERTA EN SETUP: Detectado GPIO2 en LOW.");
    solicitarEnvioSMS("EMERGENCIA: Corte de luz");
    preferences.putInt("estado", LOW);
  } 
  else if (estadoPinActual == HIGH && ultimoEstadoGuardado == LOW) {
    // Solo envía "Regreso de luz" si antes estaba registrado un corte
    Serial.println(">> ALERTA EN SETUP: Luz restablecida.");
    solicitarEnvioSMS("Regreso de luz");
    preferences.putInt("estado", HIGH);
  } else {
    Serial.println(">> SETUP: Estado normal y estable.");
  }

  attachInterrupt(digitalPinToInterrupt(PIN_CORTE_LUZ), ISR_CambioEnergia, CHANGE);
}

void loop() {
  procesarEnvioSMS();

  if (cambioDetectado) {
    cambioDetectado = false;
    int nuevoEstado = digitalRead(PIN_CORTE_LUZ);

    if (nuevoEstado != estadoPinActual) {
      estadoPinActual = nuevoEstado;

      if (estadoPinActual == LOW) {
        Serial.println(">> CAMBIO EN LOOP: CORTE DE LUZ (GPIO2 LOW)");
        preferences.putInt("estado", LOW);
        solicitarEnvioSMS("EMERGENCIA: Corte de luz");
      } else {
        Serial.println(">> CAMBIO EN LOOP: REGRESO DE LUZ (GPIO2 HIGH)");
        preferences.putInt("estado", HIGH);
        solicitarEnvioSMS("Regreso de luz");
      }
    }
  }
}