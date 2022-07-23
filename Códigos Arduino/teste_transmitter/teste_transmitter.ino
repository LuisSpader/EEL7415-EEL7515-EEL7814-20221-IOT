//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>;


//Constants
#define DHTPIN 2        // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define pin_chuva A0    // Pino do sensor de chuva
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

// pacote para transmissao de dados:
float pacote[3];

//create an RF24 object
RF24 radio(9, 10);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";



void setup()
{
  radio.begin();

  //set the address
  radio.openWritingPipe(address);

  //Set module as transmitter
  radio.stopListening();

  dht.begin();
}



void loop()
{
  pacote[0] = dht.readHumidity();
  pacote[1] = dht.readTemperature();
  pacote[2] = analogRead(pin_chuva);
  
  //Send message to receiver
  radio.write(&pacote, sizeof(pacote));
  
  delay(1000);
}
