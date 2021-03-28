#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#include <WiFi.h>
#include <DHT.h>

String oauth_token1 = "8aa13611-7a59-49ff-8de2-91cbb7f13dd3"; // token dispositivo1
String oauth_token2 = "9bfdf2e8-3ab7-4dd6-bbbc-a7844415b74e"; // token dispositivo2
const char* host = "api.tago.io";
const int httpsPort = 80;

#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float localHum = 0;
float localTemp = 0;

BluetoothSerial SerialBT;
WiFiClient client;

void setup() {
  
  Serial.begin(115200);
  
  Serial.println();
  Serial.print("connecting to ");
  Serial.println("WIFI");
  
  WiFi.begin("Anny","97343675");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
  
  
}
String conteudo1 ;
String conteudo2 ;
 
void loop() {

  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
         
  getDHT();
  Serial.print("Temp: ==> ");
  Serial.print(localTemp);
  Serial.print(" Hum ==> ");
  Serial.println(localHum);
  delay(1000); 
   
    switch(SerialBT.read())      //verifica qual caracter recebido
    {
      case 'T':                  //caso 'T'
       conteudo1 = String(localTemp); 
        Serial.println("Enviando: " + conteudo1);
        
      break;

      case 'H':                  //caso 'H'
       conteudo2 = String(localHum); 
        Serial.println("Enviando: " + conteudo2);
       
      break;

      case 'L':                  //caso 'L'
       conteudo1 = String(localTemp); 
       conteudo2 = String(localHum); 
        Serial.println("Enviando: " + conteudo1);
        Serial.println("Enviando: " + conteudo2);
        
        
      break;

    }

        httpSend1(conteudo1);
        httpSend2(conteudo2);
        delay(5000); //envia a cada 5 minutos
}

void httpSend1(String valor) {
  
  String post_payload = "{\"variable\":\"teste\",\"value\":" + valor + "}";

  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  client.println("POST /data? HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("_ssl: false");
  client.print("Device-Token: ");
  client.println(oauth_token1);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(post_payload.length());
  client.println();
  client.println(post_payload);
              
  Serial.println("request sent");

  Serial.println("reply was:");
  Serial.println("==========");

//=======================================//

  client.stop();
  Serial.println("==========");
  Serial.println("closing connection"); 
}


void httpSend2(String valor) {
  
  String post_payload = "{\"variable\":\"teste\",\"value\":" + valor + "}";

  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  client.println("POST /data? HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("_ssl: false");
  client.print("Device-Token: ");
  client.println(oauth_token2);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(post_payload.length());
  client.println();
  client.println(post_payload);
              
  Serial.println("request sent");

  Serial.println("reply was:");
  Serial.println("==========");

//=======================================//

  client.stop();
  Serial.println("==========");
  Serial.println("closing connection"); 
}


void getDHT()
{
  float tempIni = localTemp;
  float humIni = localHum;
  localTemp = dht.readTemperature();
  localHum = dht.readHumidity();
  if (isnan(localHum) || isnan(localTemp)) // Check if any reads failed and exit early (to try again).
  {
    localTemp = tempIni;
    localHum = humIni;
    return;
  }
}
