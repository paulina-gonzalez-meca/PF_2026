#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>


//DEFINE ------------------------------------------------------------------------------------------------------------------------------------
  //radio
  #define CANAL_RADIO 108
  #define NRF_SPI_SPEED 2000000
  #define TIMEOUT_CONECTAR 10000UL   /*10 segundos*/
  #define TIEMPO_ENVIO_DATOS 5000UL /*1 minuto*/
  #define DIRECCION_A 0xF0F0F0F0E0ULL
  #define DIRECCION_B 0xF0F0F0F0E1ULL

  //PINES
  #define PIN_PULSADOR PIN_PB0
  #define PIN_LED PIN_PB1
  #define PIN_CSN PIN_PA7
  #define PIN_CE PIN_PA3
  #define PIN_TERMISTOR PIN_PA2
  #define PIN_PUERTA PIN_PA1
  #define PIN_ENERGIA PIN_PA0

  //sensores y esas cosas
  #define DEBOUNCE 20UL /*20 ms*/

  #define ABIERTO 1
  #define CERRADO 0
  #define TIEMPO_LIM_ABIERTO 30000UL /*30 segundos*/

  #define PRESIONADO 0
  #define NO_PRESIONADO 1

  #define BETA 3950.0
  #define T_0 298.15
  #define R_0 10000.0
  #define FIXED_R 10000.0
//


//VARIABLES ------------------------------------------------------------------------------------------------------------------------------------
  //radio ---------------------
    RF24 radio(PIN_CE, PIN_CSN, NRF_SPI_SPEED);

    //conectar
    bool flagConectar = 1;
    unsigned long int timerRadio = 0;

    bool flagConectado = 1;
    char mensajeNRF[33];
    char nombre[4] = "P1\0";
  //

  //puerta  -----------------------
    bool estadoAnteriorPuerta = CERRADO;
    unsigned long int debouncePuerta = 0;
    unsigned long int tiempoAbierto = 0;
    bool emergenciaPuerta = 0;
  //

  //pulsador -------------------
    bool estadoAnteriorPulsador = NO_PRESIONADO;
    unsigned long int debouncePulsador = 0;
  //

  float temperatura = 0;
  byte bateria = 0;
//

int adc;


//SETUP Y LOOP ------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TERMISTOR, INPUT);
  pinMode(PIN_PUERTA, INPUT_PULLUP);
  pinMode(PIN_ENERGIA, INPUT);


  radio.begin();
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setChannel(CANAL_RADIO);
  radio.openReadingPipe(1, DIRECCION_A);
  radio.openWritingPipe(DIRECCION_B);
  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(5, 15);
  radio.startListening();


  debouncePulsador = millis();
  tiempoAbierto = millis();
  debouncePuerta = millis();
  timerRadio = millis();
}

void loop() {
  if (flagConectado == 1) {
    lecturaPuerta();

    if (flagConectar == 1) {
      flagConectar = 0;
      digitalWrite(PIN_LED, LOW);

      //leer sensores
      temperatura = lecturaTemperatura();
      bateria = lecturaBateria();

      mandarDatos();
      timerRadio = millis();
    }

    if (millis() - timerRadio >= TIEMPO_ENVIO_DATOS) {
      //leer sensores
      temperatura = lecturaTemperatura();
      bateria = lecturaBateria();

      mandarDatos();
      timerRadio = millis();
    }
  }

  /*else {
    if (flagConectar == 0) {
      if (millis() - debouncePulsador >= DEBOUNCE) {
        bool pulsador = digitalRead(PIN_PULSADOR);

        if (pulsador != estadoAnteriorPulsador) {
          debouncePulsador = millis();
          estadoAnteriorPulsador = pulsador;

          if (pulsador == NO_PRESIONADO) {
            flagConectar = 1;
            timerRadio = millis();
            digitalWrite(PIN_LED, HIGH);
          }
        }
      }
    } else {
      flagConectado = conectar();

      if (millis() - timerRadio >= TIMEOUT_CONECTAR && flagConectado == 0) {
        flagConectar = 0;

        //Parpadea 3 veces si falla la conexion
        digitalWrite(PIN_LED, LOW);
        delay(100);
        digitalWrite(PIN_LED, HIGH);
        delay(100);
        digitalWrite(PIN_LED, LOW);
        delay(100);
        digitalWrite(PIN_LED, HIGH);
        delay(100);
        digitalWrite(PIN_LED, LOW);
        delay(100);
        digitalWrite(PIN_LED, HIGH);
        delay(100);
        digitalWrite(PIN_LED, LOW);
        delay(100);
      }
    }
  }*/
}





