#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 14
#define TFT_CS 32
#define TFT_MOSI 2
#define TFT_CLK 17
#define TFT_RST 27
#define TFT_MISO 16
#define TFT_BL 12
#define ENCODER_A_PIN 25
#define ENCODER_B_PIN 26
#define ENCODER_SW_PIN 39
#define TIEMPO_DEBOUNCE 200
#define TIEMPO_PARPADEO 500
#define TIMEOUT_AGREGAR 1000

#define ANCHO 18

class per {
public:
  String nombreInterno;
  char nombreExterno[5] = { "-" };
  char nombreSensor1[5] = { "1" };
  char nombreSensor2[5] = { "2" };
  int tipoSensor1;
  int tipoSensor2;
  int lecturaSensor1;
  int lecturaSensor2;
  int indiceNombrePer = 0;
  int indiceNombreS1 = 0;
  int indiceNombreS2 = 0;
  bool alertaS2[20] = { 0 };
  bool alertaS1[20] = { 0 };
  per() {
    nombreInterno = "nombrePeriferico";
    nombreExterno;
    nombreSensor1;
    nombreSensor2;
    tipoSensor1 = 0;
    tipoSensor2 = 0;
    alertaS1;
    alertaS2;
    indiceNombrePer;
    indiceNombreS1;
    indiceNombreS2;
  }
};

char letras[27] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
char numeros[11] = { "1234567890" };
volatile int valorEncoder = 0;
int ultimoValorCLK = 0;
int ultimoValorDT = 0;
volatile int debounceSWEncoder = 0;
volatile int tiempoAgregarPer = 0;
volatile int parpadeo = 0;
int valorDisplay = 0;
int modificarUsuario = 0;
int modificarPer = 0;
int eliminar = 0;
bool seleccionar = 0;
bool cambioEncoder = 0;
bool flagSWEncoder = 0;

per perifericos[20];
int indicePerifericos = 0;
struct matriz {
  char numeroUs[15];
  char nombreUs[5] = "-";
  int indiceNumeroUs = 0;
  int indiceNombreUs = 0;
};
matriz usuario[20] = {};
int indiceUsuario = 0;
char numeroUsBackup[15];
char nombreUsBackup[5];
char nombrePerBackup[5];
char nombreS1Backup[5];
char nombreS2Backup[5];

typedef enum {  // state machine
  INICIO,
  USUARIO,
  EDITAR_US,
  EDITAR_NOMBRE_US,
  EDITAR_NUM_US,
  AGREGAR_NOMBRE_US,
  AGREGAR_NUM_US,
  PERIFERICO,
  MOSTRAR_PER,
  EDITAR_PER,
  EDITAR_NOMBRE_PER,
  EDITAR_NOMBRES1_PER,
  EDITAR_ALERTAS1_PER,
  EDITAR_UMBRALS1_PER,
  EDITAR_NOMBRES2_PER,
  EDITAR_ALERTAS2_PER,
  EDITAR_UMBRALS2_PER,
  ELIMINAR_PER,
  AGREGAR_PER
} PASOS_DISPLAY_t;
PASOS_DISPLAY_t TFT_DISPLAY;

hw_timer_t *timer = NULL;
void IRAM_ATTR onTimer();

void IRAM_ATTR rotacionEncoder();
void IRAM_ATTR botonEncoder();

int displayLetras(char nombre[], int &indiceNombre);
int displayNum(char numero[], int &indiceNumero);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST, 1000000);

