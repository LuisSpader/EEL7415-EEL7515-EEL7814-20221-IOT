// --------------------------------------- INICIO PREDEFINIÇÕES ---------------------------------------

// ------------- Bibliotecas -------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
// ------------- Mapeamento de Hardware -------------
#define pin_CE          D14
#define pin_CSN         D10
// ------------- Variáveis Globais -------------
float temperatura = 1;
float humidade_ar = 2;
int   humidade_solo_1 = 3;
int   humidade_solo_2 = 4;
int   chuva = 5;
int   solar = 6;
int   bateria = 7;
int   regulador = 8;
char  teste[32] = {0};
// ------------- Outras Configurações -------------
String token = "ee0c54f5-3ccc-4b80-9901-52ed139d36ca"; //o token vai aqui
const char* ssid = "Poco";
const char* password = "12345678";
WiFiClient client;
char server[] = "api.tago.io";
// Pinout do módulo RF:
RF24 radio(pin_CE, pin_CSN);
// Enderelo do canal de comunicação:
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
  delay(5000);
  
  // Encerra conexões em andamento para liberar o wifi:
  client.stop();
  
  // Cria a string que vai ser enviada ao tago:
  String postStr = "";
  String postData = "";
  String token_string = String("Device-Token: ")+ String(token);
  
  // Adiciona as variáveis em uma string:
  postData = String("{\"variable\":\"temperature\", \"value\":") + String(temperatura) + String(",\"unit\":\"C\"}");
  postData = String("{\"variable\":\"teste\", \"value\":") + String(123) + String(",\"unit\":\"C\"}");

  if (client.connect(server,80)) {                      // we will use non-secured connnection (HTTP) for tests
  Serial.println("connected to server");
  // Make a HTTP request:
  client.println("POST /data? HTTP/1.1");
  client.println("Host: api.tago.io");
  client.println("_ssl: false");                        // for non-secured connection, use this option "_ssl: false"
  client.println(token_string);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
