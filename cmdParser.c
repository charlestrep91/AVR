/*
 ELE542 
 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 
*/

#include "hardware.h"
#include "uart.h"
#include "dbgCmd.h"
#include "moteur.h"
#include "Watchdog.h"

#define CP_CMD_NORMALE 0xF1
#define CP_CMD_ARRET   0xF0



typedef enum eCPState
{
	CP_SYNC_STATE,
	CP_GET_VITESSE_STATE,
	CP_GET_ANGLE_STATE,
	CP_ARRET_STATE,
	CP_RUN_STATE
}tCPState;

//variables locales
U8 cPState			=CP_SYNC_STATE;
U8 cPCmdValue		=0;
U8 cPVitesseValue	=0;
U8 cPAngleValue		=0;
U8 data				=0;





void cPMainCmdParser(void)
{

	if(uartGetRxSize())
	{
		data=uartGetByte();
		uartSendByte(data);
 	  	switch(cPState)
 		{
	 		case CP_SYNC_STATE:

				if(data==CP_CMD_NORMALE||data==CP_CMD_ARRET)
				{			
					cPCmdValue=data;
					cPState=CP_GET_VITESSE_STATE;
				}

			break;

			case CP_GET_VITESSE_STATE:
				cPVitesseValue=data;
				cPState=CP_GET_ANGLE_STATE;			

			break;


			case CP_GET_ANGLE_STATE:

				cPAngleValue=data;
				

				if(cPCmdValue==CP_CMD_NORMALE)
					cPState=CP_RUN_STATE;
				else
					cPState=CP_ARRET_STATE;
			break;

			case CP_ARRET_STATE:

				cPState=CP_SYNC_STATE;
				(void)moteurControl(cPVitesseValue,cPAngleValue,M_ARRET);

			break;

			case CP_RUN_STATE:

				cPState=CP_SYNC_STATE;
				(void)moteurControl(cPVitesseValue,cPAngleValue,M_MARCHE);

			break;

			default:
				cPState=CP_SYNC_STATE;
			break;


 		}

	}


}
