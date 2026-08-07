#include <esp_now.h>
#include <WiFi.h>

// --- CONFIGURACIÓN DE HARDWARE ---
#define LED_PIN 8  // GPIO típico para LED en ESP32-C3

// --- DIRECCIONES MAC (HARDCODEADAS) ---
uint8_t macReceptor[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // Reemplazar con MAC destino
uint8_t macEmisor[]   = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// --- VARIABLES PARA EL TIMER ---
hw_timer_t *timer = NULL;
volatile uint32_t milisegundosSincronizados = 0;
volatile int32_t ledCountdown = 0;
volatile bool enviarMensajeFlag = false;

// --- FUNCIÓN DE INTERRUPCIÓN (ISR) ---
void IRAM_ATTR onTimerTick() {
  milisegundosSincronizados++;

  // Apagado de LED no bloqueante
  if (ledCountdown > 0) {
    ledCountdown--;
    if (ledCountdown == 0) {
      digitalWrite(LED_PIN, LOW);
    }
  }

  // Activar bandera de envío cada 5000 ms (5 segundos)
  if (milisegundosSincronizados >= 5000) {
    milisegundosSincronizados = 0;
    enviarMensajeFlag = true;
  }
}

// --- CONFIGURACIÓN DEL TIMER INTEGRADO (ESP32 Core 3.x) ---
void configurarTimerHardware() {
  timer = timerBegin(1000000);             // Frecuencia a 1 MHz
  timerAttachInterrupt(timer, &onTimerTick);
  timerAlarm(timer, 1000, true, 0);        // Alarma cada 1 ms (1000 us)
  Serial.println("Timer de Hardware configurado a 1ms.");
}

// --- CALLBACKS DE ESP-NOW (COMPATIBLES CON CORE 3.X) ---
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Envío OK" : "Envío FALLIDO");
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  char buffer[len + 1];
  memcpy(buffer, incomingData, len);
  buffer[len] = '\0';

  Serial.print("Recibido: ");
  Serial.println(buffer);

  // Encender LED y definir temporizador a 2000 ms
  digitalWrite(LED_PIN, HIGH);
  ledCountdown = 2000;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, macReceptor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  configurarTimerHardware();
}

void loop() {
  if (enviarMensajeFlag) {
    enviarMensajeFlag = false;
    const char *mensaje = "Probando";
    esp_now_send(macReceptor, (uint8_t *)mensaje, strlen(mensaje));
    Serial.println("Enviando mensaje periódico...");
  }
}