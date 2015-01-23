//PWM Example Code
//Connect Pin 6 to Pin 13 using a wire. The PWM output is obtained on PIN 6.

#include <avr/io.h>

void InitPWM(void){
  TCCR0A = 0b10000011;    //Refer Datasheet for these values
  TCCR0B = 0b00000101;
  OCR0A = 0;
}

void SetPWM(uint8_t val){
  OCR0A = val;            //PWM value is to be written to this register - 0 to 255
}

int main(void){
  DDRD = 0xFF;            //PORTD as output. Pin 6 is the PWM output and is on PORTD
  DDRB = 0x00;            //PORTB as input
  PORTB = 0x00;           //Disables Pull Ups on PORTB
  InitPWM();
  SetPWM(12);
  while(1){}
  return 0;
}