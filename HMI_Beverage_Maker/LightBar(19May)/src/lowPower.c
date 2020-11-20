/** \file		lowPower.c
 *
 *  \brief		Enable / Disable power regulators
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
#include "lowPower.h"
#include "display.h"






/** Initialise the low power (standby) mode driver
	\return Nothing
*/
void lowPowerInit(void)
{
	/* STANDBY, on PORTE */
	/* Standby, or low-power, mode switches off the unecessary regulators and switches the 3V2 regulator supply to the 12V5 HMI supply */
	/*          Bit:  7 6 5 4  3 2 1 0 */
	/*    Direction:  - - - 1  - - - -  0=Input, 1=Output */
	/*Initial State:  - - - 0  - - - - */
	/*	              | | | |  | | | |  bit   */
	/*	              | | | |  | | | +-- b0 : */
	/*	              | | | |  | | +---- b1 : */
	/*	              | | | |  | +------ b2 : */
	/*	              | | | |  +-------- b3 : */
	/*	              | | | +----------- b4 : STANDBY, output, Logic 0 = Power-on; Logic 1 = Standby/Lo-power. Initialise power-on */
	/*	              | | +------------- b5 : */
	/*	              | +--------------- b6 : */
	/*	              +----------------- b7 : */
	PORTE.OUTCLR = 0x10;	/* Clear output drive to logic 0 */
	PORTE.DIRSET = 0x10;	/* Set outputs */

	/* configured to output, totem-pole */
	PORTE.PIN4CTRL = PORT_OPC_TOTEM_gc;
}

bool gLowPowerTestEnabled;

void lowPowerTestEnable(bool bEnable)
{
	if(gLowPowerTestEnabled != bEnable)
	{	/* changing */
		if(bEnable)
		{	/* Switch to low-power (standby) mode */
			PORTE.OUTSET = PIN4_bm;	/* Set output drive to logic 1 */
		}
		else
		{
			PORTE.OUTCLR = PIN4_bm;	/* Clear output drive to logic 0 */
		}
		
		gLowPowerTestEnabled = bEnable;
	}
}

/*** END of FILE ***/