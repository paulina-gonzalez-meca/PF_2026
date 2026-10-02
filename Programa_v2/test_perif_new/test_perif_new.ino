// USAR ARDUINO UNO COMO ISP PARA ATTINY84
// ATTINY CORE --> http://drazzy.com/package_drazzy.com_index.json (Poner en Preferences)
// HERRAMIENTAS --> PLACA --> ATTINY 84A
// Mapeo Clockwise (PIN MAPPING --> clockwise)
// INCOMPLETO --> Finalizar SI o SI para antes del 30/10/2026.

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// incluir constantes
#define ERROR "%"

#define TIEMPO_ENVIAR_NRF 5000
#define TIEMPO_LECTURA 50000
#define TIEMPO_ENERGIA_PERIFERICOS 100000
#define TIEMPO_PUERTA 20000
#define TIEMPO_LED 100


#define PIN_PULSADOR 0
#define PIN_LED 1
#define PIN_IRQ 2 // No sé para que sirve aún
#define PIN_CSN 7
#define PIN_CE 2
#define PIN_SCK 4
#define PIN_MOSI 6
#define PIN_MISO 5

#define PIN_RESET

class per{
  public:
    char nombre[4]; // Los nombres pueden almacenar hasta 4 caracteres
    int tipoSensor1;
    int tipoSensor2;
    int tipoSensor3;
    uint64_t direccion;

    per(char nombrePeriferico, int tipoSensor1Per, int tipoSensor2Per, int tipoSensor3Per, uint64_t direccionPer){
      nombre[4] = nombrePeriferico;
      tipoSensor1 = tipoSensor1Per;
      tipoSensor2 = tipoSensor2Per;
      tipoSensor3 = tipoSensor3Per;
      direccion = direccionPer;
    }
};

per periferico("P1", 0, 0, 0, 0xF0F0F0F0E1LL);
const uint64_t direccionCentral = 0xF0F0F0F0E0LL;

RF24 radio(PIN_CE, PIN_CSN);

char mensajeRecibidoNRF[32];
bool flagCorte = 1;
bool respuestaNRF = 0;
bool pedido = 0;
int cicloNRF = 0;

unsigned long int tiempoLed = 0;
unsigned long int tiempoLectura = 0;
int switchLecturas = 0;
unsigned long int timerNRF = 0;
// corregir
char[32] tramaNRF;
char[32] tramaProceso;
String tramaPerifericoFinal = "";

int ind = 0;
bool flagBoton = 0;
bool emergencia = 0;
bool leer = 0;
bool flagNormalidad = 0;
bool flagMandarNRF = 0;
bool flagSensor1 = 0;
bool flagSensor2 = 0;
bool flagSensor3 = 0;
bool flagPulsador = 0;
char respuesta[3] = {'L', 'L', 'L'};

unsigned long int tiempoS1 = 0;
unsigned long int tiempoS2 = 0;
unsigned long int tiempoS3 = 0;

float lecturaS1 = 0;
float lecturaS2 = 0;
float lecturaS3 = 0;
int umbralMaxS2 = 30;
int umbralMinS2 = 10

/*std::vector<String> tramas;
std::vector<String> mensajesNRF;
std::vector<datosPer> filaDatosPer;
*/

char bufferNRF[10][32] = {};

typedef enum { //maquina de estados
  INICIO,
  APD,
  L1,
  L2,
  L3
} PASOS_DECODIFICADOR_t;
PASOS_DECODIFICADOR_t PDECO;

typedef enum {
  EMG,
  LUZ,
  SENSOR_1,
  SENSOR_2,
  SENSOR_3,
  ARMADO_ENVIADO
} ESTADOSARMADOPERIFERICO_t;
ESTADOSARMADOPERIFERICO_t estadosArmadoPeriferico = EMG;

hw_timer_t *timer = NULL; //timer

void IRAM_ATTR onTimer(); //function interrupts every 1ms

void setup() {
  

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);


  delay(2000);
  Serial.println("trama: #ApodoDisp,ResSens1,ResSens2,ResSens3*");

  pinMode(PIN_ENERGIA, INPUT);
  pinMode(PIN_PULSADOR, INPUT);

  SPI.begin(PIN_SCK,PIN_MISO,PIN_MOSI,5);
  radio.begin();
  radio.openReadingPipe(1, periferico.direccion);
  radio.openWritingPipe(direccionCentral);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  PDECO = INICIO;

  timer = timerBegin(1000000); // 1 MHz = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0); // tick every 1ms

  digitalWrite(PIN_LED, LOW);
  Serial.println("test end setup");

}

void loop() {
  // put your main code here, to run repeatedly:

}


/*void insertarEnTren(const char* nuevoDato) {
  // 1. Si no hemos llenado el buffer de 10, incrementamos la cuenta
  if (totalElementos < 10) {
    totalElementos++;
  }

  // 2. Desplazamos los elementos existentes una posición hacia la derecha (hacia atrás)
  // memmove(destino, origen, tamaño_en_bytes)
  memmove(&bufferNRF[1], &bufferNRF[0], (totalElementos - 1) * 32);

  // 3. Copiamos el nuevo dato en la posición 0
  strncpy(bufferNRF[0], nuevoDato, 31);
  bufferNRF[0][31] = '\0'; // Aseguramos el terminador nulo por seguridad

  Serial.print("\n--> Nuevo dato ingresado: ");
  Serial.println(nuevoDato);
  mostrarBuffer();
}*/