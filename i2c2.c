///////////////////////////////////////////////////////////////////////////////
// i2c_caneva.c
//
// Created: 2014-03-10
// Author : Maxime Tardif et Amaury LAINE
//
// Details :
//	Init TWI communication. 
//	Made a read and a write and use interrupt to send and receive data 
//     
///////////////////////////////////////////////////////////////////////////////

#include "i2c2.h"
//#include "ADC.h"
#include "hardware.h"

//ping will change all 50ms
U8 ping=SENSOR_R;
float ratioL;
float ratioR;

//Circular buffer
U8 CircularBufferOut[CIRCULAR_BUFFER_SIZE];

//counter to have position now and the end of the buffer
U8 CircularBufferOutEnd= 0;
U8 CircularBufferOutIndex= 0;

///////////////////////////////////////////////////////////////////////////////
// putDataOutBuf
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		load a data in TWI circular buffer and increment count to have position 
// of the head.
//
// Input :  Data to be send to TWI (addresse, register or data) on 8 bits
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
void putDataOutBuf(U8 data){

	CircularBufferOutEnd++;
	CircularBufferOutEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferOut[CircularBufferOutEnd] = data;

}


///////////////////////////////////////////////////////////////////////////////
// getDataOutBuf
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		Read circular buffer and increment the count who give the position now
//
// Input :  N/A
//
// Output : Adresse, register or data to be used by TWI
//
///////////////////////////////////////////////////////////////////////////////
U8 getDataOutBuf(void)
{
	
	U8 Buff;
	
	CircularBufferOutIndex++;
	CircularBufferOutIndex %= CIRCULAR_BUFFER_SIZE;
	Buff = (U8)CircularBufferOut[CircularBufferOutIndex];
	CircularBufferOut[CircularBufferOutIndex] = 0x00;

	return Buff;

}

///////////////////////////////////////////////////////////////////////////////
// nextDataOutBuf
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		Read the next value in circular buffer, but don't decrements counter
// of position.
//
// Input :  N/A
//
// Output : Adresse, register or data to be used by TWI
//
///////////////////////////////////////////////////////////////////////////////
U8 nextDataOutBuf(void){
	
	U8 Buff_count = CircularBufferOutIndex;
	
	Buff_count++;
	Buff_count %= CIRCULAR_BUFFER_SIZE;
	
	return (U8)CircularBufferOut[Buff_count];

}

///////////////////////////////////////////////////////////////////////////////
// TWI_busy
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		to know if TWI is currently communicating.
//
// Input :  N/A
//
// Output : 1 : TWI = currently communicating
//			0 : TWI = free
//
///////////////////////////////////////////////////////////////////////////////
U8 TWI_busy(void)
{

  return ( TWCR & (1<<TWIE) );                  // IF TWI Interrupt is enabled then the Transceiver is busy

}

///////////////////////////////////////////////////////////////////////////////
// Init_TWI
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		Init TWI communication and made a first set of register to slave
//
// Input :  N/A
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
void Init_TWI(void)
{
		
	// Prescale of 1, to have 10Khz on SCL
	TWSR |= (0<<TWPS1)|(1<<TWPS0);	//TWPS value of the prescale
	TWBR = CLK_TWI;						//Bit Rate register

	//init sensor, and configure parameters
	twiWrite(LEFT_SONAR_W,  REG_W_GAIN, GAIN);				//registre gain sensor left
	twiWrite(RIGHT_SONAR_W, REG_W_GAIN, GAIN);				//registre gain sensor right

	twiWrite(LEFT_SONAR_W,  REG_W_DISTANCE, DISTANCE);		//registre distance sensor left
	twiWrite(RIGHT_SONAR_W, REG_W_DISTANCE, DISTANCE);		//registre distance sensor right
	
	if(TWI_busy() == 0)
	{
		START_I2C;
	}

}

///////////////////////////////////////////////////////////////////////////////
// twiWrite
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		stop interruptions and restart when this function is over. twiWrite is used
// to put in circular buffer.
//		Start communication.
//
//		TWI protocole to write : address -> register -> data
//
// Input :  - Address : send address of slave on 8 bits. last bit (LSB) is normaly 
//                      0 to be in write mode
//			- Register : send register of slave on 8 bits. 
//			- Data : data will be put in register of slave on 8 bits. 
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
void twiWrite(U8 address, U8 registre, U8 data)
{
		
	cli();
	putDataOutBuf(address);			// First Byte send in trame TWI, ask to which slave in TWI network
	putDataOutBuf(registre);		// Second Byte send in trame TWI, which register in this slave
	putDataOutBuf(data);			// Data who will be put int this register.
	sei();
	
}

