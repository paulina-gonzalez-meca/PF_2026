#include <Arduino.h>

// Configuración de Pines del SIM800L en ESP32
#define RXD2 16          // Conectar al TX del SIM800L
#define TXD2 17          // Conectar al RX del SIM800L
#define PIN_PULSADOR 32  // Pin boton
#define DEBOUNCE 30      // 30ms

// Número de teléfono de destino
#define NUMERO_DESTINO "+5491123692363"

// Estados para el envío del SMS
enum EstadoSMS {
  SMS_INIT_MODEM,
  SMS_IDLE,
  SMS_ENVIAR_CMGS,
  SMS_ESPERAR_PROMPT,
  SMS_ENVIAR_TEXTO,
  SMS_ENVIAR_CTRL_Z,
  SMS_ESPERAR_CONFIRMACION
};

// Variables de Control
EstadoSMS estadoActual = SMS_INIT_MODEM;
unsigned long tiempoEstado = 0;
unsigned long tiempoPulsador = 0;
unsigned long ultimoTiempoPulsador = 0;

String bufferSerialPC = "";
String mensajeAEnviar = "";
bool hayMensajeNuevo = false;
bool ultimoEstado = 1;

// Prototipos de Funciones
void leerSerialPC();
void leerRespuestaSim800();
void procesarEnvioSMS();

void setup() {
  // Puerto de depuración y entrada de la PC
  Serial.begin(9600);
  pinMode(PIN_PULSADOR, INPUT);

  // Puerto UART2 conectado al SIM800L
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Serial.println("\n==============================================");
  Serial.println("  ESP32 Terminal a SMS (Código No Bloqueante) ");
  Serial.println("==============================================");
  Serial.println("Presionar boton para enviar mensaje");

  tiempoEstado = millis();
  tiempoPulsador = millis();
}

void loop() {

  mensaje();
  leerRespuestaSim800();  // Reenvía la respuesta del módem a la consola
  procesarEnvioSMS();     // Gestiona la máquina de estados con millis()
}

// ====================================================
// mensaje
// ====================================================
void mensaje() {
  unsigned long ahora = millis();
  int estadoPulsador = digitalRead(PIN_PULSADOR);

  // Detección de presionado (transición de HIGH a LOW con pull-up)
  if (ultimoEstado == 1 && estadoPulsador == 0) {
    if (ahora - ultimoTiempoPulsador >= DEBOUNCE) {
      ultimoEstado = 0;
      ultimoTiempoPulsador = ahora;
    }
  } 
  // Detección de liberación (transición de LOW a HIGH)
  else if (ultimoEstado == 0 && estadoPulsador == 1) {
    if (ahora - ultimoTiempoPulsador >= DEBOUNCE) {
      ultimoEstado = 1;
      mensajeAEnviar = "verdura";
      hayMensajeNuevo = true;  // <--- IMPORTANTE: Activa el envío
      ultimoTiempoPulsador = ahora;
    }
  }
}

// ====================================================
// Lectura No Bloqueante de Respuestas del SIM800L
// ====================================================
void leerRespuestaSim800() {
  // Se lee byte a byte sin bloquear el hilo principal
  if (Serial2.available() > 0) {
    char c = Serial2.read();
    Serial.write(c);  // Muestra la respuesta en vivo en el Monitor Serie
  }
}

// ====================================================
// Máquina de Estados para Envío de SMS sin delay()
// ====================================================
void procesarEnvioSMS() {
  unsigned long ahora = millis();

  switch (estadoActual) {

    case SMS_INIT_MODEM:
      // Espera 3 segundos iniciales tras encender para configurar el modo texto
      if (ahora - tiempoEstado >= 3000) {
        Serial2.println("AT+CMGF=1");
        tiempoEstado = ahora;
        estadoActual = SMS_IDLE;
        Serial.println("[MODEM] Configurado en Modo Texto (AT+CMGF=1)");
      }
      break;

    case SMS_IDLE:
      // Espera la bandera de un nuevo mensaje capturado desde la PC
      if (hayMensajeNuevo) {
        Serial.println("[SMS] Solicitando envío al módem...");
        Serial2.println("AT+CMGS=\"" NUMERO_DESTINO "\"");
        tiempoEstado = ahora;
        estadoActual = SMS_ESPERAR_PROMPT;
      }
      break;

    case SMS_ESPERAR_PROMPT:
      // Espera 1 segundo para que el módem devuelva el prompt '>'
      if (ahora - tiempoEstado >= 1000) {
        Serial2.print(mensajeAEnviar);
        tiempoEstado = ahora;
        estadoActual = SMS_ENVIAR_CTRL_Z;
      }
      break;

    case SMS_ENVIAR_CTRL_Z:
      // Espera 500ms tras volcar el texto para enviar la señal de confirmación
      if (ahora - tiempoEstado >= 500) {
        Serial2.write(0x1A);  // Envia carácter ASCII 26 (Ctrl+Z)
        tiempoEstado = ahora;
        estadoActual = SMS_ESPERAR_CONFIRMACION;
      }
      break;

    case SMS_ESPERAR_CONFIRMACION:
      // Tiempo de holgura para recibir el OK de la red celular antes de liberar la máquina
      if (ahora - tiempoEstado >= 3000) {
        Serial.println("\n[SMS] Operación completada.");
        hayMensajeNuevo = false;  // Resetea la bandera para aceptar nuevos mensajes
        estadoActual = SMS_IDLE;
      }
      break;
  }
}