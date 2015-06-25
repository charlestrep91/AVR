/******************************************************************************
 *
 *
 *****************************************************************************/
#include "hardware.h"

#ifndef I2C_H
#define I2C_H


#define CIRCULAR_BUFFER_SIZE 64
#define SLAVE_W_ADDR 45

/** Prototype */

void putDataOutBuf(U8 data);
U8 getDataOutBuf(void);
void putDataInBuf(U8 * ptr);
U8 * getDataInBuf(void);

void twiWrite(U8 address, U8 registre, U8 data);
void twiRead(U8 address, U8 registre, U8 *ptr);
void twiInit(void);

/** Les variables */
U8 CircularBufferOut[CIRCULAR_BUFFER_SIZE];
U8 * CircularBufferIn[CIRCULAR_BUFFER_SIZE];

U8 CircularBufferOutEnd;
U8 CircularBufferOutIndex;
U8 CircularBufferInEnd;
U8 CircularBufferInIndex;


#endif