///////////////////////////////////////////////////////////////////////////////
// twiRead
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		stop interruptions and restart when this function is over. twiRead is used
// to read in circular buffer.
//		Start communication.
//
//		TWI protocole to write : address (write mode) -> register -> address (read 
//								 mode) -> data read.
//
// Input :  - Address : send address of slave on 8 bits. This adresse is in read mode
//						last bit (LSB is 1).
//			- Register : send register of slave on 8 bits. 
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
void twiRead(U8 address, U8 registre)
{

	cli();		
	putDataOutBuf(address & 0xFE);	//begin with adresse of slave in write mode (& 0xFE)		
	putDataOutBuf(registre);		//register who will be read
	putDataOutBuf(address);			//send adresse to slave in read mode
	sei();
}

///////////////////////////////////////////////////////////////////////////////
// Ping_sensor
// Created: 2014-03-16
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		There is two sensors. First ping is to send commande to made mesure on first
// sensor and to read data on the other sensor. Second ping the reverse, send commande 
// on the second sensor and read the first sensor.
//
// Input :  N/A
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
void Ping_sensor(void){
	if(ping == SENSOR_R){

		//put new parameters on right sensor
		twiWrite(RIGHT_SONAR_W, REG_W_GAIN, GAIN);				//registre gain sensor right
		twiWrite(RIGHT_SONAR_W, REG_W_DISTANCE, sensorR.distance);		//registre distance sensor right

		// Ping sensor right
		twiWrite(RIGHT_SONAR_W, REG_W_CMD, CMD_CM);

		//read value of sensor left
		twiRead(LEFT_SONAR_R, REG_R_MSB);
		twiRead(LEFT_SONAR_R, REG_R_LSB);

		//next ping on sensor left
		ping = SENSOR_L;

	}
	else 
	{

		//put new parameters on left sensor
		twiWrite(LEFT_SONAR_W,  REG_W_GAIN, GAIN);//registre gain sensor left
		twiWrite(LEFT_SONAR_W,  REG_W_DISTANCE, sensorL.distance);//registre distance sensor left
		
		// Ping sensor left
		twiWrite(LEFT_SONAR_W, REG_W_CMD, CMD_CM);	
		
		//read value of sensor right
		twiRead(RIGHT_SONAR_R, REG_R_MSB);
		twiRead(RIGHT_SONAR_R, REG_R_LSB);

		//next ping on sensor right
		ping = SENSOR_R;

	}
	
	if(TWI_busy() == 0){
		START_I2C;
	}
	
	//set LED in function of which sensor is Pinging
//	(ping == SENSOR_R) ? CLR(PORTB, PORTB3) : SET(PORTB, PORTB3);
//	(ping == SENSOR_L) ? CLR(PORTB, PORTB5) : SET(PORTB, PORTB5);
}


///////////////////////////////////////////////////////////////////////////////
// Gestion_colision
// Created: 2014-03-20
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		The principle is to add a ratio on duty cycle. When right sensor is more
// close than left sensor, slow down roght motor.
//		For this ratio, the equation of distance : real distance = (DISTANCE+1)*4.3cm
// is used to be proportionnal.
//
// Input :  - Left duty cycle between 0.0f and 1.0f
//			- Right duty cycle between 0.0f and 1.0f
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
void Gestion_colision(float *l_Duty_cycleG, float *l_Duty_cycleD, U8 * port){

	tREG08 mPortDREG;		
	#define M_DIR_G1 mPortDREG.bit.b2
	#define M_DIR_G2 mPortDREG.bit.b3
	#define M_DIR_D1 mPortDREG.bit.b6
	#define M_DIR_D2 mPortDREG.bit.b7
	

	//more than sensor left down, more ratioR down. Vice versa
	ratioL = (sensorL.Value<sensorR.Value) ? ((sensorL.Value/4.3f-1)/100) : ((sensorL.Value>sensorR.Value) ? (0.8f) : (1.0f));
	ratioR = (sensorL.Value>sensorR.Value) ? ((sensorR.Value/4.3f-1)/100) : ((sensorL.Value<sensorR.Value) ? (0.8f) : (1.0f));	
	
	//security, turn on himself if object too close
	if((sensorL.Value<SECURITY_LENGHT || sensorR.Value<SECURITY_LENGHT) && (sensorL.Value<=sensorR.Value)){

		M_DIR_G1=1;
		M_DIR_G2=0;	
		M_DIR_D1=0;
		M_DIR_D2=1;

		ratioL = 0.1;
		ratioR = 0.1;
	}
	else if((sensorL.Value<SECURITY_LENGHT || sensorR.Value<SECURITY_LENGHT) && (sensorL.Value>sensorR.Value))
	{
		M_DIR_G1=0;
		M_DIR_G2=1;
		M_DIR_D1=1;
		M_DIR_D2=0;	

		ratioL = 0.1;
		ratioR = 0.1;
	}

	//LED4 turn on when object on sensor left, LED2 for sensor right
//	(sensorL.Value > 300) ? SET(PORTB, PORTB4) : CLR(PORTB, PORTB4);
//	(sensorR.Value > 300) ? SET(PORTB, PORTB2) : CLR(PORTB, PORTB2);

	//multiplicate ratio and duty cycle
	*l_Duty_cycleG *= ratioR;
	*l_Duty_cycleD *= ratioL;
	*port=mPortDREG.byte;
	
}

