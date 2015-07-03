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
#include "i2c2.h"

volatile U8 flag5ms=0;
U8 count5ms=0;

int main( void )
{
	//disable interrupts
	cli();	
	//INITS				
	hwInit();
//	uartInit();
	pwmInit();	
//	adcInit();
//	WdInit();
//	WdDisable();
	adcStartConversion();
	//enable interrupt
	Init_TWI();
	sei();
	//calibration des vitesses
//	adcCalibSeq();
	//attend que la sw Start soit appuyer 
	//pour lancer le reste du programme	
	SLWaitForTheStartSw();

//	WD_RESTART_WATCHDOG;
	
	
	while(1)
	{
		if(flag5ms)
		{
			count5ms++;
			if(count5ms>=10)
			{
				count5ms=0;
				Ping_sensor();
			}
		}
		SLCheckSwStatus(); //verifie la switch d'arret
		cPMainCmdParser(); //Machine à état communication
		CalculMoteur();	   //calculPWM et autre			
	}
}
