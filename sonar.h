#include "hardware.h"



#ifndef SONAR_H
#define SONAR_H



void SonarAdjPortee(U8 portee);	

void SonarPing(void);

void SonarInit(void);



void SonarCorrectDest(float *Duty_D,float *Duty_G);




#endif
