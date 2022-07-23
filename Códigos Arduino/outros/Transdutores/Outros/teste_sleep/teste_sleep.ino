#include <avr/sleep.h>
#include <avr/wdt.h>

void setup () {pinMode (3, OUTPUT);}

void loop(){
  for(int Rep = 0; Rep < 10; Rep++){
    digitalWrite(3, HIGH);
    delay(50);
    digitalWrite(3, LOW);
    delay(50);
  }
  delaySleep(10);
}

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
}

// Tratador de Interrupções:
ISR (WDT_vect)
{
  wdt_disable();  // Desativa o watchdog timer
}  // Fim de WDT_vect --- Tratador interrupções
