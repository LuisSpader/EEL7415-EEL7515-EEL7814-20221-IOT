#include <ESP8266WiFi.h>
 
String apiKey = "3WGO6312X089C3UI";
 
const char* ssid = "Thomé Suítes Externas";
const char* password = "senha123";
 
WiFiClient client;
 
const char* server = "api.thingspeak.com";
 
void setup()
{
  Serial.begin(115200);
  Serial.print("Receiver Started, ID: ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
 
void loop(){
  
  Serial.println("--- Criando string com os dados ---");
  if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += "1";
    postStr += "&field2=";
    postStr += "2";
    postStr += "&field3=";
    postStr += "3";
    postStr += "&field4=";
    postStr += "4";
    postStr += "&field5=";
    postStr += "5";
    postStr += "&field6=";
    postStr += "6";
    postStr += "&field7=";
    postStr += "7";
    postStr += "&field8=";
    postStr += "8";
    postStr += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
}
