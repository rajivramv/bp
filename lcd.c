#include <avr/io.h>
#include <avr/delay.h>
#include "lcd_routines.h"
#include "lcd_routines.c"
#include "adc_routines.h"
#include "adc_routines.c"

int main(void)
{   
	unsigned char a[] = {"M|"};
    unsigned char ADC_OP = 0;
	adc_init();
	lcd_init();
	while(1)
	{	
		_delay_ms(1000);
		lcd_home();
		for(unsigned char i =0;i<sizeof(a)-1;i++) display_char(a[i]); 
		ADC_OP = adc_read_ac();
		display_byte(ADC_OP);
	};
	
	
	return 0;
}