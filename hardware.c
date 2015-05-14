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
	//enable all interrupts
	sei();
	//init Uart
	uartInit();	

}
