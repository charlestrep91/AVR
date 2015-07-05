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
//		uartSetSendByteEnable(0);
		//SEND START BYTE
		uartSendByte(0xFE);
		//SEND STRINGS
		uartSendString(buf);
		//SEND END BYTE
		uartSendByte(0xFF);
}

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

void dbgSendRobotString(U8 *buf)
{	
	//	uartSetSendByteEnable(0);
		//SEND START BYTE
		uartSendByte(0xFE);
		//SEND STRINGS
		uartSendString((U8*)"Robot: ");
		uartSendString(buf);
		//SEND END BYTE
		uartSendByte(0xFF);
}
