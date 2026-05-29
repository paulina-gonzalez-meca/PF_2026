#define MAX_LISTA 20

String dato;
String listaDatos[MAX_LISTA];
String tramaFinal;

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
  int i = 0;

  if (Serial.available() > 0) {
    dato = Serial.readString();  // Ingresamos variables
    if (i < MAX_LISTA) {
      listaDatos[i] = dato;
      Serial.println(listaDatos[i]);
      i++;
    } else {
      tramaFinal = armarTrama(listaDatos, i);  // devuelve la trama final.
      i = 0;                                     // Se reinicia el programa.
    }
  }
}


// función
String armarTrama(String datos[], int tamano) {
  String tramaArmada;
  for (int q = 0; q < 3; q++) {
    if (datos[0] == apodosDisp[q]) {  // si el primer dato ingresado es correspondiente al apodo de un periférico
      tramaArmada = "#" + datos[0]; // ir armando la trama
      Serial.println(tramaArmada); 
    } else {
      return tramaArmada = "-1";  // error
    }
  }
}