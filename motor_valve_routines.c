#include <avr/io.h>
#include <util/delay.h>

void Initializing_Port_Timer(void)
{
	DDRB |= _BV(PB2) | _BV(PB3);
	TCCR0 = _BV(WGM00) | _BV(WGM01);	//Fast PWM
	TCNT0 = 0;
	OCR0 = 0;
}

void inflate_cuff (void)
{
	Initializing_Port_Timer();
	PORTB |= _BV(PB2) | _BV(PB3); 
}

void deflate_fast (void)
{
	Initializing_Port_Timer();
	PORTB &= (~(_BV(PB2) | _BV(PB3)));
}

void deflate_slow (void)
{
	Initializing_Port_Timer();
	PORTB &= (~(_BV(PB2)));
	OCR0 = 170;
	TCCR0 |= _BV(CS00) | _BV(COM01);	//No prescaling, Non-inverting
}
