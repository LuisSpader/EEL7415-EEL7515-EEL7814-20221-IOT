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
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
// ------------- Mapeamento de Hardware -------------
#define pin_Buzzer      3
#define pin_Temp        5
#define pin_Switch      7
#define pin_CE          9
#define pin_CSN         10
#define pin_Humidade_1  14
#define pin_Humidade_2  15
#define pin_Chuva       16
#define pin_Solar       17
#define pin_Bateria     A4
#define pin_Regulador   A5
//Tipo de sensor de humidade e temperatura:
#define DHTTYPE DHT22
//DHT 21: Para CI AM2301, cor preta
//DHT 22: Para CI AM2302, cor branca
// ------------- Constantes -------------
char teste[] = "teste";
// ------------- Variáveis Globais -------------
float solar = 0;
float bateria = 0;
float regulador = 0;
float temperatura = 0;
float humidade_ar = 0;
int humidade_solo_1 = 0;
int humidade_solo_2 = 0;
int chuva = 0;
int valor = 0;
// ------------- Funções -------------
void delaySleep(byte timer);  // Delay com deep sleep
void Buzina(int repeticoes, int tipo);  // Efeitos sonoros
// ------------- Outras Configurações -------------
// Pinout do módulo RF:
RF24 radio(pin_CE, pin_CSN);
// Enderelo do canal de comunicação:
const byte address[6] = "00001";
// Configurando o sensor de humidade e temperatura:
DHT dht(pin_Temp, DHTTYPE);



// --------------------------------------- INICIO SETUP ---------------------------------------

void setup () {
  
  // Economiza energia atribuindo nível lógico baixo aos pinos digitais não usados:
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  
  // Configuração de Pinos do Arduino:
  pinMode(pin_Buzzer,       OUTPUT);    // Pino Digital   3 - RXI ----- Pin 3
  pinMode(pin_Switch,       OUTPUT);    // Pino Digital   7 - D7 ------ Pin 7
  pinMode(pin_Humidade_1,   INPUT);     // Pino Analógico 0 - A0 ------ Pin 14
  pinMode(pin_Humidade_2,   INPUT);     // Pino Analógico 1 - A1 ------ Pin 15
  pinMode(pin_Solar,        INPUT);     // Pino Analógico 3 - A3 ------ Pin 17
  pinMode(pin_Chuva,        INPUT);     // Pino Analógico 2 - A2 ------ Pin 16
  pinMode(pin_Bateria,      INPUT);     // Pino Analógico 4 - A4 ------ Pin A4
  pinMode(pin_Regulador,    INPUT);     // Pino Analógico 5 - A5 ------ Pin A5
  
  // Configurações para o módulo NRF24L01:
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();  // Configura o módulo para transmissão

  // Configura o módulo DHT22:
  dht.begin();
  
  // Sinal de Áudio para o fim do void setup:
  Buzina(2,0);
  
  // Entra em DEEP SLEEP por 2 segundos:
  delay(5000);
  
} // Fim do SETUP



// --------------------------------------- INICIO LOOP ---------------------------------------

void loop () {
  
  // Faz a leitura de temperatura e humidade do sensor DHT22:
  temperatura = dht.readTemperature();
  humidade_ar = dht.readHumidity();

  // Faz a leitura das tensões de interesse:
  valor = analogRead(pin_Solar);
  solar = float(map(valor, 0, 1023, 0, 5000))/1000;
  valor = analogRead(pin_Bateria);
  bateria = float(map(valor, 0, 1023, 0, 5000))/1000;
  valor = analogRead(pin_Regulador);
  regulador = float(map(valor, 0, 1023, 0, 5000))/1000;
  
  // Envia os dados pelo módulo NRF24L01:
  radio.write(&teste, sizeof(teste));
  //radio.write(&teste, sizeof(teste));
  //radio.write(&teste, sizeof(teste));
  //radio.write(&teste, sizeof(teste));
  //radio.write(&teste, sizeof(teste));
  
  // Sinal de Áudio:
  Buzina(3,1);
  
  // Entra em DEEP SLEEP por 10 segundos:
  delaySleep(20);
  
} // Fim do LOOP



// --------------------------------------- FUNÇÕES PARA O EFEITO SONORO ---------------------------------------

void Buzina(int repeticoes, int tipo){
  if (tipo == 0){ // 0 = Bipes curta duração
    for(int Rep = 0; Rep < repeticoes; Rep++){
      digitalWrite(pin_Buzzer, HIGH);
      delay(50);
      digitalWrite(pin_Buzzer, LOW);
      delay(50);
    }
  }
  if (tipo == 1){ // 1 = Bipes longa duração
    for(int Rep = 0; Rep < repeticoes; Rep++){
      digitalWrite(pin_Buzzer, HIGH);
      delay(200);
      digitalWrite(pin_Buzzer, LOW);
      delay(200);
    }
  }
  else{
    for(int Rep = 0; Rep < repeticoes; Rep++){
      digitalWrite(pin_Buzzer, HIGH);
      delay(50);
      digitalWrite(pin_Buzzer, LOW);
      delay(50);
    }
  }
} // Fim de Buzina



// --------------------------------------- FUNÇÕES PARA O DEEP SLEEP ---------------------------------------

void delaySleep (int timer){
  
  // Desligando todos os módulos que não são usados do ATMega328p:
  // Desativa o Módulo NRF24L01:
  //radio.powerDown();
  // Desativando o Analog to Digital Converter (ADC):
  ADCSRA &= ~(1 << 7);
  // Desabilitando as funções da biblioteca AVR/power.h:
  #define power_adc_disable();
  #define power_usb_disable();
  #define power_usi_disable();
  #define power_usart0_disable();
  #define power_spi_disable();
  #define power_timer1_disable();
  #define power_timer2_disable();
  
  // ----- LOOP PRINCIPAL -----
  for (int i = 0; i<timer; i++){
    // Limpa algumas flags de reset:
    MCUSR = 0;
    // Permite mudanças e desabilita reset:
    WDTCSR = bit (WDCE) | bit (WDE);
    // Define o modo de interrupção e o tempo de duração:
    WDTCSR = bit (WDIE) | bit (WDP2) | bit (WDP1);    // Delay de 1 seg
    wdt_reset();  // Reseta o timer do watchdog
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    noInterrupts ();           // Sequência de comandos obrigatórios
    sleep_enable();  
    // Desliga o brown-out enable por software:
    MCUCR = bit (BODS) | bit (BODSE);
    MCUCR = bit (BODS);
    // Garante a execução da próxima instrução:
    interrupts ();
    // Entra em modo Sleep desligando a CPU:
    sleep_cpu ();  
    // Desabilita o Sleep como precaução:
    sleep_disable();
  }
  //Religando todos os módulos que foram desligados do ATMega328p:
  // Reabilitando as funções da biblioteca PPR:
  #define power_usb_enable();
  #define power_usi_enable();
  #define power_usart0_enable();
  #define power_usart1_enable();
  #define power_spi_enable();
  #define power_timer1_enable();
  #define power_timer2_enable();
  // Ativando o Analog to Digital Converter (ADC):
  ADCSRA &= (1 << 7);
  #define power_adc_enable();
  // Ativa o Módulo NRF24L01:
  //radio.powerUp();
} // Fim de delaySleep

// Tratador de Interrupções:
ISR (WDT_vect)
{
  wdt_disable();  // Desativa o watchdog timer
}  // Fim de WDT_vect --- Tratador interrupções
