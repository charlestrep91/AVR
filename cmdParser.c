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
U8 cPCmdValue;
U8 cPVitesseValue;
U8 cPAngleValue;





void cPMainCmdParser(void)
{
U8 data;
	if(uartGetRxSize())
	{
		data=uartGetByte();
 		switch(cPState)
 		{
 		case CP_SYNC_STATE:

			if(data==CP_CMD_NORMALE)
			{
				cPState=CP_SET_VITESSE_STATE;
				uartSendByte(data);
			}
			else if(data==CP_CMD_ARRET)
			{
				cPState=CP_ARRET_STATE;
				uartSendByte(data);
			}
			cPCmdValue=data;

		break;

		case CP_SET_VITESSE_STATE:

			cPVitesseValue=data;
			uartSendByte(data);
			cPState=CP_SET_ANGLE_STATE;
			PORTB=cPVitesseValue;
		//	dbgSendString("vitesse");

		break;


		case CP_SET_ANGLE_STATE:
			cPAngleValue=data;
			uartSendByte(data);
			cPState=CP_SYNC_STATE;


		break;

		case CP_ARRET_STATE:
			uartSendByte(data);

			cPState=CP_SYNC_STATE;


		break;

		default:
			cPState=CP_SYNC_STATE;
		break;


 		}

	}


}
