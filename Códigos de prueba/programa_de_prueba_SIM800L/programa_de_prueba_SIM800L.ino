#include <HardwareSerial.h>

HardwareSerial sim800l(2);

String mensajeSerial;

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600, SERIAL_8N1, 16, 17);



  Serial.println("---------------------------------------------");
  Serial.println("Iniciando...");
  delay(15000);


  //COMANDOS DE DIAGNÓSTICO
  sim800l.println("AT");
  delay(500); 

  /*Consulta funcionalidad del SIM800L
    Respuesta esperada:
    +CFUN: 1
    */
  sim800l.println("AT+CFUN?");
  delay(1000); 

  //Consulta el estado de la tarjeta SIM
  sim800l.println("AT+CPIN?");
  delay(1000); 

  /* Consulta el SMS Service Center Address
    Respuesta esperadacon Claro:
    +CSCA: "+543200000001",145
    */
  sim800l.println("AT+CSCA?");
  delay(1000); 

  /* Consulta la calidad de la señal 
    +CSQ: <rssi>,<ber>

    <rssi> Fuerza de la señal
      0: Señal demasiado débil o sin conexión
      1-31: Señales funcionales. Cuanto mayor, mejor
      99: Señal desconocida o no detectable
    
    <ber> 
      0-7: Señales medibles
      99: Desconocido o no detectable
    */
  sim800l.println("AT+CSQ");
  delay(2000); 

  /* Consulta el registro de red 
    Respuesta esperada: +CREG: 0-1,1

    +CREG: <mode>,<stat>
    
    <mode>
      0: no manda notificaciones cuando el registro cambia
      1: manda notificaciones cuando el registro cambia
      2: manda notificaciones cuando el registro cambia con extra info

    <stat>
      0: no registrado
      1: registrado (home)
      2: buscando
      3: registro denegado
      4: desconocido
      5: registrado (roaming)
    */
  sim800l.println("AT+CREG?");
  delay(2000); 


  //COMANDOS DE CONFIGURACIÓN
  sim800l.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  delay(1000);
  sim800l.println("AT+CMGF=1"); //confifuracion de formato de SMS (recibir y enviar)
  delay(1000);
  sim800l.println("AT+CNMI=2,2,0,0,0"); //configuracion de mensajes recibidos
  delay(1000);

  Serial.print(sim800l.readString());  
}

void loop() {
  if (Serial.available() > 0) {
    mensajeSerial = Serial.readString();
    //mensajeSerial.trim();

    if(mensajeSerial[0] == '%'){
      Serial.println(mensajeSerial);
      sim800l.println("AT+CMGS=\"+5491161386381\""); // numero de telefono
      delay(500);
      sim800l.print(mensajeSerial);
      delay(500);
      sim800l.write(0x1A);
      delay(5000);
    }
    else {
      sim800l.print(mensajeSerial);
    }
  }
  if (sim800l.available() > 0) {
    Serial.print(sim800l.readString()); 
  }
}
