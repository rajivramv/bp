/*
 * testrun2.c
 *
 * Created: 28-04-2012 PM 03:22:03
 *  Author: Karan
 */ 

#include <avr/io.h>
#include "lcd.h"

int main(void)
{
	DDRD = 0x0ff;
	DDRC = 0x0ff;	   
	LCD_init();
    LCD_wait();
	while(1)
    {
       LCD_write("Testing wtf!!");
    }
}