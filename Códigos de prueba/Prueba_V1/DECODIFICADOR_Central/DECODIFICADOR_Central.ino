#include <vector>

String trama = "";
String trama_proceso = "";
int ind = 0;

class per{
  public:
    String nombre;
    int emergencia;
    String apodoSensor1;
    String apodoSensor2;
    String apodoSensor3;
    float resultadoSensor1;
    float resultadoSensor2;
    float resultadoSensor3;

    per(String nombrePer, int emergenciaPer, String apodoSensor1Per, String apodoSensor2Per, String apodoSensor3Per, float resultadoSensor1Per, float resultadoSensor2Per, float resultadoSensor3Per){
      nombre = nombrePer;
      emergencia = emergenciaPer;
      apodoSensor1 = apodoSensor1Per;
      apodoSensor2 = apodoSensor2Per;
      apodoSensor3 = apodoSensor3Per;
      resultadoSensor1 = resultadoSensor1Per;
      resultadoSensor2 = resultadoSensor2Per;
      resultadoSensor3 = resultadoSensor3Per;
    }
};

typedef enum { //state machine
  COMANDO,
  APD,
  EMG,
  APD_S1,
  APD_S2,
  APD_S3,
  RSLT_S1,
  RSLT_S2,
  RSLT_S3
} PASOS_DECODIFICADOR_t;

PASOS_DECODIFICADOR_t PDECO;

std::vector<String> tramas;
std::vector<per> perifericos;

void setup() {
  // #Comando,ApodoDisp,EMG,ApodoSens1,ApodoSens2,ApodoSens3,ResSens1,ResSens2,ResSens3
  Serial.begin(9600);
  Serial.println("trama: #Comando,ApodoDisp,EMG,ApodoSens1,ApodoSens2,ApodoSens3,ResSens1,ResSens2,ResSens3");
  perifericos.push_back(per("perif1",0, "sensor1", "sensor2", "sensor3", 0, 0, 0));
  perifericos.push_back(per("perif2",1, "sensor1", "-", "-", 0, 0, 0));
  perifericos.push_back(per("perif3",0, "-", "-", "-", 0, 0, 0));
  tramas.reserve(5);

  PDECO = COMANDO;


}

void loop() {
  if(Serial.available()>0){
    trama = Serial.readString();
    Serial.println(trama);
    trama.trim();
  }
  decodificador();

}

