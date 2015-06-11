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


#define ADC_MOTEUR_GAUCHE 	0
#define ADC_MOTEUR_DROIT  	1
#define ADC_MUX_SETTTING  	(0<<REFS1)|(0<<REFS0)|(0<<ADLAR)
#define ADC_NEG_VALUE 	  	1		
#define ADC_POS_VALUE     	0 
#define ADC_NB_SAMPLE_MAX 	512

volatile U8  adcMuxState	=0;
volatile S32 adcMoteurGAvg	=0;
volatile S32 adcMoteurDAvg	=0;
volatile U16 adcNbSamples	=0;
volatile tREG08 adcPortAREG;	

#define ADC_DIR_GAUCHE_PIN adcPortAREG.bit.b0
#define ADC_DIR_DROIT_PIN  adcPortAREG.bit.b1
	

/*
	@fn adcInit(void)
	@des init du peripherique adc
*/
void adcInit(void)
{
    SFIOR=0x0F&SFIOR;
	ADMUX=ADC_MUX_SETTTING|(ADC_MOTEUR_GAUCHE);	
 	ADCSRA=(1<<ADEN)|(0<<ADSC)|(1<<ADATE)|(0<<ADIF)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	
	//init letat de la variable de lecture
   	adcMuxState=ADC_MOTEUR_GAUCHE;	
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
    //affectation des valeurs du port à la variable de port
	adcPortAREG.byte=PINA;
	//lecture du moteur Droit
	if(adcMuxState==ADC_MOTEUR_DROIT)
	{
		//assignation de la valeur de mux pour la prochaine conversion
		ADMUX=ADC_MUX_SETTTING|ADC_MOTEUR_GAUCHE;
		adcMuxState=ADC_MOTEUR_GAUCHE;

		//addition ou soustraction de la valeur lue selon le bit de signe
		if(ADC_DIR_GAUCHE_PIN==ADC_NEG_VALUE)
			adcMoteurGAvg-=(((ADCH&0x03)<<8)|ADCL);
		else
			adcMoteurGAvg+=(((ADCH&0x03)<<8)|ADCL);

	

	}
	//lecture du moteur Gauche
	else
	{
		//assignation de la valeur de mux pour la prochaine conversion
		ADMUX=ADC_MUX_SETTTING|ADC_MOTEUR_DROIT;
		adcMuxState=ADC_MOTEUR_DROIT;		

		//addition ou soustraction de la valeur lue selon le bit de signe
		if(ADC_DIR_DROIT_PIN==ADC_NEG_VALUE)
			adcMoteurDAvg-=(((ADCH&0x03)<<8)|ADCL);
		else
			adcMoteurDAvg+=(((ADCH&0x03)<<8)|ADCL);
	}

	//incrémente le compteur d'échantillon
	adcNbSamples++;

	//512 valeurs echantillonnées totales, mais 256 valeurs pour chacun des moteurs
	if(adcNbSamples>=ADC_NB_SAMPLE_MAX)
	{
		//division par 256 
		adcMoteurGAvg=adcMoteurGAvg>>8;
		adcMoteurDAvg=adcMoteurDAvg>>8;
		//appel de la fonction dasservissement moteur
		moteurAsservissement((S16)adcMoteurGAvg,(S16)adcMoteurDAvg);
		//reset des valeurs cumulés
		adcMoteurGAvg=0;
		adcMoteurDAvg=0;
		//Reset samples counter
		adcNbSamples=0;

	}
}



