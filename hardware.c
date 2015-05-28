/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 
*/
#include "hardware.h"
#include "uart.h"



void hdInit(void)
{
	//init leds port
	DDRB = 0xff; 
	DDRD = 0xfe;
	//enable all interrupts
	sei();
	//init Uart
	uartInit();	

}
