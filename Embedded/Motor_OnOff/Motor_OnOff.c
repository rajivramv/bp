#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    	DDRB |= _BV(PB2);
		while(1)
    {
        PORTB |= _BV(PB2);
		_delay_ms(5000);
		PORTB &= (~(_BV(PB2)));
		_delay_ms(5000);
	}
}
