#include <avr/io.h>
#include "lcdroutines.h"
int main(void)
{
unsigned char a[] = {"Hello World!"};
lcd_init();
for(unsigned char i =0;i<sizeof(a)-1;i++) display_char(a[i]);
while(1);
return 0;
}