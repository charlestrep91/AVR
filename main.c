/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Tr�panier 


*/


#include "hardware.h"
#include "dbgCmd.h"
#include "uart.h"



int main( void )
{

U32 i;
DDRB = 0xff; 
sei();
uartInit();

for(;;)
{

		dbgSendString("allo debugdebug");
	for(i=0;i<0xffff;i++);
	
}


}
