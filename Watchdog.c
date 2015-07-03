/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/
#include "hardware.h"
#include "Watchdog.h"




void WdDisable(void)
{

	WDTCR |= (1<<WDTOE); 
	WDTCR &= ~(1<<WDE);

}




void WdInit(void)
{
	WDTCR=0x07;
}

