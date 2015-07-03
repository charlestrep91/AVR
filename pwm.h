///////////////////////////////////////////////////////////////////////////
/*
	pwm.h
 	ELE542 - ÉTÉ2015
 	Jonathan Lapointe LAPJ05108303
 	Charles Trépanier TREC07029107

	Contains prototypes and definitions for pwm.c.
*/
///////////////////////////////////////////////////////////////////////////

#ifndef PWM_H
#define PWM_H
#include "hardware.h"



void pwmInit(void);		//initialize pwm module

void pwmSetDutyValue(U16 valueD,U16 valueG,U8 portValue);	//sets pwm registers and directions


#endif
