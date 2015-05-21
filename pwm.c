/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/

#include "pwm.h"
#include "hardware.h"
#include "dbgCmd.h"

U16 pwmCompRegValue;


void pwmInit(void)
{
  U16 period=10000;
  TCCR1A=(1<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);
  TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
  //overflow interruption on
  TIMSK=(0<<TICIE1)|(0<<OCIE1A)|(0<<OCIE1B)|(1<<TOIE1);

  //ajustement de la periode de 5mS
  ICR1H=(U8)(period>>8);
  ICR1L=(U8) period;
  OCR1A=0;
  OCR1B=0;
}

void pwmSetDutyValue(U8 value)
{

}

void pwmCalculPwm(void)
{
	dbgSendDbgString("pwm");

}

ISR(TIMER1_OVF_vect)
{
	pwmCalculPwm();

}
