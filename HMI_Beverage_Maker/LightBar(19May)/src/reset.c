/** \file		reset.c
 *
 *  \brief		Read the micro-controller register that indicates the reason for the last reset
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#include "bevMakerHmi.h"
#include "reset.h"




uint8_t gLastReasonForReset;

/** Initialise the reset object, read the reason for the reset
	\return Nothing
*/   
void rstInit(void)
{
	gLastReasonForReset = RST.STATUS;
	
	/* Clear all reset flags */
	RST.STATUS = 0x3F;
}

uint8_t rstGetLastReasonForReset(void)
{
	return(gLastReasonForReset);
}


/*** END of FILE ***/