#include <avr/io.h>
#include <util/delay.h>

void Initializing_Port_Timer(void)
{
	DDRB |= _BV(PB2) | _BV(PB3);
	TCCR0 = _BV(WGM01);
	TCNT0 = 0;
	OCR0 = 0;
}

void InflateCuff(void)
{
	Initializing_Port_Timer();
	PORTB |= _BV(PB2) | _BV(PB3); 
}

void DeflateFast(void)
{
	Initializing_Port_Timer();
	PORTB &= (~(_BV(PB2) | _BV(PB3)));
}

void DeflateSlow(void)
{
	Initializing_Port_Timer();
	PORTB &= (~(_BV(PB2)));
	OCR0 = 255;
	TCCR0 |= _BV(CS00) | _BV(COM00);
}

int main(void)
{
    while(1)
	{   
		InflateCuff();
		_delay_ms(5000);
		DeflateFast();
		_delay_ms(5000);
		InflateCuff();
		_delay_ms(5000);
		DeflateSlow();
		_delay_ms(5000);
	};
	return(0);
}