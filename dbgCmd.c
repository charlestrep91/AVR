///////////////////////////////////////////////////////////////////////////
/*
	dbgCmd.c
 	ELE542 - ÉTÉ2015
 	Jonathan Lapointe LAPJ05108303
 	Charles Trépanier TREC07029107

	Contains functions for sending debug information to the console.
*/
///////////////////////////////////////////////////////////////////////////


#include "dbgCmd.h"
#include "uart.h"
#include "hardware.h"




//Sends a debug string to the console
void dbgSendDbgString(U8 *buf)
{	
//		uartSetSendByteEnable(0);
		//SEND START BYTE
		uartSendByte(0xFE);
		//SEND STRINGS
		uartSendString(buf);
		//SEND END BYTE
		uartSendByte(0xFF);
}

//Converts a decimal value to an ascii string and send it to the debug console
void dbgSendDbgU16ToDec(U8 *buf,U16 value)
{
	U8 buffer[5]={0x30,0x30,0x30,0x30,0x30};
	U8 i=3;	
	//uartSetSendByteEnable(0);
	while(value!=0&&i!=0)
	{
		buffer[i--]=value%10+0x30;
		value=value/10;
	}
	buffer[4]=0;
	uartSendByte(0xFE);
	uartSendString(buf);
	uartSendString(&buffer[i]);
	uartSendByte(0xFF);

}

//Sends a debug string to the console with a header
void dbgSendRobotString(U8 *buf)
{	
	//	uartSetSendByteEnable(0);
		//SEND START BYTE
		uartSendByte(0xFE);
		//SEND STRINGS
		uartSendString("Robot: ");
		uartSendString(buf);
		//SEND END BYTE
		uartSendByte(0xFF);
}
