/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/

#include "pwm.h"
#include "hardware.h"
#include "dbgCmd.h"
#include "moteur.h"

volatile U16 pwmOCR1A_value=0;
volatile U16 pwmOCR1B_value=0;
tREG08 pwmPortDREG;
U16 period=10000;

void pwmInit(void)
{
  //com1b1=1
  TCCR1A=(1<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);
  TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
  //overflow interruption on
  TIMSK=(0<<TICIE1)|(0<<OCIE1A)|(0<<OCIE1B)|(1<<TOIE1);

  //ajustement de la periode de 5mS
  ICR1=0x2710;
  OCR1A=0;
  OCR1B=0;
}

void pwmSetDutyValue(U16 valueD,U16 valueG,U8 portValue)
{
	pwmOCR1B_value=valueG;
	pwmOCR1A_value=valueD;
	pwmPortDREG.byte=portValue;
/*
	dbgSendRobotString("pwmG:");
	dbgSendDbgU16ToDec(pwmOCR1B_value);

	dbgSendRobotString("pwmD:");
	dbgSendDbgU16ToDec(pwmOCR1A_value);
	*/
}


ISR(TIMER1_OVF_vect)
{
	CalculMoteur();
	OCR1A=pwmOCR1A_value;
	OCR1B=pwmOCR1B_value;
	pwmPortDREG.bit.b4=1;
	pwmPortDREG.bit.b5=1;
	PORTD=pwmPortDREG.byte;
}
