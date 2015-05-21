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

	hdInit();
	pwmInit();
	dbgSendRobotString("Reset");

	while(1)
	{
		cPMainCmdParser();
	}


}