void decodificador(){
  if(trama != ""){
    tramas.push_back(trama);
    trama = "";
  }
  if(tramas.empty() != true){
    switch(PDECO){
      case COMANDO:
      trama_proceso = tramas[0];
        if(trama_proceso.startsWith("#-,") == true){
          trama_proceso.remove(0,3);
          PDECO = APD;
        }
        else if(trama_proceso.startsWith("#/menu") == true){}
        else{
          Serial.println("error- comando");
          tramas.erase(tramas.begin());
        }
      break;
      case APD:
        if(perifericos[ind].nombre == trama_proceso.substring(0, trama_proceso.indexOf(","))){
          trama_proceso.remove(0, trama_proceso.indexOf(",")+1);
          PDECO = EMG;
        }
        else if(ind >= perifericos.size()){
          Serial.println("error- apodo");
          ind = 0;
          tramas.erase(tramas.begin());
          PDECO = COMANDO;
        }
        if(ind < perifericos.size() && PDECO != EMG){
          ind += 1;
        }
      break;
      case EMG:
        if(trama_proceso[0] == '0'){
          perifericos[ind].emergencia = 0;
          trama_proceso.remove(0, trama_proceso.indexOf(",")+1);
          PDECO = APD_S1;
        }
        else if(trama_proceso[0] == '1'){
          perifericos[ind].emergencia = 1;
          trama_proceso.remove(0, trama_proceso.indexOf(",")+1);
          PDECO = APD_S1;
        }
        else{
          Serial.println("error- emergencia");
          tramas.erase(tramas.begin());
          ind = 0;
          PDECO = COMANDO;
        }
      break;
      case APD_S1:
        if(perifericos[ind].apodoSensor1 == trama_proceso.substring(0, trama_proceso.indexOf(","))){
          trama_proceso.remove(0, trama_proceso.indexOf(",")+1);
          PDECO = APD_S2;
        }
        else{
          Serial.println("error- apodo 1");
          tramas.erase(tramas.begin());
          ind = 0;
          PDECO = COMANDO;
        }
      break;
      case APD_S2:
        if(perifericos[ind].apodoSensor2 == trama_proceso.substring(0, trama_proceso.indexOf(","))){
          trama_proceso.remove(0, trama_proceso.indexOf(",")+1);
          PDECO = APD_S3;
        }
        else{
          Serial.println("error- apodo 2");
          tramas.erase(tramas.begin());
          ind = 0;
          PDECO = COMANDO;
        }
      break;
      case APD_S3:
        if(perifericos[ind].apodoSensor3 == trama_proceso.substring(0, trama_proceso.indexOf(","))){
          trama_proceso.remove(0, trama_proceso.indexOf(",")+1);
          PDECO = RSLT_S1;
        }
        else{
          Serial.println("error- apodo 3");
            tramas.erase(tramas.begin());
            ind = 0;
            PDECO = COMANDO;
        }
      break;
      case RSLT_S1:
        if(perifericos[ind].apodoSensor1 == "-"){
          if(trama_proceso[0] == '-' && trama_proceso[1] == ','){
            trama_proceso.remove(0, 2);
            perifericos[ind].resultadoSensor1 = NULL;
            PDECO = RSLT_S2;
          }
          else{
            Serial.println("error- resultado 1");
            tramas.erase(tramas.begin());
            ind = 0;
            PDECO = COMANDO;
          }
        }
        else{
          if(trama_proceso[0] == '-' && trama_proceso[1] == ','){
            Serial.println("error- resultado 1");
            tramas.erase(tramas.begin());
            ind = 0;
            PDECO = COMANDO;
          }
          else{
            perifericos[ind].resultadoSensor1 = trama_proceso.substring(0, trama_proceso.indexOf(',')).toFloat();
            trama_proceso.remove(0, trama_proceso.indexOf(',')+1);
            PDECO = RSLT_S2;
          }
        }
      break;
      case RSLT_S2:
        if(perifericos[ind].apodoSensor2 == "-"){
          if(trama_proceso[0] == '-' && trama_proceso[1] == ','){
            trama_proceso.remove(0, 2);
            perifericos[ind].resultadoSensor2 = NULL;
            PDECO = RSLT_S3;
          }
          else{
            Serial.println("error- resultado 2");
            tramas.erase(tramas.begin());
            ind = 0;
            PDECO = COMANDO;
          }
        }
        else{
          if(trama_proceso[0] == '-' && trama_proceso[1] == ','){
            Serial.println("error- resultado 2");
            tramas.erase(tramas.begin());
            ind = 0;
            PDECO = COMANDO;
          }
          else{
            perifericos[ind].resultadoSensor2 = trama_proceso.substring(0, trama_proceso.indexOf(',')).toFloat();
            trama_proceso.remove(0, trama_proceso.indexOf(',')+1);
            PDECO = RSLT_S3;
          }
        }
      break;
      case RSLT_S3:
        if(perifericos[ind].apodoSensor3 == "-"){
          if(trama_proceso[0] == '-' && trama_proceso[1] == NULL){
            trama_proceso.remove(0, 2);
            perifericos[ind].resultadoSensor3 = NULL;
            tramas.erase(tramas.begin());
            Serial.println(perifericos[ind].nombre);
            Serial.println(perifericos[ind].emergencia);
            Serial.println(perifericos[ind].apodoSensor1);
            Serial.println(perifericos[ind].apodoSensor2);
            Serial.println(perifericos[ind].apodoSensor3);
            Serial.println(perifericos[ind].resultadoSensor1);
            Serial.println(perifericos[ind].resultadoSensor2);
            Serial.println(perifericos[ind].resultadoSensor3);
            ind = 0;
            PDECO = COMANDO;
          }
          else{
            Serial.println("error- resultado 3");
            tramas.erase(tramas.begin());
            ind = 0;
            PDECO = COMANDO;
          }
        }
        else{
          if(trama_proceso[0] == '-' && trama_proceso[0] == ','){
            Serial.println("error- resultado 3");
            tramas.erase(tramas.begin());
            ind = 0;
            PDECO = COMANDO;
          }
          else{
            perifericos[ind].resultadoSensor3 = trama_proceso.substring(0).toFloat();
            tramas.erase(tramas.begin());
            Serial.println(perifericos[ind].nombre);
            Serial.println(perifericos[ind].emergencia);
            Serial.println(perifericos[ind].apodoSensor1);
            Serial.println(perifericos[ind].apodoSensor2);
            Serial.println(perifericos[ind].apodoSensor3);
            Serial.println(perifericos[ind].resultadoSensor1);
            Serial.println(perifericos[ind].resultadoSensor2);
            Serial.println(perifericos[ind].resultadoSensor3);
            ind = 0;
            PDECO = COMANDO;
          }
        }
      break;
    }
  }
}