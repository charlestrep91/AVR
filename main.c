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



int main( void )
{
	tREG08 portBREG;
	cli();					//disable interrupts
	hwInit();
	uartInit();
	pwmInit();	
	adcInit();
	portBREG.byte=0xff;
	portBREG.bit.b0=0;
	PORTB=portBREG.byte;
	adcStartConversion();	
	sei();					//enable interrupts
	
	
	

	while(1)
	{
		cPMainCmdParser();
		uartSendData();
		CalculMoteur();		
	}


}
