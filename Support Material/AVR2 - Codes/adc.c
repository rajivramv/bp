//This code demonstrates usage of the Analog-to-Digital Peripheral of AVR Microcontrollers
//This code stores the 8-bit ADC result in a variable named adcValue - this variable can be used for other purposes


#include <avr/io.h>
#include <util/delay.h>


void InitADC(void)
{   
	DDRC=0;																//Set Port C as input. ADC is connected to PORTC in ATMega328
	PORTC=0;
	ADMUX=(1<<REFS0)|(1<<ADLAR);                                    	//For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);    	//Prescalar div factor =128
}


uint8_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-5 in case of ATMega328
	ch=ch&0b00000111;
	ADMUX=0;
	ADMUX=(1<<REFS0)|(1<<ADLAR);
	ADMUX+=ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while((ADCSRA & (1<<ADSC)));

	return(ADCH);
}



int main(void)
{
	InitADC();
	int adcValue;
	
	while(1){
		adcValue = ReadADC(1);
	}
	return 0;
}
