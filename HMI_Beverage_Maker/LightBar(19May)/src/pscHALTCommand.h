/** \file		pscHALTCommand.h
 *
 *  \brief		
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 



/* types */

#ifndef PSC_HALT_I2C_H
#define PSC_HALT_I2C_H
#include "bevMakerHmi.h"
#include "pscCommand.h"
#include "pscHALTCommand_Private.h"
#include "pscI2c.h"
#include "externalFlashMem.h"
#include "pscEvent.h"
#include "ledDriverIc.h"
#include "display.h"
#include "rs485test.h"
#include "lowPower.h"
#include "adc.h"
#include "proxSensor.h"
#include "quadDecoder.h"
#ifdef __cplusplus
extern "C"{
	#endif
extern	uint32_t gSequenceCount;
	static void pscHALTProcessCmd(PscHALTCommand_t* pCmd);

/** \brief Set the display to the on/off state and HALT condition commanded by the test-box.
 *
 *  \return  Nothing
 */
static void pscHALTdisplay(uint8_t onOff, uint8_t step);

/** \brief Set the Indicators to the HALT condition commanded by the test-box.
 *
 *  \return  Nothing
 */
static void pscHALTIndicatorsColourIndex(uint8_t index);

/** \brief Set the button backlights to the HALT condition commanded by the test-box.
 *
 *  \return  Nothing
 */
static void pscHALTButtonsColourIndex(uint8_t index);


/* function prototypes for fns in other modules, for purpose of HALT only */
void dspInverseEnable(void);
void dspInverseDisable(void);

/* function prototypes */

/** \brief A PSC command has been received, is pending processing and has been copied to the local buffer. Process it.
 *
 *  \return  Nothing
 */
void pscHALTProcessDataFromPsc(void);

/** \brief Access the sequence count that was sent in the most recent command from the test-box.
 *
 *  \return  The current sequence count
 */
uint32_t pscHALT_GetSequenceCount(void);

/*** END of FILE ***/
#ifdef __cplusplus
}
#endif

#endif