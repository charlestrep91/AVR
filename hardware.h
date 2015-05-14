/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/






#ifndef HARDWARE_H
#define HARDWARE_H

												

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

//Definition des type de var declarés dans le code
#define U8  unsigned char 
#define U16 unsigned int  
#define S8  signed   char 
#define S16 signed   int  
#define U32 long


/*
	initialisation des peripheriques 
*/
void hdInit(void);





#endif





		  
