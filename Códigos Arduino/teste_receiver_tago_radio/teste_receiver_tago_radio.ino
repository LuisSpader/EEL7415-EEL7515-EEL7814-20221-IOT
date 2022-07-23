// --------------------------------------- INICIO PREDEFINIÇÕES ---------------------------------------

// ------------- Bibliotecas -------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
// ------------- Mapeamento de Hardware -------------
#define pin_CE          D9
#define pin_CSN         D10
// ------------- Variáveis Globais Sensores -------------
float temperatura = 1;
float umidade_ar = 2;
int   umidade_solo_1 = 3;
int   umidade_solo_2 = 4;
int   chuva = 5;
int   solar = 6;
int   bateria = 7;
int   regulador = 8;
// ------------- Outras Configurações -------------
// --------------- WIFI ---------------
const char* ssid = "Thome";
const char* password = "12345678";
WiFiClient client;
char server[] = "api.tago.io";
// --------------- TAGO ---------------
String token = "ee0c54f5-3ccc-4b80-9901-52ed139d36ca"; //o token vai aqui
// Cria a string que vai ser enviada ao tago:
String token_string = String("Device-Token: ")+ String(token);
String postData = "";
String postStr = "";
// --------------- RÁDIO ---------------
// Cria o objeto para a classe do rádio:
RF24 radio(D9, D10);  // CE, CSN
// Enderelo do canal de comunicação:
const byte address[6] = "00001";
// pacote para transmissão de dados:
float pacote[2] = {0};


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
  
  //Read the data if available in buffer
  if (radio.available())
  {
    // LENDO VARIÁVEIS DO RÁDIO SALVAS NO BUFFER 
    radio.read(&pacote, sizeof(pacote));
    umidade_ar = pacote[0];
    temperatura = pacote[1];
    Serial.println(pacote[0]);
    Serial.println(pacote[1]);

    // Adiciona as variáveis em uma string:
    postData  = String("["); // abrindo array de dicionários
    postData += String("{\"variable\":\"temperatura\", \"value\":") + String(temperatura) + String(",\"unit\":\"C\"},");
    postData += String("{\"variable\":\"humidade\", \"value\":") + String(umidade_ar) + String(",\"unit\":\"%\"}");
    postData += String("]"); // fechando array de dicionários
    
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
}
