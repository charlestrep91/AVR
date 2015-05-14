







#include "uart.h"
#include "hardware.h"

#ifndef F_CPU
#define F_CPU            16000000UL      /* Crystal 16.000 Mhz */
#endif

#define UART_BAUD_RATE      9600      /* 9600 baud */


#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)

//variable locales

//Variables pour les buffers
U8 uartRxBuffer[RX_BUFFER_SIZE];
U8 uartTxBuffer[TX_BUFFER_SIZE];
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


void uartInitBuffers(void)
{
	uartRxInPtr=0;
	uartRxOutPtr=0;
	uartTxInPtr=0;
	uartTxOutPtr=0;   
	uartTxSize=0;
    uartRxSize=0; 
}

void uartInit(void)
/* initialize uart */
{
    /* configure asynchronous operation, no parity, 1 stop bit, 8 data bits, Tx on rising edge */
    UCSRC = (1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL);       
    /* enable RxD/TxD and ints */
    UCSRB = (1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2);       
    /* set baud rate */
    UBRRH = (U8)(UART_BAUD_SELECT >> 8);          
    UBRRL = (U8)(UART_BAUD_SELECT & 0x00FF); 
	//init buffers variables
	uartInitBuffers();   
}

void uartTxRoutine(void)
{
	if(uartTxSize)
	{
		UDR = uartTxBuffer[uartTxOutPtr];       /* write byte to data buffer */
		uartTxOutPtr++;
		uartTxSize--;
		if(uartTxOutPtr>(TX_BUFFER_SIZE-1))
	 		uartTxOutPtr=0;
	}
	else
	{
		uartTxOutPtr=0;
		uartTxInPtr=0;
	}
		
}

void uartRxRoutine(void)
{
	uartRxBuffer[uartRxInPtr] = UDR;        
	uartRxInPtr++;
	uartRxSize++;
	if(uartRxInPtr>(RX_BUFFER_SIZE-1))
			uartRxInPtr=0;
}


void uartSendString(U8 *buf, U8 size)
/* send buffer <buf> to uart */
{ 
	U8 i;  
	for(i=0;i<size;i++)
	{
		uartTxBuffer[uartTxInPtr]=*buf;
		buf++;
		uartTxInPtr++;
		if(uartTxInPtr>(TX_BUFFER_SIZE-1))
			uartTxInPtr=0;
		uartTxSize++;
	}
	//envoie le premier byte pour démarrer l'interruption
    uartTxRoutine();

}

void uartSendByte(U8 byte)
/* send buffer <buf> to uart */
{ 
	uartTxBuffer[uartTxInPtr]=byte;
	uartTxInPtr++;
	uartTxSize++;
	if(uartTxInPtr>(TX_BUFFER_SIZE-1))
		uartTxInPtr=0;
	//envoie le premier byte pour démarrer l'interruption
    uartTxRoutine();
	
}

U8 uartGetByte(void)
/* send buffer <buf> to uart */
{ 
	U8 rxByte=0;

	if(uartRxSize)
	{		
		rxByte=uartRxBuffer[uartRxOutPtr];
		uartRxOutPtr++;
		if(uartRxOutPtr>(RX_BUFFER_SIZE-1))
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

U8 uartGetRxSize(void)
{
	return uartRxSize;
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
