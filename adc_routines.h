//Assignment of ADC pins
//#define DC_PIN		PA0
//#define AC_PIN		PA1
//Assignment of channels (considering channel and pin addressing is similar)
#define DC_CHAN		PA0
#define AC_CHAN		PA1

//Functions to use the ADC
void adc_init(void);
unsigned char adc_read_dc(void);
unsigned char adc_read_ac(void);