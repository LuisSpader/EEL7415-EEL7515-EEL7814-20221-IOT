/*

Projeto IoT Hidroponia para Morangos Lajes - SC
Desenvolvido por: Vitor F. Thomé
Versão 1.0 // Agosto de 2021
Sensores: Humidade e Temperatura, Chuva, Luminosidade (Placas Solares), 2x Humidade do Solo, Tensão da Bateria, Tensão do Regulador 9V.
Comunicação: NRF24L01 Módulo Wireless 2.4GHz
Alimentação: 2x Placa solar 1W // 2x Bateria Samsung LiPo 18650 com 2600mAH // TP4056 Carregador de Bateria.

*/



// --------------------------------------- INICIO PREDEFINIÇÕES ---------------------------------------

// ------------- Bibliotecas -------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// ------------- Mapeamento de Hardware -------------
#define pin_CE          D14
#define pin_CSN         D10
// ------------- Variáveis Globais -------------
float temperatura = 0;
float humidade_ar = 0;
int humidade_solo_1 = 0;
int humidade_solo_2 = 0;
int chuva = 0;
int solar = 0;
int bateria = 0;
int regulador = 0;
char teste[32] = {0};
// ------------- Outras Configurações -------------
String apiKey = "3WGO6312X089C3UI";
const char* ssid = "Thomé Suítes Externas";
const char* password = "senha123";
WiFiClient client;
const char* server = "api.thingspeak.com";
// // Pinout do módulo RF://*

// Projeto IoT Hidroponia para Morangos Lajes - SC
// Desenvolvido por: Vitor F. Thomé
// Versão 1.0 // Agosto de 2021
// Sensores: Humidade e Temperatura, Chuva, Luminosidade (Placas Solares), 2x Humidade do Solo, Tensão da Bateria, Tensão do Regulador 9V.
// Comunicação: NRF24L01 Módulo Wireless 2.4GHz
// Alimentação: 2x Placa solar 1W // 2x Bateria Samsung LiPo 18650 com 2600mAH // TP4056 Carregador de Bateria.

// *//



// --------------------------------------- INICIO PREDEFINIÇÕES ---------------------------------------

// ------------- Bibliotecas -------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
// ------------- Mapeamento de Hardware -------------
#define pin_CE          D14
#define pin_CSN         D10
// ------------- Variáveis Globais -------------
float temperatura = 0;
float humidade_ar = 0;
int humidade_solo_1 = 0;
int humidade_solo_2 = 0;
int chuva = 0;
int solar = 0;
int bateria = 0;
int regulador = 0;
char teste[32] = {0};
// ------------- Outras Configurações -------------
String apiKey = "3WGO6312X089C3UI";

WiFiClient client;
const char* server = "api.thingspeak.com";
// Pinout do módulo RF:
RF24 radio(pin_CE, pin_CSN);
// Enderelo do canal de comunicação:
const byte address[6] = "00001";


// --------------------------------------- WiFi SETUP ---------------------------------------
// WiFi
const char* ssid = "Thomé Suítes Externas"; // Enter your WiFi name
const char* password = "senha123"; // Enter WiFi password


// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp8266/test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;


// --------------------------------------- MQTT Broker SETUP ---------------------------------------
// Set MQTT broker, write callback function, and print connection information on the serial monitor at the same time.
client.setServer(mqtt_broker, mqtt_port);
client.setCallback(callback);
while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}
// publish and subscribe
client.publish(topic, "hello emqx");
client.subscribe(topic);



// --------------------------------------- INICIO SETUP ---------------------------------------

void setup()
{
  while (!Serial);
    Serial.begin(9600);

  delay(10000);
  
  radio.begin();
  //set the address
  radio.openReadingPipe(0, address);
  //Set module as receiver
  radio.startListening();
  
  Serial.println("Inicializando o servidor.");
  Serial.print("Conectando ao wifi na rede: ");
  Serial.println(ssid);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
} // Fim do SETUP



// --------------------------------------- INICIO LOOP ---------------------------------------

void loop(){
  //Read the data if available in buffer
  if (radio.available()){
    // Envia os dados pelo módulo NRF24L01:
    radio.read(&teste, sizeof(teste));
    //radio.read(&teste, sizeof(teste));
    //radio.read(&teste, sizeof(teste));//address through which two modules communicate.

    //radio.read(&teste, sizeof(teste));
    //radio.read(&teste, sizeof(teste));
    Serial.println("Dados coletados e enviados ao thingspeak!");
    Serial.println("temperatura");
    Serial.println(teste);
    Serial.println("humidade_ar");
    Serial.println(teste);
    Serial.println("solar");
    Serial.println(teste);
    Serial.println("bateria");
    Serial.println(teste);
    Serial.println("regulador");
    Serial.println(teste);
    
    if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(solar);
      postStr += "&field2=";
      postStr += String(bateria);
      postStr += "&field3=";
      postStr += String(regulador);
      postStr += "&field4=";
      postStr += String(temperatura);
      postStr += "&field5=";
      postStr += String(humidade_ar);
      postStr += "&field6=";
      postStr += String(humidade_ar);
      postStr += "&field7=";
      postStr += String(humidade_ar);
      postStr += "&field8=";
      postStr += String(humidade_ar);
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
} // Fim do LOOP

const byte address[6] = "00001";



// --------------------------------------- INICIO SETUP ---------------------------------------

void setup()
{
  while (!Serial);
    Serial.begin(9600);

  delay(10000);
  
  radio.begin();
  //set the address
  radio.openReadingPipe(0, address);
  //Set module as receiver
  radio.startListening();
  
  Serial.println("Inicializando o servidor.");
  Serial.print("Conectando ao wifi na rede: ");
  Serial.println(ssid);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
} // Fim do SETUP



// --------------------------------------- INICIO LOOP ---------------------------------------

void loop(){
  //Read the data if available in buffer
  if (radio.available()){
    // Envia os dados pelo módulo NRF24L01:
    radio.read(&teste, sizeof(teste));
    //radio.read(&teste, sizeof(teste));
    //radio.read(&teste, sizeof(teste));//address through which two modules communicate.

    //radio.read(&teste, sizeof(teste));
    //radio.read(&teste, sizeof(teste));
    Serial.println("Dados coletados e enviados ao thingspeak!");
    Serial.println("temperatura");
    Serial.println(teste);
    Serial.println("humidade_ar");
    Serial.println(teste);
    Serial.println("solar");
    Serial.println(teste);
    Serial.println("bateria");
    Serial.println(teste);
    Serial.println("regulador");
    Serial.println(teste);
    
    if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(solar);
      postStr += "&field2=";
      postStr += String(bateria);
      postStr += "&field3=";
      postStr += String(regulador);
      postStr += "&field4=";
      postStr += String(temperatura);
      postStr += "&field5=";
      postStr += String(humidade_ar);
      postStr += "&field6=";
      postStr += String(humidade_ar);
      postStr += "&field7=";
      postStr += String(humidade_ar);
      postStr += "&field8=";
      postStr += String(humidade_ar);
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
} // Fim do LOOP
