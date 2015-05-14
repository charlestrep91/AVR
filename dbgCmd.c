/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/


#include "dbgCmd.h"
#include "uart.h"
#include "hardware.h"





void dbgSendDbgString(U8 *buf)
{	
		//SEND START BYTE
		uartSendByte(0xFE);
		//SEND STRINGS
		uartSendString("Debug: ");
		uartSendString(buf);
		//SEND END BYTE
		uartSendByte(0xFF);
}

void dbgSendRobotString(U8 *buf)
{	
		//SEND START BYTE
		uartSendByte(0xFE);
		//SEND STRINGS
		uartSendString("Robot: ");
		uartSendString(buf);
		//SEND END BYTE
		uartSendByte(0xFF);
}
