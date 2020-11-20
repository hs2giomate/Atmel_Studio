/** \file		manualResetDetect.c
 *
 *  \brief		Detect the manual reset condition
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
#include "manualResetDetect.h"






/** Initialise the manual Reset Detect
	\return Nothing
*/   
void manualResetDetectInit(void)
{
	/* USER-RST/, on PORTE */
	/* Manual reset is invoked by press-and-hold the power button for at least 5s */
	/*          Bit: 7 6 5 4  3 2 1 0 */
	/*    Direction: 0 - - -  - - - -  0=Input, 1=Output */
	/*Initial State: - - - -  - - - - */
	/*	              | | | |  | | | |  bit   */
	/*	              | | | |  | | | +-- b0 : */
	/*	              | | | |  | | +---- b1 : */
	/*	              | | | |  | +------ b2 : */
	/*	              | | | |  +-------- b3 : */
	/*	              | | | +----------- b4 : */
	/*	              | | +------------- b5 : */
	/*	              | +--------------- b6 : */
	/*	              +----------------- b7 : USER-RST/, input with pull-up */
	PORTE.DIRCLR = 0x80;	/* Clear to input */

	/* configured to pull-up on input */
	PORTE.PIN7CTRL = PORT_OPC_PULLUP_gc;

}

bool manageManualResetDetect(void)
{
	bool bActive;
	uint8_t cPinMask;

	bActive = false;
	cPinMask = PIN7_bm;

	if((PORTE.IN & cPinMask) == 0)
	{	/* button indicating active */
		bActive = true;
	}

	return(bActive);
}


/*** END of FILE ***/