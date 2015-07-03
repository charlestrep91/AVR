///////////////////////////////////////////////////////////////////////////
/*
	dbgCmd.h
 	ELE542 - �T�2015
 	Jonathan Lapointe LAPJ05108303
 	Charles Tr�panier TREC07029107

	Contains prototypes and definitions for dbgCmd.c.
*/
///////////////////////////////////////////////////////////////////////////


#ifndef DBG_CMD_H
#define DBG_CMD_H
#include "hardware.h"
										

/*
	envoi une chaine de caract�re qui doit se terminer 
    par NULL (string)
	ARG U8 *buf ptr sur le d�but de la string
*/
void dbgSendDbgString(U8 *buf);

/*
	envoi une chaine de caract�re qui doit se terminer 
    par NULL (string)
	ARG U8 *buf ptr sur le d�but de la string
*/
void dbgSendRobotString(U8 *buf);


void dbgSendDbgU16ToDec(U8 *buf,U16 value);


#endif		
