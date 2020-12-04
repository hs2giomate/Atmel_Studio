/** \file		pscCommand.c
 *
 *  \brief		Handle commands from the PSC. This is hijacked for HALT
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#include "pscCommand.h"


void pscCommandManager(void)
{
	/* check if a command has been sent from the PSC */
	if(pscI2cIsCommandPendingProcessing())
	{
		pscHALTProcessDataFromPsc();
	}
}


/*** END of FILE ***/
