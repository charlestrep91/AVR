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

void dbgSendDbgU16ToDec(U16 value)
{
	U8 buffer[5]={0,0,0,0,0};
	U8 i=4;	
	while(value!=0)
	{
		buffer[--i]=value%10+0x30;
		value=value/10;
	}

	buffer[4]=0;
	dbgSendDbgString(&buffer[i]);
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
