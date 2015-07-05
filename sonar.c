






#include "sonar.h"
#include "hardware.h"
#include "twi.h"



#define SONAR_MEASURE_CM 0x51
#define SONAR_GAIN_REG   	1
#define SONAR_PORTEE_REG 	2
#define SONAR_GAUCHE_POS    0
#define SONAR_DROIT_POS     1
#define SONAR_LED_G_PING	0x20
#define SONAR_LED_D_PING	0x08
#define SONAR_LED_G_DET  	0x10
#define SONAR_LED_D_DET 	0x04

#define SECURITY_LENGHT   60
#define SONAR_THRESHOLD   2000

U8 SonarStateTab[2]={SONAR_GAUCHE,SONAR_DROIT};
U8 portee=0x22;
U8 whichSonar=0;
U16 SonarVal[2]={0,0};
float ratioD;
float ratioG;
//porte en CM
void SonarAdjPortee(U8 portee)	
{

	twiWrite(SonarStateTab[0], SONAR_PORTEE_REG, portee);
	twiWrite(SonarStateTab[1], SONAR_PORTEE_REG, portee);

}
void SonarInit(void)
{
	twiWrite(SonarStateTab[0], SONAR_GAIN_REG, 12);
	twiWrite(SonarStateTab[1], SONAR_GAIN_REG, 1);
	twiWrite(SonarStateTab[0], SONAR_PORTEE_REG, 0x22);
	twiWrite(SonarStateTab[1], SONAR_PORTEE_REG, 0x22);
}


void SonarPing(void)
{

		if(twiGetInSize())
		{
			SonarVal[whichSonar]=(U16)getDataInBuf()<<8;
			SonarVal[whichSonar]+=getDataInBuf();
		}
		
		twiWrite(SonarStateTab[whichSonar], 0, SONAR_MEASURE_CM);
		//switch to the other sonar
		if(whichSonar==SONAR_DROIT_POS)
		{
			whichSonar=SONAR_GAUCHE_POS;
			PORTB|=SONAR_LED_D_PING;
			PORTB&=~SONAR_LED_G_PING;
		}
		else 
		{
			whichSonar=SONAR_DROIT_POS;
			PORTB|=SONAR_LED_G_PING;
			PORTB&=~SONAR_LED_D_PING;
		}

		twiRead(SonarStateTab[whichSonar],2);
		twiRead(SonarStateTab[whichSonar],3);
		
}
													

void SonarCorrectDest(float *Duty_D,float *Duty_G)
{
	U16 sonarG=SonarVal[SONAR_GAUCHE_POS];
	U16 sonarD=SonarVal[SONAR_DROIT_POS];

	/*ratioG = (sonarG<sonarD) ? ((sonarG/4.3f-1)/100)*2 
	: ((sonarG>sonarD) ? (0.8f) : (1.0f));
	ratioD = (sonarG>sonarD) ? ((sonarD/4.3f-1)/100)*2
	: ((sonarG<sonarD) ? (0.8f) : (1.0f));

	if((sonarG<SECURITY_LENGHT || sonarD<SECURITY_LENGHT) && (sonarG<=sonarD)){

		ratioG = 0.1;
		ratioD = 0.1;
	}
	else if((sonarG<SECURITY_LENGHT || sonarD<SECURITY_LENGHT) && (sonarG>sonarD))
	{
	

		ratioG = 0.1;
		ratioD = 0.1;
	}*/
	if(sonarG<SONAR_THRESHOLD||sonarD<SONAR_THRESHOLD)
	{
		
		if(sonarG>sonarD)
		{
			*Duty_G*=1.0f;
			*Duty_D*=-1.0f;
			PORTB&=~SONAR_LED_G_DET;
			PORTB|=SONAR_LED_D_DET;
		}
		else if(sonarG<sonarD)
		{
			*Duty_G*=-1.0f;
			*Duty_D*=1.0f;
			PORTB&=~SONAR_LED_D_DET;
			PORTB|=SONAR_LED_G_DET;
		}
		else if(sonarG==sonarD)
		{
			*Duty_G*=-1.0f;
			*Duty_D*=-1.0f;
		}
			
	}
	//*Duty_G*=ratioG;
	//*Duty_D*=ratioD;


}
