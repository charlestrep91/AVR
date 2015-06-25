/******************************************************************************
 *
 *
 *****************************************************************************/


#include "i2c.h"

/******************************************************************************
 * Ins�rer dans le buffer out
 *****************************************************************************/
void putDataOutBuf(u08 data){

	CircularBufferOutEnd++;
	CircularBufferOutEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferOut[CircularBufferOutEnd] = data;

}


/******************************************************************************
 * Retirer du buffer out
 *****************************************************************************/
u08 getDataOutBuf(void){

	CircularBufferOutIndex++;
	CircularBufferOutIndex %= CIRCULAR_BUFFER_SIZE;
	return (u08)CircularBufferOut[CircularBufferOutIndex];

}


/******************************************************************************
 * Ins�rer dans le buffer in
 *****************************************************************************/
void putDataInBuf(u08 * ptr){

	CircularBufferInEnd++;
	CircularBufferInEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferIn[CircularBufferInEnd] = ptr;

}


/******************************************************************************
 * Retirer du buffer in
 *****************************************************************************/
u08 * getDataInBuf(void){

	CircularBufferInIndex++;
	CircularBufferInIndex %= CIRCULAR_BUFFER_SIZE;
	return CircularBufferIn[CircularBufferInIndex];	

}


/******************************************************************************
 * �crire sur le bus twi
 *****************************************************************************/
void twiWrite(u08 address, u08 registre, u08 data){
		
	cli();
	/*
		Ins�rer ici le code pour lancer une �criture dur le bus twi
	*/
	sei();

}

/******************************************************************************
 * lire sur le bus
 *****************************************************************************/
void twiRead(u08 address, u08 registre, u08 *ptr){

	cli();
	/*
		Ins�rer ici le code pour lancer une �criture dur le bus twi
	*/	
	sei();

}


/******************************************************************************
 *
 *****************************************************************************/
ISR(TWI_vect) {
	
	u08 status  = TWSR & 0xF8;
		
	switch (status) {
		case	0x08: /* Start Condition */
		case	0x10: /* Restart Condition */
			
			/* 
				Si  nous avons un start ou un restart condition alors il faut envoyer l'addr 
				qui est dans le buffer Out et Activer le bus sans start/stop 
			*/
			
			break;

		case	0x18: /* Address Write Ack */
		case	0x28: /* Data Write Ack */
		case	0x30: /* Date Write NoAck */
			
			/* 
				Si  nous avons un data ou une addr d'�crit sur le bus, ensuite il peut y avoir un autre data, 
				un stop ou un restart. Il faut donc lire le buffer pour savoir quoi faire et configure 
				le bus en cons�quence 
			*/
			
			break;

		case	0x50: /* Data Read Ack */
		case	0x58: /* Data Read NoAck */

			/* 
				Une lecture � �t� effectu� sur le bus, il faut donc la r�cup�rer 
			*/

		case	0x40: /* Address Read Ack */

			/* 
				Puisqu'il n'y a pas de break dans les deux case 0x50 et 0x58, quand nous sommes ici
				nous avons soit lue la donn�e ou envoy� l'addr � lire, il peut donc y avoir un stop, un
				start ou encore il faut placer le bus en mode lecture 
			*/
	
			break;

		case	0x48: /* Address Read NoAck */
		case	0x20: /* Address Write NoAck */

			/* 
				Ici l'un des deux sonars n'a pas r�pondu, il faut donc tout stoper ou faire un restart
			    pour la prochaine trame qui peut �tre dans le buffer 
			*/
			
			break;

		default : 
			/*
				Cette partie de code ne devrait pas �tre utile :)
			*/
			break;
	}
}

/******************************************************************************
 *
 *****************************************************************************/
void twiInit(void)
{
/*
	TWBR = (1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL); 
	TWCR = 
	TWSR = 
	*/
}	





























