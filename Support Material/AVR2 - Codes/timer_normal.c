//This code will blink the PIN13 LED at one sec.
//After approximately 61 overflows of the Timer/Counter 1 second elapses

#include <avr/io.h>
#include <avr/interrupt.h>
volatile int counter = 0;

void Timer0Init(void){
  TCCR0A = 0b00000000;
  TCCR0B = 0b00000101;        //Prescaler set to 1024: CS12 and CS10
  TIMSK0 = 0b00000001;        //Enable Timer Overflow Interrupt
}

void togglePin(void){
  PORTB = ~PORTB;
}

ISR (TIMER0_OVF_vect){
  counter++;
  if(counter == 61){
    togglePin();
    counter = 0;
  }
}



int main(void){
  sei();                    //Enable Global Interrupts
  DDRB = 0xFF;              //PORTB as output
  PORTB = 0xFF;
  Timer0Init();
  while(1){;}
  return 0;
}