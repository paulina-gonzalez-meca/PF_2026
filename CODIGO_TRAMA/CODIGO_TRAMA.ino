#define MAX_LISTA 6
#define CANT_DISPOSITIVOS 3
#define CANT_SENSORES 3

String dato;
String listaDatos[MAX_LISTA];
String tramaFinal;

int i = 0;

String apodosDisp[] = {
  "P1",
  "P2",
  "P3",
};
String apodosSensores[] = {
  "S1",
  "S2",
  "S3",
};

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    dato = Serial.readString();  // Ingresamos variables
    
    if (i < MAX_LISTA) {
      listaDatos[i] = dato;
      Serial.println(listaDatos[i]);
      i++;
      Serial.println(i);
    } else {
      tramaFinal = armarTrama(listaDatos, i);  // Devuelve la trama final.
      i = 0;                                   // ¡Corregido! Ahora sí se reinicia a 0.
      Serial.println(i);
      Serial.println(tramaFinal);
    }
  }
}

// Función corregida para revisar toda la lista antes de dar error
String armarTrama(String datos[], int tamano) {
  String tramaArmada;
  for (int q = 0; q < CANT_DISPOSITIVOS; q++) {
    if (datos[0] == apodosDisp[q]) {  // Si el primer dato coincide con un periférico
      tramaArmada = "#" + datos[0];   // Armar la trama
      Serial.println(tramaArmada);
      return tramaArmada;             // Retorna la trama exitosa
    }
  }
  return "-1"; // Error: Solo retorna -1 si terminó el bucle y no encontró coincidencias
}