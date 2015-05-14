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
		//SEND START BYTE
		uartSendByte(0xFE);
		//SEND STRING
		uartSendString(buf);
		//SEND END BYTE
		uartSendByte(0xFF);
}

