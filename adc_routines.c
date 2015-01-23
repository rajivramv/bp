#include <avr/io.h>
#include "adc_routines.h"

void adc_init(void)
{
	DDRA &= ~_BV(AC_CHAN);								// Set necessary ADC pins as input		
	PORTA &= ~_BV(AC_CHAN);							// Disable corresponding pull-ups
	ADMUX = _BV(REFS0) | _BV(ADLAR);				 	// Set AVCC as reference, results left adjusted
	ADCSRA = _BV(ADEN) | _BV(ADPS2)| _BV(ADPS1);   	// Enabling ADC, clearing flag, interrupt enable and auto-trigger, 1/2 div. factor
	ADMUX = (ADMUX & 0xe0) + AC_CHAN;					// Set to dummy read AC_SIG for initiation of analog circuit
	ADCSRA |= _BV(ADSC); 								// Start dummy AC_CHAN conversion to initialize analog circuits
	while((ADCSRA & _BV(ADSC)) != 0) {};
}
unsigned char adc_read_dc (void)
{
	DDRA &= ~_BV(DC_CHAN);			
	PORTA &= ~_BV(DC_CHAN);
	ADMUX = (ADMUX & 0xe0) + DC_CHAN;
	ADCSRA |= _BV(ADSC); 		
	while((ADCSRA & _BV(ADSC)) != 0) {};						
	return (ADCH);
}
unsigned char adc_read_ac (void)
{
	DDRA &= ~_BV(AC_CHAN);						
	PORTA &= ~_BV(AC_CHAN);	
	ADMUX = (ADMUX & 0xe0) + AC_CHAN;
	ADCSRA |= _BV(ADSC); 		
	while((ADCSRA & _BV(ADSC)) != 0) {};		
	return (ADCH);
}