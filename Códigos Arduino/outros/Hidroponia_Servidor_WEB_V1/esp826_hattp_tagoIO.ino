// https://imasters.com.br/desenvolvimento/sensor-ds18b20-com-esp8266-e-tago-io
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <OneWire.h>
#include <DallasTemperature.h>
 
#define SENSOR_PIN D4
 
WiFiManager wifiManager;
WiFiClient client;
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);
 
float temp = 0.0;
 
String apiKey = "0dcdb8e3-2d69-403d-ab51-0e52aa4ec860"; //o token vai aqui
const char* server = "api.tago.io";
 
 
void setup() {
  Serial.begin(115200);
  sensors.begin();
  wifiManager.autoConnect("SensorTemperatura");
  
}
 
void envia_dados(void){
 
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  Serial.println(temp);  
 
  //Inicia um client TCP para o envio dos dados
  if (client.connect(server,80)) {
 
    Serial.print("CONNECTED AT TAGO\n");
 
    String postStr = "";
 
    String postData = "variable=temp&value="+String(temp)+"\n"; //enviando
 
    postStr = "POST /data HTTP/1.1\n";
    postStr += "Host: api.tago.io\n";
    postStr += "Device-Token: "+apiKey+"\n";
    postStr += "_ssl: false\n";
    postStr += "Content-Type: application/x-www-form-urlencoded\n";
    postStr += "Content-Length: "+String(postData.length())+"\n";
    postStr += "\n";
    postStr += postData;
 
    client.print(postStr);
    //Serial.print("%d", postStr);
 
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
 
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
 
  }
  client.stop();
 
}
 
// the loop function runs over and over again forever
void loop() {
    envia_dados();
    delay(30000);
}
