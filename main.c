/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Tr�panier 


*/


#include "hardware.h"
#include "dbgCmd.h"
#include "cmdParser.h"



int main( void )
{

	hdInit();
	dbgSendString("Reset!!!");

	while(1)
	{
		cPMainCmdParser();
	}


}
