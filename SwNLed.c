///////////////////////////////////////////////////////////////////////////
/*
	SwNLed.c
 	ELE542 - ÉTÉ2015
 	Jonathan Lapointe LAPJ05108303
 	Charles Trépanier TREC07029107

	Contains functions related to the STK500's switches and LEDs.
*/
///////////////////////////////////////////////////////////////////////////

#include "hardware.h"
#include "SwNLed.h"
#include "Watchdog.h"
#define   SW_STOP_MSK  0x80 
#define   SW_START_MSK 0x40 
#define   LED_WAIT_LED 0x40
#define   LED_RUN_LED  0x80
#define   LED_COM_STATUS  0x02
#define   SW_STOP_THE_MOTORS 0xcc

void SLWaitForTheStartSw(void)		//blocks code until SW6 is pressed
{
	cli();
	PORTB|=LED_RUN_LED;
	PORTB&=~LED_WAIT_LED;;
	PORTD|=SW_STOP_THE_MOTORS;
	//Attend que Start soit appuyer
	while(PINA&SW_START_MSK);
	PORTB|=LED_WAIT_LED;
	PORTB&=~LED_RUN_LED;
	sei();
	
}

void SLCheckSwStatus(void)			//polls SW6 and blocks or resumes code
	if((PINA&SW_STOP_MSK)==0)
	{
		cli();
//		WdDisable();
		PORTB|=LED_RUN_LED;
		PORTB|=LED_COM_STATUS;
		PORTB&=~LED_WAIT_LED;	
		PORTD|=SW_STOP_THE_MOTORS;
		//Attend que Start soit appuyer
		while(PINA&SW_START_MSK);
		PORTB|=LED_WAIT_LED;
		PORTB&=~LED_RUN_LED;
		sei();
//		WD_RESTART_WATCHDOG;
	}

}
