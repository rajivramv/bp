#include <avr/io.h>
#include <avr/interrupt.h>

volatile int counter = 0;

void Timer0InitCTC(void){
  TCCR0A = 0b01000010;
  TCCR0B = 0b00000101;
  OCR0A = 250;
  TIMSK0 = 0b00000010;  
}

void togglePin(void){
  PORTB = ~PORTB;
}


ISR (TIMER0_COMPA_vect){
  counter++;
  if(counter == 63){
    togglePin();
    counter = 0;
  }
}


int main(void){
  sei();
  DDRB = 0xFF;
  PORTB = 0xFF;
  Timer0InitCTC();
  while(1){;}
  return 0;
}