/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/


#include "uart.h"
#include "hardware.h"

//UART COMMUNICATION SPEED CONFICGURATION
#ifndef F_CPU
#define F_CPU            16000000UL      /* Crystal 16.000 Mhz */
#endif

#define UART_BAUD_RATE      9600      /* 9600 baud */
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)

//Variable globales locales

//Variables pour les buffers
U8 uartRxBuffer[UART_RX_BUFFER_SIZE];
U8 uartTxBuffer[UART_TX_BUFFER_SIZE];
U8 uartRxInPtr;
U8 uartRxOutPtr;
U8 uartTxInPtr;
U8 uartTxOutPtr;
U8 uartTxSize;
U8 uartRxSize;

//fonctions utilisées localement
void uartTxRoutine(void);
void uartRxRoutine(void);
void uartInitBuffers(void);

/*
 Initialisation des var pour les FIFOS
*/
void uartInitBuffers(void)
{
	uartRxInPtr		=0;
	uartRxOutPtr	=0;
	uartTxInPtr		=0;
	uartTxOutPtr	=0;   
	uartTxSize		=0;
    uartRxSize		=0; 
}


/*
 Initialisation du Uart
*/
void uartInit(void)
{
    /* configure asynchronous operation, no parity, 1 stop bit, 8 data bits, Tx on rising edge */
    UCSRC = (1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL);       
    /* enable RxD/TxD and ints */
    UCSRB = (1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2);       
    /* set baud rate */
    UBRRH = (U8)(UART_BAUD_SELECT >> 8);          
    UBRRL = (U8)(UART_BAUD_SELECT & 0x00FF); 
	//init les variables des buffers 
	uartInitBuffers();   
}

/*
 routine d'envoi des data au uart
*/
void uartTxRoutine(void)
{
	if(uartTxSize)
	{
		UDR = uartTxBuffer[uartTxOutPtr];      
		uartTxOutPtr++;
		uartTxSize--;
		//remet à zéro si jamais la taille du buffer 
		if(uartTxOutPtr>(UART_TX_BUFFER_SIZE-1))
	 		uartTxOutPtr=0;
	}
	else
	{
		uartTxOutPtr=0;
		uartTxInPtr=0;
	}
		
}

/*
 routine de reception des data du uart
*/
void uartRxRoutine(void)
{
	uartRxBuffer[uartRxInPtr] = UDR;        
	uartRxInPtr++;
	uartRxSize++;
	if(uartRxInPtr>(UART_RX_BUFFER_SIZE-1))
			uartRxInPtr=0;
}

/*
 envoi une chaine de caractère avec caractere de fin
*/ 
void uartSendString(U8 *buf)
{ 
	U8 stringSize=0;

	//loop tant que le caractere de fin n'est pas trouvé
	while((*buf!=UART_END_OF_STRING_CHAR) && (stringSize<UART_MAX_SIZE_STRING))
	{
		uartTxBuffer[uartTxInPtr]=*buf;

		buf++;
		uartTxInPtr++;
		uartTxSize++;
		stringSize++;

		//remet à zéro uartTxInPtr si  dépasse la taille du buffer 
		if(uartTxInPtr>(UART_TX_BUFFER_SIZE-1))
			uartTxInPtr=0;		
	}

	//envoie le premier byte pour démarrer l'interruption si UDRE est à 1
	if(UCSRA&&(1<<UDRE))
   		 uartTxRoutine();

}

/*
 envoi un byte à la fois sur TX
*/
void uartSendByte(U8 byte)
{ 
	
	uartTxBuffer[uartTxInPtr]=byte;
	uartTxInPtr++;
	uartTxSize++;
	if(uartTxInPtr>(UART_TX_BUFFER_SIZE-1))
		uartTxInPtr=0;

	//envoie le premier byte pour démarrer l'interruption si UDRE est à 1
   	if(UCSRA&&(1<<UDRE))
   		uartTxRoutine();
	
}

/*
	extrait un byte du FIFO de reception
*/
U8 uartGetByte(void)
{ 
	U8 rxByte=0;
	if(uartRxSize)
	{		
		rxByte=uartRxBuffer[uartRxOutPtr];
		uartRxOutPtr++;
		//remet à zéro si dépasse la taille du buffer 
		if(uartRxOutPtr>(UART_RX_BUFFER_SIZE-1))
			uartRxOutPtr=0;
		uartRxSize--;
	}
	else
	{
		uartRxOutPtr=0;
		uartRxInPtr=0;
	}
	return rxByte;		

}

/*
	retourne le nb de byte disponible dans le FIFO de reception
*/
U8 uartGetRxSize(void)
{
	return uartRxSize;
}

/*
	retourne le nb de byte dans le buffer d'envoi
*/
U8 uartGetTxSize(void)
{
	return uartTxSize;
}


//--INTERRUPTS LOOPS--
SIGNAL(SIG_UART_TRANS)      
/* signal handler for uart txd ready interrupt */
{
    uartTxRoutine();
}

SIGNAL(SIG_UART_RECV)      
/* signal handler for receive complete interrupt */
{
	uartRxRoutine();   
}
