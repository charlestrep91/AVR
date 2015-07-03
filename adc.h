/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/

#ifndef ADC_H
#define ADC_H

#include "hardware.h"



/*
	@fn adcInit(void)
	@dest init du peripherique adc
*/
void adcInit(void);


/*
	@fn void adcStartConversion(void)
	@des demarre les lectures
*/
void adcStartConversion(void);

void adcCalculateAvg(float * moteurD, float * moteurG);


void adcCalibSeq(void);







#endif
