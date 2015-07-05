

#include "hardware.h"

#ifndef TWI_H
#define TWI_H
#define SONAR_GAUCHE 0xE1
#define SONAR_DROIT  0xE3

void twiRead(U8 add, U8 reg);

void twiWrite(U8 add, U8 reg, U8 data);

void twiInit(void);


U8  getDataInBuf(void);

U8 twiGetInSize(void);


#endif
