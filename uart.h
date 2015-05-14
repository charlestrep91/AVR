





#ifndef UART_H
#define UART_H

#include "hardware.h"													
#define RX_BUFFER_SIZE 60
#define TX_BUFFER_SIZE 60




void uartInit(void);
void uartSendString(U8 *buf, U8 size);
void uartSendByte(U8 byte);
U8 uartGetByte(void);
U8 uartGetRxSize(void);




#endif
