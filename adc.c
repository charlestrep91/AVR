/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/
#include "adc.h"
#include "moteur.h"
#include "uart.h"
#include "hardware.h"
#include <avr/interrupt.h>
#include "dbgCmd.h"


#define ADC_MOTEUR_GAUCHE 	0x00
#define ADC_MOTEUR_DROIT  	0x01
#define ADC_MUX_SETTTING  	(0<<REFS1)|(0<<REFS0)|(0<<ADLAR)
#define ADC_NEG_VALUE 	  	1		
#define ADC_POS_VALUE     	0 
#define ADC_NB_SAMPLE_MAX 	32

volatile U8  adcMuxState	=0;
volatile S16 adcMoteurGAvg	=0;
volatile S16 adcMoteurDAvg	=0;
volatile U16 adcNbSamples	=0;
volatile tREG08 adcPortAREG;	
volatile U16 Mcounter		=0;
volatile S16 adcValue		=0;
volatile U8 adcUpdateSetting=0;

#define ADC_DIR_GAUCHE_PIN  	adcPortAREG.bit.b2
#define ADC_DIR_DROIT_PIN   	adcPortAREG.bit.b3
	

/*
	@fn adcInit(void)
	@des init du peripherique adc
*/
void adcInit(void)
{
    
	ADMUX=ADC_MOTEUR_DROIT;	
 	ADCSRA=(1<<ADEN )
		  |(0<<ADSC )	
		  |(1<<ADATE)	
		  |(0<<ADIF )
		  |(1<<ADIE )
		  |(1<<ADPS2)
		  |(1<<ADPS1)
		  |(1<<ADPS0);	

	//init letat de la variable de lecture
   	adcMuxState=ADC_MOTEUR_DROIT;	
	SFIOR=0x0F&SFIOR;
}

/*
	@fn void adcStartConversion(void)
	@des demarre les lectures
*/
void adcStartConversion(void)
{
   ADCSRA |= 1<<ADSC;
}


/*
	@fn ISR(ADC_vect)
	@des routine d'interruption
*/
ISR(ADC_vect)
{
	ADMUX = (ADMUX & 0x01) ? (0<<MUX0) : (1<<MUX0);
	adcValue=(S16)(((ADCH&0x03)<<8)|ADCL);
	adcPortAREG.byte=PINA;
	adcUpdateSetting=1;	
	if(adcMuxState==ADC_MOTEUR_GAUCHE)
			adcMuxState=ADC_MOTEUR_DROIT;
		else
			adcMuxState=ADC_MOTEUR_GAUCHE;
}

void adcCalculateAvg(void)
{
	if(adcUpdateSetting==1)
	{
		
		adcUpdateSetting=0;
		//lecture du moteur Droit
		//routine d'accumulation des valeurs
		if(adcMuxState==ADC_MOTEUR_GAUCHE)
		{	
			//assignation de la valeur de mux pour la prochaine conversion
			adcNbSamples++;
			//addition ou soustraction de la valeur lue selon le bit de signe
			if(ADC_DIR_GAUCHE_PIN==ADC_NEG_VALUE)
				adcMoteurGAvg-=adcValue;
			else
				adcMoteurGAvg+=adcValue;
		}
		else if(adcMuxState==ADC_MOTEUR_DROIT)
		{
			//assignation de la valeur de mux pour la prochaine conversion
			adcNbSamples++;
			//addition ou soustraction de la valeur lue selon le bit de signe
			if(ADC_DIR_DROIT_PIN==ADC_NEG_VALUE)
				adcMoteurDAvg-=adcValue;
			else
				adcMoteurDAvg+=adcValue;
		
		}
		
		
	}
	//incrémente le compteur d'échantillon	
	//16 valeurs echantillonnées totales, mais 8 valeurs pour chacun des moteurs
	//routine de calcul de la moyenne et envoie a la fct d'asservisement moteur
	if(adcNbSamples>=ADC_NB_SAMPLE_MAX)
	{
		//division par 8 
		adcMoteurGAvg=adcMoteurGAvg>>4;
		adcMoteurDAvg=adcMoteurDAvg>>4;		
		//appel de la fonction dasservissement moteur
		moteurAsservissement(adcMoteurGAvg,adcMoteurDAvg);
		//reset des valeurs cumulés
		adcMoteurGAvg=0;
		adcMoteurDAvg=0;
		//Reset samples counter
		adcNbSamples=0;
	}

}


