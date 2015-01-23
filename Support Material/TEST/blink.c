#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	DDRA = 0b11111111;
	DDRB = 0b11111111;
	DDRC = 0b11111111;
	DDRD = 0b11111111;
	while(1)
	{
		PORTA = 0b11111111;
		PORTB = 0b11111111;
		PORTC = 0b11111111;
		PORTD = 0b11111111;
		_delay_ms(2000);
        PORTA = 0b00000000;
        PORTB = 0b00000000;
		PORTC = 0b00000000;
		PORTD = 0b00000000;
		_delay_ms(1000);
		
	}
	return 0;
}
