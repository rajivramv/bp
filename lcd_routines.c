//8-bit interface for LCD
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_routines.h"

// Some LCD parameters and macros
#define delay             1
#define fastdelay        40
#define Set_Enable  		        CONTROL_PORT|=_BV(E)
#define Clear_Enable 		        CONTROL_PORT&=~_BV(E)
#define Select_InstructionRegister 	CONTROL_PORT&=~_BV(RS)
#define Select_DataRegister	   	    CONTROL_PORT|=_BV(RS)
#define Data_Lcd(a)			        DATA_PORT=a

void lcd_clear(void)
{
	Select_InstructionRegister;
	Data_Lcd(0x01);
	Set_Enable;
	_delay_us(2);
	Clear_Enable;
	_delay_ms(2);
}

void lcd_home(void)
{
	Select_InstructionRegister;
	Data_Lcd(0x02);
	Set_Enable;
	_delay_us(2);
	Clear_Enable;
	_delay_ms(2);
}

void lcd_command(unsigned char command)
{
	Select_InstructionRegister;
	Data_Lcd(command);
	Set_Enable;
	_delay_us(1);
	Clear_Enable;
	_delay_us(fastdelay);
}

void lcd_init(void)
{
   DATA_DDR=0XFF; //Setting data port for output
   CONTROL_DDR |= _BV(E)| _BV(RS);//setting selected pins of control port for output
   CONTROL_PORT&= ~(_BV(E)|_BV(RS)); //setting values to 0 at starting
   CONTROL_DDR |= _BV(RW); //
   CONTROL_PORT |= ~(_BV(RW)); //
   

   _delay_ms(15);
   _delay_ms(15);
   char init[3];
   unsigned char i;
   init[0] = 0x38;// 8bitdata and 5x8 dot display
   init[1] = 0x0c;// enable display 
   init[2] = 0x06;// move cursor right after writing a character 
   for(i=0;i<3;i++) lcd_command(init[i]);
   lcd_clear();	
}

void display_char(unsigned char data)
{
	Select_DataRegister;
	Data_Lcd(data);
	Set_Enable;
	_delay_us(1);
	Clear_Enable;
	_delay_us(fastdelay);
}

void display_byte(unsigned char num)
{   unsigned char i = 0;
    while(num>=100) 
	{
		i++;
		num = num -100;
	}
	display_char(i+48);
	i=0;
	while(num>=10)
	{
		i++;
		num = num -10;
	}
	display_char(i+48);
	display_char(num + 48);
}

void display_int(unsigned int num)
{
   unsigned char i=0;
   while(num>=10000) {num = num - 10000; i++;}
   display_char(i+48);
   i=0;
   while(num>=1000)  {num = num - 1000; i++;}
   display_char(i+48);
   i=0;
   while(num>=100)   {num = num - 100; i++;}
   display_char(i+48);
   i=0;
   while(num>=10)    {num = num - 10; i++;}
   display_char(i+48);
   i = (unsigned char)num;
   display_char(i+48);
}

void move_to(unsigned char x,unsigned char y)
{
    unsigned char t;
	t = 128 + (y<<6) + x;
    lcd_command(t);
}


