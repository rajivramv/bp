//This code toggles the PB5 pin on PORTB after every 500 ms


#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	DDRB = _BV(DDB5);
	while(1)
	{
        PORTB |= _BV(PB5);
		_delay_ms(500);
        PORTB &= ~(_BV(PB5));
        _delay_ms(500);
		
	}
	return 0;
}