//FUNCIONES ------------------------------------------------------------------------------------------------------------------------------------
/*bool conectar() {
  if (radio.available() > 0) {
    uint8_t largoMensaje = radio.getDynamicPayloadSize();
    if (largoMensaje < 6 || largoMensaje > 32) {
      radio.flush_rx();
      return 0;
    }
    radio.read(mensajeNRF, largoMensaje);
    mensajeNRF[largoMensaje] = '\0';


    if(mensajeNRF[0] == '#' && mensajeNRF[1] == 'C' && mensajeNRF[2] == ',' && mensajeNRF[3] == 'P' && (mensajeNRF[4] >= '0' && mensajeNRF[4] <= '9')){
      if (mensajeNRF[5] == '*'){
        nombre[0] = 'P';
        nombre[1] = mensajeNRF[4];
        nombre[2] = '\0';
      } else if (mensajeNRF[6] == '*' && (mensajeNRF[5] >= '0' && mensajeNRF[5] <= '9')) {
        nombre[0] = 'P';
        nombre[1] = mensajeNRF[4];
        nombre[2] = mensajeNRF[5];
        nombre[3] = '\0';
      } else return 0;
    } else return 0;

    radio.stopListening();
    bool exito = radio.write("OK", 3);
    radio.startListening();
    if (exito){
      return 1;
    } else {
      memset(nombre, 0, sizeof(nombre));
      return 0;
    }
  } 
  return 0;
}*/

void mandarDatos() {
  //char mensajeNRF[32];  //#PX,100,-00.00,0* 17 caracteres + caracter de finalizacion

  char temperaturaTexto[10]; //-00.00
  dtostrf(temperatura, 0, 2, temperaturaTexto);

  //snprintf(mensajeNRF, sizeof(mensajeNRF), "#%s,%d,%s,%d*", nombre, bateria, temperaturaTexto, emergenciaPuerta);
  snprintf(mensajeNRF, sizeof(mensajeNRF), "#%s,%d,%s,%d*", nombre, adc, temperaturaTexto, emergenciaPuerta);


  radio.stopListening();
  bool exito = radio.write(mensajeNRF, strlen(mensajeNRF) + 1);
  radio.startListening();

  if (exito){
    digitalWrite(PIN_LED, HIGH);
    delay(500);
    digitalWrite(PIN_LED, LOW);
    delay(100);
  } else {
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    delay(100);
  }

  radio.flush_tx();
  radio.flush_rx();
}

//sensores ---------------------------------------------------
void lecturaPuerta() {
  if (millis() - debouncePuerta >= DEBOUNCE) {
    bool puerta = digitalRead(PIN_PUERTA);

    if (puerta != estadoAnteriorPuerta) {
      debouncePuerta = millis();
      estadoAnteriorPuerta = puerta;

      if (puerta == ABIERTO) {
        tiempoAbierto = millis();
      } else {
        emergenciaPuerta = 0;
      }
    }
  }

  if (estadoAnteriorPuerta == ABIERTO) {
    if (millis() - tiempoAbierto >= TIEMPO_LIM_ABIERTO) {
      emergenciaPuerta = 1;
    }
  }
}

float lecturaTemperatura() {
  analogReference(DEFAULT);
  delay(2);
  analogRead(PIN_TERMISTOR);
  adc = analogRead(PIN_TERMISTOR);
  float rNtc = FIXED_R / ((1023.0 / (float)adc) - 1.0);
  float tempNTC = 1.0 / ((1.0 / T_0) + (log(rNtc / R_0) / BETA)) - 273.15; //Ecuación Beta

  return tempNTC;
}

byte lecturaBateria() {
  analogReference(INTERNAL);
  delay(2);
  analogRead(PIN_ENERGIA);
  int lecturaPin = analogRead(PIN_ENERGIA);
  int bateriaPorcentaje = map(lecturaPin, 335, 558, 0, 100);
  return constrain(bateriaPorcentaje, 0, 100);
}