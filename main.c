/*
 ELE542 
 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 

*/


#include "hardware.h"
#include "dbgCmd.h"
#include "cmdParser.h"
#include "pwm.h"



int main( void )
{
	tREG08 portBREG;
	hdInit();
	pwmInit();
	dbgSendRobotString("Reset");
	portBREG.byte=0xff;
	portBREG.bit.b0=0;
	PORTB=portBREG.byte;

	while(1)
	{
		cPMainCmdParser();
	}


}
