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
#include <DNSServer.h>
#include <ESP8266WebServer.h>
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
String apiKey = "0dcdb8e3-2d69-403d-ab51-0e52aa4ec860"; //o token vai aqui
const char* ssid = "Thomé Suítes Externas";
const char* password = "senha123";
WiFiClient client;
const char* server = "api.tago.io";
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
  //Read the data if available in buffer
  if (radio.available()){
    // Envia os dados pelo módulo NRF24L01:
    radio.read(&teste, sizeof(teste));
    
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
    
    //Inicia um client TCP para o envio dos dados
    if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
      
      // Conectado ao tagoIO
      Serial.print("CONNECTED AT TAGO\n");
      
      // Cria a string que vai ser enviada ao tago:
      String postStr = "";
      
      // Adiciona as variáveis em uma string:
      String PostData = "[{\"variable\":\"solar\", \"value\":\"";
      PostData += String(solar);
      PostData = "\"},{\"variable\":\"bateria\",\"value\":\"";      
      PostData += String(bateria);
      PostData = "\"},{\"variable\":\"regulador\",\"value\":\"";      
      PostData += String(regulador);
      PostData = "\"},{\"variable\":\"temperatura\",\"value\":\"";      
      PostData += String(temperatura);
      PostData = "\"},{\"variable\":\"humidade_ar\",\"value\":\"";      
      PostData += String(humidade_ar);
      PostData = "\"},{\"variable\":\"humidade_solo1\",\"value\":\"";      
      PostData += String(humidade_solo1);
      PostData = "\"},{\"variable\":\"humidade_solo2\",\"value\":\"";      
      PostData += String(humidade_solo2);
      PostData = "\"},{\"variable\":\"chuva\",\"value\":\"";      
      PostData += String(chuva);
      PostData += "\"}]";
      
      // Cria o header para envio dos dados:
      postStr = "POST /data HTTP/1.1\n";
      postStr += "Host: api.tago.io\n";
      postStr += "Device-Token: "+apiKey+"\n";
      postStr += "_ssl: false\n";
      postStr += "Content-Type: application/x-www-form-urlencoded\n";
      postStr += "Content-Length: "+String(postData.length())+"\n";
      postStr += "\n";
      
      // Adiciona as variáveis no string com o header:
      postStr += postData;
      
      // Envia a string ao servidor:
      client.print(postStr);
      
      // DEBUGAR A STRING ENVIADA:
      //Serial.print("%d", postStr);
      
      // Salva o tempo atual
      unsigned long timeout = millis();
      
      // Timeout - Erro de comunicação:
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
      
      // 
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      
      // Encerra as comunicações:
      client.stop();
      
    }
  }
} // Fim do LOOP
