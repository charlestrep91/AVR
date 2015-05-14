/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/


#include "dbgCmd.h"
#include "uart.h"
#include "hardware.h"





void dbgSendString(U8 *buf)
{	
		uartSendByte(0xFE);
		uartSendString(buf);
		uartSendByte(0xFF);
}

