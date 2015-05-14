/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/


#ifndef UART_H
#define UART_H

#include "hardware.h"
													
#define UART_RX_BUFFER_SIZE 60
#define UART_TX_BUFFER_SIZE 60

//definition utilisé pour la fonction void uartSendString(U8 *buf);	
#define UART_MAX_SIZE_STRING 	UART_TX_BUFFER_SIZE
#define UART_END_OF_STRING_CHAR  0


/*
 initialise le UART
*/
void uartInit(void);

/*
 envoi une chaine de caractère qui doit se terminer 
 par NULL 
 La grandeur MAX est UART_MAX_SIZE_STRING
*/
void uartSendString(U8 *buf);

/*
 envoi un byte sur TX
*/
void uartSendByte(U8 byte);

/*
 extrait un byte du FIFO de reception
*/
U8 	uartGetByte(void);

/*
	retourne le nb de byte dans le buffer d'envoi
*/
U8 uartGetTxSize(void);

/*
 retoure le nb de byte disponible dans le fifo
*/
U8 uartGetRxSize(void);




#endif
