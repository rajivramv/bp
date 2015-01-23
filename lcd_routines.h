//Connections between uC and LCD
#define DATA_DDR	 DDRC
#define DATA_PORT 	 PORTC

#define CONTROL_DDR	     DDRA
#define CONTROL_PORT 	 PORTA
#define RS               PA5
#define RW 				 PA6
#define E	             PA7

//Functions for interfacing to a 16x2 LCD

void lcd_clear(void);
void lcd_home(void);
void lcd_command(unsigned char command);
void lcd_init(void);

void display_char(unsigned char data);
void display_byte(unsigned char num);
void display_int(unsigned int num);

void move_to(unsigned char x,unsigned char y);