#include "moteur.h"
#include "hardware.h"
#include "pwm.h"

U8 lastVitesse=0;
U8 lastAngle=0;
U8 lastVitG=0;
U8 lastVitD=0;
U8 mMode=0;

float mVitesse_D=0;
float mAngle_D=0;
float mVg=0;
float mVd=0;
float mDuty_G=0;
float mDuty_D=0;
tREG08 mPortDREG;
#define M_DIR_G1 mPortDREG.bit.b2
#define M_DIR_G2 mPortDREG.bit.b3
#define M_DIR_D1 mPortDREG.bit.b6
#define M_DIR_D2 mPortDREG.bit.b7


void CalculPWM(float Vitesse_D, float Angle_D, float Vg, float Vd, float *Duty_G, float *Duty_D)
{

	/**
		Dans cette fonction, la valeur des duty cycle pour chaque moteur est calculé.  
		Ce calcul est effectué à l`aide de la vitesse désirée, de l`angle désriré ainsi 
		que de la vitesse (mesurée avec CalculVitesses) et l`angle actuel.
	*/
	static float Angle = 0.0;
	static float ErreurAngle = 0.0;
	static float W = 0.0;
	static float Old_W = 0.0;
	static float Vt     = 0.0;
	static float Ut     = 0.0;
	static float Ua     = 0.0;
	static int 	 Signe_Ua = 0;
	static int 	 Signe_Ut = 0;


	Vg = (Vg > 1.0) ? 1.0 : ((Vg < -1.0) ? -1.0 : Vg);  /* Regarde les limites (-1.0 à 1.0) */
	Vd = (Vd > 1.0) ? 1.0 : ((Vd < -1.0) ? -1.0 : Vd);  /* Regarde les limites (-1.0 à 1.0) */
		
	Old_W = W;
	W     = 0.5*(Vmax/RAYON)*(Vd-Vg);
	Vt    = 0.5*(Vd+Vg);

	Angle =  Angle + (0.5)*TS*(W+Old_W);
	Angle = (Angle > 2.0*Pi) ? Angle - 2*Pi : ((Angle < 0.0) ? Angle + 2*Pi : Angle); /* Angle entre 0 et 2 pi */
	ErreurAngle = ((Angle_D >= Pi + Angle) ? Angle_D - 2*Pi : ((Angle_D <= -Pi + Angle) ? Angle_D + 2*Pi : Angle_D))-Angle;

	Ut = -H11*Vt + H12*Vitesse_D;
	Ua = H21*ErreurAngle - H22*W;
			
	Signe_Ua = (Ua >= 0.0) ? 1 : -1;
	Signe_Ut = (Ut >= 0.0) ? 1 : -1;
	
	Ua = (Signe_Ua*Ua > 1.0) ? Signe_Ua*1.0 : ((Signe_Ua*Ua <= 0.05) ? 0.0 : Ua);
	Ut = (Signe_Ut*Ut > 1.0) ? Signe_Ut*1.0 : Ut;
	Ut = ((Signe_Ut*Ut) > (1.0 - Signe_Ua*Ua)) ? Signe_Ut*(1.0 - Signe_Ua*Ua) : Ut;


	*Duty_D = (Ut+Ua);
	*Duty_G = (Ut-Ua);

	*Duty_D = (*Duty_D > 0.99) ? 0.99 : ((*Duty_D < -0.99) ? -0.99 : *Duty_D);
	*Duty_G = (*Duty_G > 0.99) ? 0.99 : ((*Duty_G < -0.99) ? -0.99 : *Duty_G);	
}

void moteurControl(U8 vitesse,U8 angle,U8 mode)
{
U16 dutyValD;
U16 dutyValG;
    if(mode!=M_MARCHE)
	{
		switch(mode)
		{
			case M_NEUTRE:
			M_DIR_G1=0;
			M_DIR_G2=0;
			M_DIR_D1=0;
			M_DIR_D2=0;

			break;

			//M_ARRET
			default:  
			M_DIR_G1=1;
			M_DIR_G2=1;
			M_DIR_D1=1;
			M_DIR_D2=1;
			break;
			mMode=mode;
		}
		
		pwmSetDutyValue(0,0,mPortDREG.byte);
	}
	else if(vitesse!=lastVitesse || angle!=lastAngle)
	{ 
		if(vitesse!=lastVitesse)
		{
			mVitesse_D=(float)(vitesse-100)/100;
			
		}

		if(angle!=lastAngle)
		{
			mAngle_D=(float)((angle<<1)*Pi)/180;
		}
			
		CalculPWM(mVitesse_D, 0,mVitesse_D,mVitesse_D,&mDuty_G,&mDuty_D);

		//MODE AVANT
		if(mDuty_G<0)
		{
			M_DIR_G1=0;
			M_DIR_G2=1;
			mDuty_G=mDuty_G*(-1);
		}
		//MODE ARRIERE	
		else
		{
			M_DIR_G1=1;
			M_DIR_G2=0;	
		}	

		//MODE AVANT
		if(mDuty_D<0)
		{
			M_DIR_D1=0;
			M_DIR_D2=1;
			mDuty_D=mDuty_D*(-1);
		}
		//MODE ARRIERE	
		else
		{
			M_DIR_D1=1;
			M_DIR_D2=0;	
		}
		dutyValD=(U16)((float)mDuty_D*10000);
		dutyValG=(U16)((float)mDuty_D*10000);
		pwmSetDutyValue(dutyValD,dutyValG,mPortDREG.byte);
		lastVitesse=vitesse;
		lastAngle=angle;

	}
}

void moteurAsservissement(U16 vitG,U16 vitD)
{
U8 dutyValD;
U8 dutyValG;
	if((vitG!=lastVitG || vitD!=lastVitD)&&mMode==M_MARCHE)
	{
		if(vitG!=lastVitG)
		{
			mVg=(float)((float)vitG-511)/511;
			
		}

		if(vitD!=lastVitD)
		{
			mVd=(float)((float)vitD-511)/511;
		}
			
		CalculPWM(mVitesse_D, mAngle_D,mVg,mVd,&mDuty_G,&mDuty_D);

		//MODE AVANT
		if(mDuty_G<0)
		{
			M_DIR_G1=0;
			M_DIR_G2=1;
			mDuty_G=mDuty_G*(-1);
		}
		//MODE ARRIERE	
		else
		{
			M_DIR_G1=1;
			M_DIR_G2=0;	
		}	

		//MODE AVANT
		if(mDuty_D<0)
		{
			M_DIR_D1=0;
			M_DIR_D2=1;
			mDuty_D=mDuty_D*(-1);
		}
		//MODE ARRIERE	
		else
		{
			M_DIR_D1=1;
			M_DIR_D2=0;	
		}
		dutyValD=mDuty_D*255;
		dutyValG=mDuty_D*255;	
		pwmSetDutyValue(dutyValD,dutyValG,mPortDREG.byte);
		lastVitG=vitG;
		lastVitD=vitD;
	}



}
