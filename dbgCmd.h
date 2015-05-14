/*
 ELE542 

 Jonathan Lapointe LAPJ05108303
 Charles Trépanier 


*/


#ifndef DBG_CMD_H
#define DBG_CMD_H
#include "hardware.h"
										

/*
	envoi une chaine de caractère qui doit se terminer 
    par NULL (string)
	ARG U8 *buf ptr sur le début de la string
*/
void dbgSendString(U8 *buf);


#endif		