void setup() {
  TFT_DISPLAY = INICIO;
  SPI.begin(TFT_CLK, TFT_MISO, TFT_MOSI);
  Serial.begin(9600);
  tft.begin();
  // ledcAttachChannel(TFT_BL, 5000, 8, 0);
  // ledcWriteChannel(TFT_BL, 200);
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(ENCODER_SW_PIN, INPUT);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), rotacionEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_SW_PIN), botonEncoder, FALLING);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);

  ultimoValorCLK = digitalRead(ENCODER_A_PIN);
  ultimoValorDT = digitalRead(ENCODER_B_PIN);

  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(4);
  tft.print("USUARIOS");
  tft.setCursor(10, 70);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(4);
  tft.print("PERIFERICOS");


  strcpy(usuario[0].nombreUs, "AAAA");
  strcpy(usuario[1].nombreUs, "BBBB");
  strcpy(usuario[2].nombreUs, "CCCC");
  strcpy(usuario[3].nombreUs, "DDDD");
  strcpy(usuario[4].nombreUs, "EEEE");
  strcpy(usuario[5].nombreUs, "FFFF");
  strcpy(usuario[6].nombreUs, "GGGG");
  strcpy(usuario[7].nombreUs, "HHHH");
  strcpy(usuario[8].nombreUs, "IIII");
  strcpy(usuario[9].nombreUs, "JJJJ");
  indiceUsuario = 10;
  strcpy(perifericos[0].nombreExterno, "1111");
  strcpy(perifericos[1].nombreExterno, "2222");
  strcpy(perifericos[2].nombreExterno, "3333");
  strcpy(perifericos[3].nombreExterno, "4444");
  strcpy(perifericos[4].nombreExterno, "5555");
  strcpy(perifericos[5].nombreExterno, "6666");
  strcpy(perifericos[6].nombreExterno, "7777");
  strcpy(perifericos[7].nombreExterno, "8888");
  strcpy(perifericos[8].nombreExterno, "9999");
  strcpy(perifericos[9].nombreExterno, "0000");
  indicePerifericos = 10;
}


void loop() {
  display();
}

