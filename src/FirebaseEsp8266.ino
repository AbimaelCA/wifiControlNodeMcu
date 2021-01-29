#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define ssid "Izzi2" //Nombre del SSid Wifi
#define password "Maelo21583.1"  //Contraseña del WIFI
#define FIREBASE_HOST "prueba-55344.firebaseio.com"   //Direccion de la Base de datos
#define FIREBASE_AUTH "lINMfC4aQzEc0IZjAdOaHZQCrfZ9ynlzObFew3vi"  // Codigo secreto

FirebaseData firebaseData; //Conexion con base de datos
// Variables
int ledSalidas[] = {D1,D2,D3,D4};//Salidas del NodMCu
String nombreVariablesBD[] = {"/LED1","/LED2","/LED3","/LED4"};//Nombre de variables en BD
FirebaseJson json;//Varibale de tipo Json

//Inicializa el progragrama
void setup() {
  //Establece la velocidad de comunicacion
  Serial.begin(9600);
  //establece las salidas
  for(int i=0;i< 4;i++){
    pinMode(ledSalidas[i],OUTPUT);
    digitalWrite(ledSalidas[i],HIGH);
  }
  //Establece las  variables de conexion
  WiFi.begin(ssid, password);
  //Intenta conectar con el WIfi
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());
  //Accede con las credenciales de FireBase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
void loop() {
  ComunicacionFirebase();
}
//Método encargado de comunicarse con FireBase y activar desactivar las salidas del NodMcu
void ComunicacionFirebase(){
// Verifica si esta conecata al wifi
  if(WiFi.status()== WL_CONNECTED){ 
    //recorre el areglo ,cuenta en automatico cuantos datos existe 
    for (int i = 0; i < (sizeof(nombreVariablesBD)/sizeof(nombreVariablesBD[0])); i++){ 
      //Obiene el nombre de los datos de las varibales en la Base de datos 
      Firebase.getString(firebaseData,nombreVariablesBD[i]);
      //Valida que sea 1
      if(firebaseData.intData() == 1){
        //Enciende la salida correspondiente
        digitalWrite(ledSalidas[i],LOW);
      }
      //Valida que sea 0
      if(firebaseData.intData() == 0){
        //Apga la salida correspondiete
        digitalWrite(ledSalidas[i],HIGH);
      }
      delay(500);
    }
    delay(1000);
  }
}
