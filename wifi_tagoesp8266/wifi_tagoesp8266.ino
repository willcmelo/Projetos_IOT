

#include <ESP8266WiFi.h>


//#include <WiFiClientSecure.h>

// ========== start configuration ==========
// WiFi configuration
//const char* ssid = "LAB05"; //rede wifi
//const char* password = "senai1954"; //senha wifi
String oauth_token = "8aa13611-7a59-49ff-8de2-91cbb7f13dd3"; // token dispositivo
const char* host = "api.tago.io";
const int httpsPort = 80;

// ========== end configuration ============

WiFiClient client;

void setup() {
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println("LAB05");
  
  WiFi.begin("Anny","97343675");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
   
  

    String conteudo = String(random(0,40)); // gera número aleatório
    //String conteudo = String(t); 
    Serial.println("Enviando: " + conteudo);
    httpSend(conteudo);
    delay(5000); //envia a cada 5 segundos
  
}

void httpSend(String valor) {
  
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
  client.println(oauth_token);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(post_payload.length());
  client.println();
  client.println(post_payload);
              
  Serial.println("request sent");

  Serial.println("reply was:");
  Serial.println("==========");

//aguarda resposta
//  while (client.connected()) {
//    String line = client.readStringUntil('\n');
//    Serial.println(line);
//  }

  client.stop();
  Serial.println("==========");
  Serial.println("closing connection"); 
}
