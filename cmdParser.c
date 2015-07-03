///////////////////////////////////////////////////////////////////////////
/*
	cmdParser.c
 	ELE542 - ÉTÉ2015
 	Jonathan Lapointe LAPJ05108303
 	Charles Trépanier TREC07029107

	Contains functions for processing commands received from remote control.
*/
///////////////////////////////////////////////////////////////////////////

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

	if(uartGetRxSize())		//if uart fifo not empty
	{
		data=uartGetByte();
		uartSendByte(data);		//acknowlegde by sending the received byte
 	  	switch(cPState)
 		{
	 		case CP_SYNC_STATE:		

				if(data==CP_CMD_NORMALE||data==CP_CMD_ARRET)	//if known command type
				{			
					cPCmdValue=data;
					cPState=CP_GET_VITESSE_STATE;
				}

			break;

			case CP_GET_VITESSE_STATE:							//get speed byte
				cPVitesseValue=data;
				cPState=CP_GET_ANGLE_STATE;			

			break;


			case CP_GET_ANGLE_STATE:							//get angle byte	

				cPAngleValue=data;
				

				if(cPCmdValue==CP_CMD_NORMALE)
					cPState=CP_RUN_STATE;						//normal mode
				else
					cPState=CP_ARRET_STATE;						//emergency stop
			break;

			case CP_ARRET_STATE:								//emergency stop

				cPState=CP_SYNC_STATE;
				(void)moteurControl(cPVitesseValue,cPAngleValue,M_ARRET);

			break;

			case CP_RUN_STATE:									//normal mode

				cPState=CP_SYNC_STATE;
				(void)moteurControl(cPVitesseValue,cPAngleValue,M_MARCHE);	//controls motor according to received command

			break;

			default:
				cPState=CP_SYNC_STATE;
			break;


 		}

	}


}
