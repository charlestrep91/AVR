










#include "hardware.h"
#include "uart.h"



int main( void )
{

U32 i;
U8 data;
DDRB = 0xff; 
sei();
uartInit();

for(;;)
{
	if(uartGetRxSize())
	{
		data=uartGetByte();
		PORTB=data;
		uartSendString("CHAR : ",7);
		uartSendByte(data);
	}
	for(i=0;i<0xffff;i++);
	
}


}
