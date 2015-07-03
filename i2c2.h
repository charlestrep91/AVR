///////////////////////////////////////////////////////////////////////////////
// i2c_caneva.h
// Created: 2014-01-17 17:59:19
// Author : Maxime Tardif et Amaury LAINE
//
// Details :
//	Function do configure and use TWI in 10Khz and in master mode
///////////////////////////////////////////////////////////////////////////////

#ifndef I2C_H
#define I2C_H

#include "hardware.h"

#define CIRCULAR_BUFFER_SIZE 32

//TWI specification 
#define CLK_TWI			198			// SCL = CLOCK_CPU/(16+2*TWBR*4^TWPS)

//define for sensor
#define SENSOR_R		0
#define SENSOR_L		1

// Define adresse of sensor
#define LEFT_SONAR_W	0xE0
#define LEFT_SONAR_R	0xE1
#define RIGHT_SONAR_W	0xE2
#define RIGHT_SONAR_R	0xE3

// Define function read in register of sonar
#define REG_R_MSB		2
#define REG_R_LSB		3

// Define function write in register of sonar
#define REG_W_CMD		0		
#define REG_W_GAIN		1
#define REG_W_DISTANCE	2	

// Define value to put in register
// register of commande
#define CMD_INCH		0x50
#define CMD_CM			0x51
#define CMD_US			0x52

// register of gain
#define GAIN			12

// register of distance
#define DISTANCE		34 		//real distance = (DISTANCE+1)*4.3cm
#define COEFF_CONVERT	38.889f
#define OFFSET_CONVERT	34		//to have 15m of range
#define SECURITY_LENGHT	60

// Struct data sensor
typedef struct{
	U8 gain;
	U8 distance;
	U16 Value;
}Sensor_T;

//Macros of set condition of TWI fonctionnement
#define START_I2C  		(TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA))
#define STOP_I2C  		(TWCR = (1<<TWEN)|(0<<TWIE)|(1<<TWINT)|(1<<TWSTO))
#define RESTART_I2C  	(TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA))
#define CONTINUE_I2C  	(TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT))

// Function's prototypes : circular buffer
void putDataOutBuf(U8 data);
U8 getDataOutBuf(void);

// Function's prototypes : TWI gestion
void Init_TWI();
void twiWrite(U8 address, U8 registre, U8 data);
void twiRead(U8 address, U8 registre);
void Ping_sensor(void);
void Gestion_colision(float *l_Duty_cycleG, float *l_Duty_cycleD);
void range_sensor(float MoyenneLeft, float MoyenneRight);

void TestSensor();
void putDataInBuf(U8 data);
U8 getDataInBuf(void);
void twiRead(U8 address, U8 registre);

// Globales variables
volatile Sensor_T sensorR, sensorL;


#endif
