//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(D9, D10);  // CE, CSN

// pacote para transmissão de dados:
float pacote[2] = {0};

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
    radio.read(&pacote, sizeof(pacote));
    Serial.println(pacote[0]);
    Serial.println(pacote[1]);
  }
}