void display() {
  switch (TFT_DISPLAY) {
    case INICIO:
      if (flagSWEncoder) {
        if (valorDisplay % 2 != 0) {
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        } else {
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = USUARIO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay % 2 == 0) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(4);
            tft.print("USUARIOS");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(4);
            tft.print("USUARIOS");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 0, 20) % 2 != 0) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("USUARIOS");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 0, 20);
          cambioEncoder = 0;
        }
      } else {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 70);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(4);
            tft.print("PERIFERICOS");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 70);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(4);
            tft.print("PERIFERICOS");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 0, 20) % 2 == 0) {
          tft.setCursor(10, 70);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("PERIFERICOS");
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 0, 20);
        }
      }
      break;
    case USUARIO:
      if (flagSWEncoder) {
        if (valorDisplay <= indiceUsuario) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Numero: ");
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].numeroUs);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          modificarUsuario = valorDisplay - 1;
          strcpy(numeroUsBackup, usuario[modificarUsuario].numeroUs);
          strcpy(nombreUsBackup, usuario[modificarUsuario].nombreUs);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_US;
        } else if (valorDisplay == indiceUsuario + 1) {
          if (indiceUsuario < 20) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(28, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("ABCDEFGHI");
            tft.setCursor(28, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("JKLMNOPQR");
            tft.setCursor(28, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("STUVWXYZ");
            tft.setCursor(28, 120);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Borrar");
            tft.setCursor(28, 145);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Confirmar");
            tft.setCursor(28, 170);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            strcpy(nombreUsBackup, "   ");
            strcpy(numeroUsBackup, "             ");
            valorDisplay = 1;
            valorEncoder = 0;
            flagSWEncoder = 0;
            TFT_DISPLAY = AGREGAR_NOMBRE_US;
          }
        } else if (valorDisplay == indiceUsuario + 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("USUARIOS");
          tft.setCursor(10, 70);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("PERIFERICOS");
          valorDisplay = 0;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = INICIO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indiceUsuario + 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indiceUsuario + 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      }
      break;
    case EDITAR_US:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRE_US;
        } else if (valorDisplay == 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("123");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("456");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("7890");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NUM_US;
        } else if (valorDisplay == 3) {
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          flagSWEncoder = 0;
          valorEncoder = 0;
          TFT_DISPLAY = USUARIO;
        }
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].nombreUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].nombreUs);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 3) != 1) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].nombreUs);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 3);
          cambioEncoder = 0;
        }
      } else if (valorDisplay == 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].numeroUs);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[modificarUsuario].numeroUs);
            ;
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 3) != 2) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].numeroUs);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 3);
        }
      } else if (valorDisplay == 3) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 3) != 3) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 3);
        }
      }
      break;
    case EDITAR_NOMBRE_US:
      {
        short int returnDisplayLetras = displayLetras(usuario[modificarUsuario].nombreUs, usuario[modificarUsuario].indiceNombreUs);
        if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
          if (returnDisplayLetras == 2) {
            strcpy(usuario[modificarUsuario].nombreUs, nombreUsBackup);
          }
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].nombreUs);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Numero: ");
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].numeroUs);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_US;
        }
        break;
      }
    case EDITAR_NUM_US:
      {
        short int returnDisplayNum = displayNum(usuario[modificarUsuario].numeroUs, usuario[modificarUsuario].indiceNumeroUs);
        if (returnDisplayNum == 1 || returnDisplayNum == 2) {
          if (returnDisplayNum == 1) {
            strcpy(numeroUsBackup, usuario[modificarUsuario].numeroUs);
          } else {
            strcpy(usuario[modificarUsuario].numeroUs, numeroUsBackup);
          }
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].nombreUs);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Numero: ");
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[modificarUsuario].numeroUs);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_US;
        }
        break;
      }
    case AGREGAR_NOMBRE_US:
      {
        short int returnDisplayLetras = displayLetras(usuario[indiceUsuario].nombreUs, usuario[indiceUsuario].indiceNombreUs);
        if (returnDisplayLetras == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("123");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("456");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("7890");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = AGREGAR_NUM_US;
        }
        if (returnDisplayLetras == 2) {
          strcpy(usuario[indiceUsuario].nombreUs, nombreUsBackup);
          usuario[indiceUsuario].indiceNombreUs = 0;
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          TFT_DISPLAY = USUARIO;
        }
        break;
      }
    case AGREGAR_NUM_US:
      {
        short int returnDisplayNum = displayNum(usuario[indiceUsuario].numeroUs, usuario[indiceUsuario].indiceNumeroUs);
        if (returnDisplayNum == 1 || returnDisplayNum == 2) {
          if (returnDisplayNum == 1) {
            indiceUsuario++;
          } else {
            strcpy(usuario[indiceUsuario].numeroUs, numeroUsBackup);
            strcpy(usuario[indiceUsuario].nombreUs, nombreUsBackup);
            usuario[indiceUsuario].indiceNumeroUs = 0;
            usuario[indiceUsuario].indiceNombreUs = 0;
          }
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorDisplay = 1;
          valorEncoder = 0;
          TFT_DISPLAY = USUARIO;
        }
        break;
      }
    case PERIFERICO:
      if (flagSWEncoder) {
        if (valorDisplay <= indicePerifericos) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].lecturaSensor1);
          tft.setCursor(102, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreSensor2);
          tft.setCursor(102, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].lecturaSensor2);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Editar");
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Eliminar");
          modificarPer = valorDisplay - 1;
          strcpy(nombrePerBackup, "   ");
          strcpy(nombreS1Backup, "   ");
          strcpy(nombreS2Backup, "   ");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = MOSTRAR_PER;
        } else if (valorDisplay == indicePerifericos + 1) {
          if (indicePerifericos < 20) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(28, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Buscando");
            tft.setCursor(28, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("perifericos...");
            valorDisplay = 1;
            valorEncoder = 0;
            flagSWEncoder = 0;
            tiempoAgregarPer = 0;
            TFT_DISPLAY = AGREGAR_PER;
          }
        } else if (valorDisplay == indicePerifericos + 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("USUARIOS");
          tft.setCursor(10, 70);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("PERIFERICOS");
          valorDisplay = 0;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = INICIO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indicePerifericos + 2)) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indicePerifericos + 2);
          if (valorDisplay <= indicePerifericos && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 5].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 4].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indicePerifericos + 2)) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indicePerifericos + 2);
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indicePerifericos + 2)) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indicePerifericos + 2);
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indicePerifericos + 2)) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indicePerifericos + 2);
        }
      } else if (valorDisplay <= indicePerifericos && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indicePerifericos + 2)) {
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[valorDisplay - 1].nombreExterno);
          valorDisplay = map(valorEncoder, 0, 80, 1, indicePerifericos + 2);
          if (valorDisplay <= indicePerifericos && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            if (indicePerifericos >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 1].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 2].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 3].nombreExterno);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indicePerifericos + 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Agregar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indicePerifericos + 2)) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indicePerifericos + 2);
          if (valorDisplay <= indicePerifericos && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 5].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 4].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 4].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 3].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 2].nombreExterno);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indicePerifericos && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indicePerifericos + 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indicePerifericos + 2)) {
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indicePerifericos + 2);
          if (valorDisplay <= indicePerifericos) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[valorDisplay - 1].nombreExterno);
            if (indicePerifericos >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 1].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 2].nombreExterno);
            }
            if (indicePerifericos >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[valorDisplay + 3].nombreExterno);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Agregar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      }
      break;
    case MOSTRAR_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        } else if(valorDisplay == 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre S2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_PER;
        }
        else if(valorDisplay == 3){
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("Eliminar ");
          tft.setCursor(10, 55);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Si");
          tft.setCursor(60, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("No");
          valorEncoder = 0;
          valorDisplay = 1;
          eliminar = modificarPer + 1;
          TFT_DISPLAY = ELIMINAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 3) != 1) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 3);
          cambioEncoder = 0;
        }
      } else if (valorDisplay == 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Editar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Editar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 3) != 2) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Editar");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 3);
          cambioEncoder = 0;
        }
      } else if (valorDisplay == 3) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Eliminar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Eliminar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 3) != 3) {
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Eliminar");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 3);
          cambioEncoder = 0;
        }
      }
      break;
    case EDITAR_PER:
      if (flagSWEncoder) {
        if (valorDisplay == 1) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombrePerBackup, perifericos[modificarPer].nombreExterno);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRE_PER;
        }
        else if (valorDisplay == 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombreS1Backup, perifericos[modificarPer].nombreSensor1);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRES1_PER;
        }
        else if (valorDisplay == 3) {
          tft.fillScreen(ILI9341_BLACK);
          if (indiceUsuario >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[0].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[0]);
          }
          if (indiceUsuario >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[1].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[1]);
          }
          if (indiceUsuario >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[2].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[2]);
          }
          if (indiceUsuario >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[3].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[3]);
          }
          if (indiceUsuario >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[4].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[4]);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = EDITAR_ALERTAS1_PER;
        }
        else if (valorDisplay == 4) {
          TFT_DISPLAY = EDITAR_UMBRALS1_PER;
        }
        else if (valorDisplay == 5) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(28, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("ABCDEFGHI");
          tft.setCursor(28, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("JKLMNOPQR");
          tft.setCursor(28, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("STUVWXYZ");
          tft.setCursor(28, 120);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Borrar");
          tft.setCursor(28, 145);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Confirmar");
          tft.setCursor(28, 170);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          strcpy(nombreS2Backup, perifericos[modificarPer].nombreSensor2);
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = EDITAR_NOMBRES2_PER;
        }
        else if(valorDisplay == 6){
          TFT_DISPLAY = EDITAR_ALERTAS2_PER;
        }
        else if (valorDisplay == 7) {
          TFT_DISPLAY = EDITAR_UMBRALS2_PER;
        }
        else if (valorDisplay == 8) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreExterno);
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor1);
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].lecturaSensor1);
          tft.setCursor(102, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].nombreSensor2);
          tft.setCursor(102, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].lecturaSensor2);
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Editar");
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Eliminar");
          valorDisplay = 1;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = MOSTRAR_PER;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay == 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Nombre: ");
            tft.print(perifericos[modificarPer].nombreExterno);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Nombre: ");
            tft.print(perifericos[modificarPer].nombreExterno);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 1) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre: ");
          tft.print(perifericos[modificarPer].nombreExterno);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }else if (valorDisplay == 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Nombre s1: ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Nombre s1: ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 2) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre s1: ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }else if (valorDisplay == 3) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 3) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }else if (valorDisplay == 4) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor1);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 4) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor1);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }else if (valorDisplay == 5) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Nombre s2: ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Nombre s2: ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 5) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Nombre s2: ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }else if (valorDisplay == 6) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 160);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Alertas ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 6) {
          tft.setCursor(10, 160);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Alertas ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }else if (valorDisplay == 7) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 185);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 185);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Umbral ");
            tft.print(perifericos[modificarPer].nombreSensor2);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 7) {
          tft.setCursor(10, 185);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Umbral ");
          tft.print(perifericos[modificarPer].nombreSensor2);
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }else if (valorDisplay == 8) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 210);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 210);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 1, 8) != 8) {
          tft.setCursor(10, 210);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, 8);
          cambioEncoder = 0;
        }
      }
      break;
    case EDITAR_NOMBRE_PER: {
      short int returnDisplayLetras = displayLetras(perifericos[modificarPer].nombreExterno, perifericos[modificarPer].indiceNombrePer);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[modificarPer].nombreExterno, nombrePerBackup);
        }
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre: ");
        tft.print(perifericos[modificarPer].nombreExterno);
        tft.setCursor(10, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S1: ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S2: ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 160);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 185);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 210);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Volver");
        valorEncoder = 0;
        valorDisplay = 1;
        TFT_DISPLAY = EDITAR_PER;
      }
      break;
    }
    case EDITAR_NOMBRES1_PER: {
      short int returnDisplayLetras = displayLetras(perifericos[modificarPer].nombreSensor1, perifericos[modificarPer].indiceNombreS1);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[modificarPer].nombreSensor1, nombreS1Backup);
        }
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre: ");
        tft.print(perifericos[modificarPer].nombreExterno);
        tft.setCursor(10, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S1: ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S2: ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 160);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 185);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 210);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Volver");
        valorEncoder = 0;
        valorDisplay = 1;
        TFT_DISPLAY = EDITAR_PER;
      }
      break;
    }
    case EDITAR_ALERTAS1_PER:
      if (flagSWEncoder) {
        if (valorDisplay <= indiceUsuario) {
          if(perifericos[modificarPer].alertaS1[valorDisplay - 1] == 1){
            perifericos[modificarPer].alertaS1[valorDisplay - 1] = 0;
          }else{
            perifericos[modificarPer].alertaS1[valorDisplay - 1] = 1;
          }
          TFT_DISPLAY = EDITAR_US;
        } else if (valorDisplay == indiceUsuario + 1) {
          if (indiceUsuario < 20) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(28, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("ABCDEFGHI");
            tft.setCursor(28, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("JKLMNOPQR");
            tft.setCursor(28, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("STUVWXYZ");
            tft.setCursor(28, 120);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Borrar");
            tft.setCursor(28, 145);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Confirmar");
            tft.setCursor(28, 170);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            strcpy(nombreUsBackup, "   ");
            strcpy(numeroUsBackup, "             ");
            valorDisplay = 1;
            valorEncoder = 0;
            flagSWEncoder = 0;
            TFT_DISPLAY = AGREGAR_NOMBRE_US;
          }
        } else if (valorDisplay == indiceUsuario + 2) {
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("USUARIOS");
          tft.setCursor(10, 70);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(4);
          tft.print("PERIFERICOS");
          valorDisplay = 0;
          valorEncoder = 0;
          flagSWEncoder = 0;
          TFT_DISPLAY = INICIO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 10);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 5]);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 4]);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 35);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 60);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 85);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
        }
      } else if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(usuario[valorDisplay - 1].nombreUs);
          tft.setCursor(100, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay]);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 2]);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
              tft.setCursor(100, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 3]);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indiceUsuario + 1) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Cancelar");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Cancelar");
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario && (valorDisplay == 5 || valorDisplay == 10 || valorDisplay == 15 || valorDisplay == 20)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 5].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 5]);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 4]);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 4 || valorDisplay == 9 || valorDisplay == 14 || valorDisplay == 19)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 4].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 4]);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 3 || valorDisplay == 8 || valorDisplay == 13 || valorDisplay == 18)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 3].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 3]);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 2 || valorDisplay == 7 || valorDisplay == 12 || valorDisplay == 17)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 2].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 2]);
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else if (valorDisplay <= indiceUsuario && (valorDisplay == 1 || valorDisplay == 6 || valorDisplay == 11 || valorDisplay == 16)) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Cancelar");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      } else if (valorDisplay == indiceUsuario + 2) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Volver");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && valorDisplay != map(valorEncoder, 0, 80, 1, indiceUsuario + 2)) {
          cambioEncoder = 0;
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 1, indiceUsuario + 2);
          if (valorDisplay <= indiceUsuario) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(usuario[valorDisplay - 1].nombreUs);
            tft.setCursor(100, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            if (indiceUsuario >= valorDisplay + 1) {
              tft.setCursor(10, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay].nombreUs);
              tft.setCursor(100, 35);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            }
            if (indiceUsuario >= valorDisplay + 2) {
              tft.setCursor(10, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 1].nombreUs);
              tft.setCursor(100, 60);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 1]);
            }
            if (indiceUsuario >= valorDisplay + 3) {
              tft.setCursor(10, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 2].nombreUs);
              tft.setCursor(100, 85);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay - 1]);
            }
            if (indiceUsuario >= valorDisplay + 4) {
              tft.setCursor(10, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(usuario[valorDisplay + 3].nombreUs);
              tft.setCursor(100, 110);
              tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
              tft.setTextSize(3);
              tft.print(perifericos[modificarPer].alertaS1[valorDisplay + 3]);
            }
            tft.setCursor(10, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          } else {
            tft.setCursor(140, 135);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Volver");
          }
        }
      }
      break;
    case EDITAR_UMBRALS1_PER:
      break;
    case EDITAR_NOMBRES2_PER: {
      short int returnDisplayLetras = displayLetras(perifericos[modificarPer].nombreSensor2, perifericos[modificarPer].indiceNombreS2);
      if (returnDisplayLetras == 1 || returnDisplayLetras == 2) {
        if (returnDisplayLetras == 2) {
          strcpy(perifericos[modificarPer].nombreSensor2, nombreS2Backup);
        }
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(10, 35);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre: ");
        tft.print(perifericos[modificarPer].nombreExterno);
        tft.setCursor(10, 60);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S1: ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 85);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor1);
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Nombre S2: ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 160);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Alertas ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 185);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Umbral ");
        tft.print(perifericos[modificarPer].nombreSensor2);
        tft.setCursor(10, 210);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("Volver");
        valorEncoder = 0;
        valorDisplay = 1;
        TFT_DISPLAY = EDITAR_PER;
      }
      break;
    }
    case EDITAR_ALERTAS2_PER:
      break;
    case EDITAR_UMBRALS2_PER:
      break;
    case ELIMINAR_PER:
      if (flagSWEncoder) {
        if (valorDisplay%2 == 1 || valorDisplay%2 == 0) {
          if(valorDisplay%2 == 0){
            if(!eliminarPer(perifericos, 20, eliminar)){
              break;
            }
            indicePerifericos --;
          }
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        }
        flagSWEncoder = 0;
      } else if (valorDisplay%2 == 0) {
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("Si");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("Si");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 0, 20)%2 != 0) {
          tft.setCursor(10, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Si");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 0, 20);
          cambioEncoder = 0;
        }
      } else{
        if (parpadeo >= TIEMPO_PARPADEO) {
          if (seleccionar) {
            tft.setCursor(60, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print("No");
            seleccionar = 0;
            parpadeo = 0;
          } else {
            tft.setCursor(60, 110);
            tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
            tft.setTextSize(3);
            tft.print("No");
            seleccionar = 1;
            parpadeo = 0;
          }
        }
        if (cambioEncoder && map(valorEncoder, 0, 80, 0, 20)%2 != 1) {
          tft.setCursor(60, 110);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("No");
          seleccionar = 0;
          parpadeo = TIEMPO_PARPADEO;
          valorDisplay = map(valorEncoder, 0, 80, 0, 20);
          cambioEncoder = 0;
        }
      }
      break;
    case AGREGAR_PER:
    if(conectarPer()){
      //SEGUIR
    }else if(tiempoAgregarPer >= TIMEOUT_AGREGAR){
        tft.setCursor(28, 110);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("No se encontro");
        tft.setCursor(10, 135);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.setTextSize(3);
        tft.print("ningun periferico");
        if(tiempoAgregarPer >= TIMEOUT_AGREGAR + 1000){
          tft.fillScreen(ILI9341_BLACK);
          if (indicePerifericos >= 1) {
            tft.setCursor(10, 10);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[0].nombreExterno);
          }
          if (indicePerifericos >= 2) {
            tft.setCursor(10, 35);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[1].nombreExterno);
          }
          if (indicePerifericos >= 3) {
            tft.setCursor(10, 60);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[2].nombreExterno);
          }
          if (indicePerifericos >= 4) {
            tft.setCursor(10, 85);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[3].nombreExterno);
          }
          if (indicePerifericos >= 5) {
            tft.setCursor(10, 110);
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            tft.setTextSize(3);
            tft.print(perifericos[4].nombreExterno);
          }
          tft.setCursor(10, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Agregar");
          tft.setCursor(140, 135);
          tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
          tft.setTextSize(3);
          tft.print("Volver");
          valorEncoder = 0;
          valorDisplay = 1;
          TFT_DISPLAY = PERIFERICO;
        }
      }
      break;
  }
}

void rotacionEncoder() {
  int valorCLK = digitalRead(ENCODER_A_PIN);
  int valorDT = digitalRead(ENCODER_B_PIN);

  if (valorCLK != ultimoValorCLK) {
    cambioEncoder = 1;
    if (valorDT != valorCLK) {
      valorEncoder--;
    } else {
      valorEncoder++;
    }
  }

  if (valorEncoder < 0) {
    valorEncoder = 80;
  } else if (valorEncoder > 80) {
    valorEncoder = 0;
  }

  ultimoValorCLK = valorCLK;
  ultimoValorDT = valorDT;
}

void botonEncoder() {
  if (debounceSWEncoder >= TIEMPO_DEBOUNCE) {
    debounceSWEncoder = 0;
    flagSWEncoder = 1;
  }
}

void onTimer() {
  tiempoAgregarPer += 1;
  debounceSWEncoder += 1;
  parpadeo += 1;
}