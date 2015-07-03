///////////////////////////////////////////////////////////////////////////
/*
	hardware.c
 	ELE542 - ÉTÉ2015
 	Jonathan Lapointe LAPJ05108303
 	Charles Trépanier TREC07029107

	Contains hardware related functions.
*/
///////////////////////////////////////////////////////////////////////////

#include "hardware.h"


#define HW_OUTPUT  1
#define HW_INPUT   0

void hwInit(void)
{
	//init leds port
	DDRB = 0xff; 
	DDRD = 0xfe;
	DDRA = 0x10;
	PORTB=0xff;
}
