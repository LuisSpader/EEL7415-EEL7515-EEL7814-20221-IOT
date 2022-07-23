//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>

String apiKey = "3WGO6312X089C3UI";
const char* ssid = "Thomé Suítes Externas";
const char* password = "senha123";
WiFiClient client;
const char* server = "api.thingspeak.com";

//create an RF24 object
RF24 radio(9, 10);  // CE, CSN
//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  while (!Serial);
    Serial.begin(115200);
  
  radio.begin();
  //set the address
  radio.openReadingPipe(0, address);
  //Set module as receiver
  radio.startListening();
  
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
  //Read the data if available in buffer
  if (radio.available()){
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
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
}
