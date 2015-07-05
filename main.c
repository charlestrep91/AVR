 /*
 ELE542 
 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 

*/


#include "hardware.h"
#include "dbgCmd.h"
#include "cmdParser.h"
#include "pwm.h"
#include "uart.h"
#include "adc.h"
#include "moteur.h"
#include "SwNLed.h"
#include "Watchdog.h"
#include "twi.h"
#include "sonar.h"

volatile U8 flag5ms=0;
U8 count5ms=0;

int main( void )
{

	//disable interrupts
	cli();	
	//INITS				
	hwInit();
	uartInit();
	pwmInit();	
	adcInit();
	twiInit();
	
	
	
	adcStartConversion();
	//enable interrupt
	sei();
	cli();
//	WD_RESTART_WATCHDOG;
//	WdInit();
	//calibration des vitesses
	adcCalibSeq();
	
	//attend que la sw Start soit appuyer 
	//pour lancer le reste du programme	
	SLWaitForTheStartSw();
	SonarInit();
	SonarAdjPortee(0x22);
	
	


	while(1)
	{
		if(flag5ms)
		{
			count5ms++;
			if(count5ms>=10)
			{
				count5ms=0;
				SonarPing();
			}
		}
		SLCheckSwStatus(); //verifie la switch d'arret
		cPMainCmdParser(); //Machine à état communication
		CalculMoteur();	   //calculPWM et autre			
	}


}
