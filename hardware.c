/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 
*/
#include "hardware.h"
#include "uart.h"


#define HW_OUTPUT  1
#define HW_INPUT   0

void hwInit(void)
{
	//init leds port
	DDRB = 0xff; 
	DDRD = 0xfe;
	DDRA = 0x10;
}
