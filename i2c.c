/******************************************************************************
 *
 *
 *****************************************************************************/


#include "i2c.h"

/******************************************************************************
 * Insérer dans le buffer out
 *****************************************************************************/
void putDataOutBuf(U8 data){

	CircularBufferOutEnd++;
	CircularBufferOutEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferOut[CircularBufferOutEnd] = data;

}


/******************************************************************************
 * Retirer du buffer out
 *****************************************************************************/
U8 getDataOutBuf(void){

	CircularBufferOutIndex++;
	CircularBufferOutIndex %= CIRCULAR_BUFFER_SIZE;
	return (U8)CircularBufferOut[CircularBufferOutIndex];

}


/******************************************************************************
 * Insérer dans le buffer in
 *****************************************************************************/
void putDataInBuf(U8 * ptr){

	CircularBufferInEnd++;
	CircularBufferInEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferIn[CircularBufferInEnd] = ptr;

}


/******************************************************************************
 * Retirer du buffer in
 *****************************************************************************/
U8 * getDataInBuf(void){

	CircularBufferInIndex++;
	CircularBufferInIndex %= CIRCULAR_BUFFER_SIZE;
	return CircularBufferIn[CircularBufferInIndex];	

}


/******************************************************************************
 * Écrire sur le bus twi
 *****************************************************************************/
void twiWrite(U8 address, U8 registre, U8 data){
		
	cli();
	/*
		Insérer ici le code pour lancer une écriture dur le bus twi
	*/
	sei();

}

/******************************************************************************
 * lire sur le bus
 *****************************************************************************/
void twiRead(U8 address, U8 registre, U8 *ptr){

	cli();
	/*
		Insérer ici le code pour lancer une écriture dur le bus twi
	*/	
	sei();

}


/******************************************************************************
 *
 *****************************************************************************/
ISR(TWI_vect) {
	
	U8 status  = TWSR & 0xF8;
		
	switch (status) {
		case	0x08: /* Start Condition */
	//		TWCR |= (1<<TWSTA) | (1<<TWINT); 	//send a start condition and clear interrupt flag
	//		while (!(TWCR & (1<<TWINT)))		//wait until start condition has been sent
			TWDR = SLAVE_W_ADDR;				//load slave address
			TWCR = (1<<TWINT) | (1<<TWEN);		//start transmission of address
			break;


		case	0x10: /* Restart Condition */
			
			/* 
				Si  nous avons un start ou un restart condition alors il faut envoyer l'addr 
				qui est dans le buffer Out et Activer le bus sans start/stop 
			*/
			
		break;

		case	0x18: /* Address Write Ack */
			TWDR = getDataOutBuf();
			TWCR = (1<<TWINT) | (1<<TWEN);
			break;

		case	0x28: /* Data Write Ack */

		case	0x30: /* Date Write NoAck */
			
			/* 
				Si  nous avons un data ou une addr d'écrit sur le bus, ensuite il peut y avoir un autre data, 
				un stop ou un restart. Il faut donc lire le buffer pour savoir quoi faire et configure 
				le bus en conséquence 
			*/
			
			break;

		case	0x50: /* Data Read Ack */
		case	0x58: /* Data Read NoAck */

			/* 
				Une lecture à été effectué sur le bus, il faut donc la récupérer 
			*/

		case	0x40: /* Address Read Ack */

			/* 
				Puisqu'il n'y a pas de break dans les deux case 0x50 et 0x58, quand nous sommes ici
				nous avons soit lue la donnée ou envoyé l'addr à lire, il peut donc y avoir un stop, un
				start ou encore il faut placer le bus en mode lecture 
			*/
	
			break;

		case	0x48: /* Address Read NoAck */
		case	0x20: /* Address Write NoAck */

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
}

/******************************************************************************
 *
 *****************************************************************************/
void twiInit(void)
{

	TWBR = 0x20; 
	TWCR |= ((1<<TWEN) | (1<<TWIE));
	TWSR |= (1<<TWD0);
	TWSR &= (0<<TWD1);
	
}	





























