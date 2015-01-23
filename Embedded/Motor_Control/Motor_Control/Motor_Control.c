/*
 * Motor_Control.c
 *
 * Created: 01-05-2012 PM 04:37:57
 *  Author: Karan
 */ 

#include <avr/io.h>
#include <util/delay.h>

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
	}
}

void InflateCuff()
{
	DDRB = 0x00001100;
	PORTB.2 = 1; 
		
}

Void DeflateFast()
{
	DDRB = 0x00001100;
	PORTB.2 = 0;
	
}

Void DeflateSlow()
{
	DDRB = 0x00001100;
	PORTB.2 = 0;
	DDRB |= _BV(PB3);

}