/*
 ELE542 
 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 
*/

#include "hardware.h"
#include "uart.h"
#define CP_CMD_NORMALE 0xf1
#define CP_CMD_ARRET   0xf0

typedef enum eCPState
{
	CP_SYNC_STATE,
	CP_SET_VITESSE_STATE,
	CP_SET_ANGLE_STATE,
	CP_ARRET_STATE,
	CP_ECHO_STATE
}tCPState;


U8 cPState=CP_SYNC_STATE;
U8 cPCmdValue=0;
U8 cPVitesseValue=0;
U8 cPAngleValue=0;
U8 data;





void cPMainCmdParser(void)
{

	if(uartGetRxSize())
	{
		data=uartGetByte();
 		switch(cPState)
 		{
 		case CP_SYNC_STATE:
			if(data==CP_CMD_NORMALE||data==CP_CMD_ARRET)
			{			
				uartSendByte(data);
				cPCmdValue=data;
				cPState=CP_SET_VITESSE_STATE;
			}
		break;

		case CP_SET_VITESSE_STATE:
			
			if(cPVitesseValue!=data&&data==0)
				dbgSendString("vitesse=0");

			cPVitesseValue=data;
			uartSendByte(data);
			cPState=CP_SET_ANGLE_STATE;
			
			

		break;


		case CP_SET_ANGLE_STATE:
			cPAngleValue=data;
			uartSendByte(data);
			cPState=CP_SYNC_STATE;
			PORTB=~cPAngleValue;


		break;

		case CP_ARRET_STATE:

			cPState=CP_SYNC_STATE;
			dbgSendString("arret");



		break;

		default:
			cPState=CP_SYNC_STATE;
		break;


 		}

	}


}
