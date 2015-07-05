










#include "twi.h"
#include "hardware.h"

/******************************************************************************
 *
 *
 *****************************************************************************/

U8 twiMODE=0;
#define TWI_WRITE_MODE 0
#define TWI_READ_MODE  1
#define CLK_TWI			198	
#define u08 U8


#define TWI_SEND_START (TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE))
#define TWI_SEND_STOP  (TWCR=(1<<TWINT)|(1<<TWEN) |(1<<TWSTO)|(1<<TWIE))
#define TWI_LOAD_DATA  (TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWIE))

#define CIRCULAR_BUFFER_SIZE 32
//Circular buffer
U8 CircularBufferOut[CIRCULAR_BUFFER_SIZE];
U8 CircularBufferIn[CIRCULAR_BUFFER_SIZE];
U8 BytesSend=0;
U8 ValidRxDataFlag=0;

//counter to have position now and the end of the buffer
U8 CircularBufferOutEnd= -1;
U8 CircularBufferOutIndex= -1;
U8 CircularBufferOutSize=0;

U8 CircularBufferInEnd= -1;
U8 CircularBufferInIndex= -1;
U8 CircularBufferInSize=0;

void twiResetBuffers(void)
{
   CircularBufferOutEnd		= -1;
   CircularBufferOutIndex	= -1;
   CircularBufferOutSize	=  0;
   CircularBufferInEnd		= -1;
   CircularBufferInIndex	= -1;
   CircularBufferInSize		=  0;
}

void twiInit(void)
{		
	// Prescale of 1, to have 10Khz on SCL
	TWBR = CLK_TWI;	//Bit Rate register
	TWSR |= (0<<TWPS1)|(1<<TWPS0);	//TWPS value of the prescale
	TWCR |= (1<<TWIE);	
	TWCR |= (1<<TWINT);
}

/******************************************************************************
 * Insérer dans le buffer out RX
 *****************************************************************************/
void putDataOutBuf(u08 data){

	CircularBufferOutEnd++;
	CircularBufferOutEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferOutSize++;
	CircularBufferOut[CircularBufferOutEnd] = data;

}


/******************************************************************************
 * Retirer du buffer out RX
 *****************************************************************************/
u08 getDataOutBuf(void){

	CircularBufferOutIndex++;
	CircularBufferOutIndex %= CIRCULAR_BUFFER_SIZE;
	CircularBufferOutSize--;
	return (u08)CircularBufferOut[CircularBufferOutIndex];

}

U8 twiGetInSize(void)
{
	return CircularBufferInSize;
}




/******************************************************************************
 * Insérer dans le buffer in TX
 *****************************************************************************/
void putDataInBuf(u08 data){

	CircularBufferInEnd++;
	CircularBufferInEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferInSize++;
	CircularBufferIn[CircularBufferInEnd] = data;

}


/******************************************************************************
 * Retirer du buffer in TX
 *****************************************************************************/
U8  getDataInBuf(void){

	CircularBufferInIndex++;
	CircularBufferInIndex %= CIRCULAR_BUFFER_SIZE;
	CircularBufferInSize--;
	return CircularBufferIn[CircularBufferInIndex];	

}


/******************************************************************************
 * Écrire sur le bus twi 
 *****************************************************************************/
void twiWrite(u08 add, u08 reg, u08 data)
{		
	cli();	
	putDataOutBuf(add&0xfe);		// First Byte send in trame TWI, ask to which slave in TWI network
	putDataOutBuf(reg);		// Second Byte send in trame TWI, which register in this slave
	putDataOutBuf(data);	// Data who will be put int this register.
	twiMODE=TWI_WRITE_MODE;
	TWI_SEND_START;
	sei();	
	

}

/******************************************************************************
 * lire sur le bus
 *****************************************************************************/
void twiRead(u08 add, u08 reg)
{
	cli();
	putDataOutBuf(add&0xfe);	    //begin with adresse of slave in write mode (& 0xFE)		
	putDataOutBuf(reg);		//register who will be read
	putDataOutBuf(add);//send adresse to slave in read mode
	twiMODE=TWI_READ_MODE;
	TWI_SEND_START;
	sei();

}


/******************************************************************************
 *
 *****************************************************************************/
ISR(TWI_vect) {
	
	u08 status;
	U8 data;
	status  = TWSR & 0xF8;
//	PORTB=~status;	
	
	switch (status) 
	{
		case	0x08: /* Start Condition */				
		case	0x10: /* Restart Condition */
					
			
			if(CircularBufferOutSize!=0)
				TWDR =getDataOutBuf();
			BytesSend=0;	
			/* 
				Si  nous avons un start ou un restart condition alors il faut envoyer l'addr 
				qui est dans le buffer Out et Activer le bus sans start/stop 
			*/
			
			break;

		case	0x18: /* Address Write Ack */
			if(CircularBufferOutSize)
				 TWDR = getDataOutBuf();
			break;
		case	0x28: /* Data Write Ack */
			if(TWI_WRITE_MODE==twiMODE)
				{
					if(BytesSend==0&&CircularBufferOutSize!=0)
					{
						TWDR = getDataOutBuf();
						BytesSend++;
					
					}
					else
					{
						if(CircularBufferOutSize!=0)
						  TWI_SEND_START;	
						else	
						  TWI_SEND_STOP;
					}
				}
				if(TWI_READ_MODE==twiMODE)
				{
					if((BytesSend==0)&&(CircularBufferOutSize!=0))
					{
						TWDR = getDataOutBuf();
						BytesSend++;
		
					}
					else
						TWI_SEND_START;
				}
		
			break;
		case	0x30: /* Date Write NoAck */
			/* 
				Si  nous avons un data ou une addr d'écrit sur le bus, ensuite il peut y avoir un autre data, 
				un stop ou un restart. Il faut donc lire le buffer pour savoir quoi faire et configure 
				le bus en conséquence 
			*/		

					TWI_SEND_STOP;
					twiResetBuffers();
			break;

		case	0x50: /* Data Read Ack */
				TWI_SEND_STOP;
		     	twiResetBuffers();

	
		case	0x58: /* Data Read NoAck */
			/* 
				Une lecture à été effectué sur le bus, il faut donc la récupérer 
			*/
				if(CircularBufferOutSize!=0)
					TWI_SEND_START;	
				else	
				    TWI_SEND_STOP;
				

		case	0x40: /* Address Read Ack */

			/*	Puisqu'il n'y a pas de break dans les deux case 0x50 et 0x58, quand nous sommes ici
				nous avons soit lue la donnée ou envoyé l'addr à lire, il peut donc y avoir un stop, un
				start ou encore il faut placer le bus en mode lecture 
			*/	data=TWDR;
				putDataInBuf(data);
	
			break;

		case	0x48: /* Address Read NoAck *///gestion erreur
				TWI_LOAD_DATA;
				twiResetBuffers();
		case	0x20: /* Address Write NoAck *///gestion erreur


				TWI_LOAD_DATA;
				twiResetBuffers();
				/*
				Ici l'un des deux sonars n'a pas répondu, il faut donc tout stoper ou faire un restart
			    pour la prochaine trame qui peut être dans le buffer 
			*/
			
			break;

		default : 
			/*
				Cette partie de code ne devrait pas être utile :)
			*/
			break;
	}
	TWI_LOAD_DATA;
}

