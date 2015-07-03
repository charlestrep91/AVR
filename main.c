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



int main( void )
{
	//disable interrupts
	cli();	
	//INITS				
	hwInit();
	uartInit();
	pwmInit();	
	adcInit();
	WdInit();
	WdDisable();
	
	adcStartConversion();
	//enable interrupt
	sei();
	//calibration des vitesses
	adcCalibSeq();
	//attend que la sw Start soit appuyer 
	//pour lancer le reste du programme	
	SLWaitForTheStartSw();

	WD_RESTART_WATCHDOG;
	
	
	

	while(1)
	{
		
		SLCheckSwStatus(); //verifie la switch d'arret
		cPMainCmdParser(); //Machine à état communication
		CalculMoteur();	   //calculPWM et autre			
	}


}