///////////////////////////////////////////////////////////////////////////////
// Gestion_colision
// Created: 2014-03-20
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		To have a range between 1.5m and 3.0m, depend of speed
//
// Input :  - moyenne of right speed
//			- moyenne of left speed
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
void range_sensor(float MoyenneLeft, float MoyenneRight)
{
	sensorR.distance = (MoyenneRight > 0.0f) ? ((U8)(MoyenneRight * COEFF_CONVERT + OFFSET_CONVERT)) : (1);
	sensorL.distance = (MoyenneLeft  > 0.0f) ? ((U8)(MoyenneLeft  * COEFF_CONVERT + OFFSET_CONVERT)) : (1);

}

///////////////////////////////////////////////////////////////////////////////
// TWI interruption
// Created: 2014-03-20
// Author : Maxime Tardif and Amaury Laine 
//
// Details :
//		Send TWI protocle depend of mode and other. Use state machine integrate
// in ATMEGA32.
//
// Input :  N/A
//
// Output : N/A
//
///////////////////////////////////////////////////////////////////////////////
ISR(TWI_vect) {
	
	U8 data_next=0;
	static U8 reg_sensor = 0, flag_save_L = 0, flag_save_R = 0, Left_LSB = 0, Right_LSB = 0;
	U8 status  = TWSR & 0xF8;

	switch (status) {
		case	0x08: //start condition
		case	0x10: //restart condition
					  if(CircularBufferOutEnd == CircularBufferOutIndex){	//Buffer empty... STOP TWI
						STOP_I2C;
					  }
					  else{													//Send adresse
						
					  	TWDR = getDataOutBuf(); 							
						CONTINUE_I2C;
					  }
			/* 
				Si  nous avons un start ou un restart condition alors il faut envoyer l'addr 
				qui est dans le buffer Out et Activer le bus sans start/stop 
			*/
			break;			
		case	0x18: // address has been send	 
		case	0x28: // data has been send 	
		case	0x30: if(CircularBufferOutEnd == CircularBufferOutIndex){	//Buffer empty... STOP TWI
						STOP_I2C;
					  }
					  else{													
							data_next = nextDataOutBuf();					
					  		if(data_next == 0xE0 || data_next == 0xE1 || 	//restart if there is an adresse in buffer
							   data_next == 0xE2 || data_next == 0xE3){	
	    						RESTART_I2C;
							}
                            else{
								TWDR = getDataOutBuf();						//send data or register
								CONTINUE_I2C;
							}
							reg_sensor = TWDR;
					  }

			
			break;

			/* 
				Si  nous avons un data ou une addr d'écrit sur le bus, ensuite il peut y avoir un autre data, 
				un stop ou un restart. Il faut donc lire le buffer pour savoir quoi faire et configure 
				le bus en conséquence 
			*/
			
			

		case	0x50:  // data received
					   CONTINUE_I2C;

				break;
		case	0x58: //ack and save data depend of register and address
					if(CircularBufferOutEnd == CircularBufferOutIndex){	//Buffer empty... STOP TWI
						STOP_I2C;
					}
					else{													//Send adresse		
					 	RESTART_I2C;
					}
					  
						
					if(ping == SENSOR_L){									//save data from left sensor 
						if(reg_sensor == REG_R_MSB){						//which register has been traited
							if(flag_save_L == 1){
								sensorL.Value = (TWDR<<8)|Left_LSB;
								Left_LSB = 0;
								flag_save_L = 0;
							}
						}
						else if(reg_sensor == REG_R_LSB){
								Left_LSB = TWDR;
								flag_save_L = 1;
							
						}
					}
					else if(ping == SENSOR_R){								//save data from right sensor 
						if(reg_sensor == REG_R_MSB){						//MSB traited
							if(flag_save_R == 1){
								sensorR.Value = (TWDR<<8)|Right_LSB;		//Save 16 bits of data only if LSB and MSB traited
								Right_LSB = 0;
								flag_save_R = 0;
							}
						}
						else if(reg_sensor == REG_R_LSB){					//LSB traited
								Right_LSB = TWDR;							//Save in buffer
								flag_save_R = 1;
							
						}
					}

				break;	  

		case	0x40: // address in read mode send
					TWCR |= ((1<<TWINT)|(1<<TWEA));							//acquittement
				

			break;

		case	0x48: //no respond of right and left sensor
		case	0x20: RESTART_I2C;

			break;

		default : 

			break;


	}
}
