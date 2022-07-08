//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led_pin 3

int led = 0;

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  radio.begin();
  //set the address
  radio.openReadingPipe(0, address);
  //Set module as receiver
  radio.startListening();
  pinMode(led_pin, OUTPUT);
}

void loop()
{
  if (led ==0){
    digitalWrite(led_pin, LOW);
  }
  
  if (led == 3){
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);
    delay(500);
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);
    delay(500);
  }
  
  //Read the data if available in buffer
  if (radio.available())
  {
    radio.read(&led, sizeof(led));
  }
  
  // Sinal de Luz caso receba o sinal:
  if (led == 1){
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);
    delay(500);
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);
    delay(500);
  }
  
}
