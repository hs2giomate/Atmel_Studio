/** \file		pscCommand.h
 *
 *  \brief		process command data from the HALT test box
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 
#ifndef PSC_COMMAND_H
#define PSC_COMMAND_H
#include "pscHALTCommand.h"
#include "pscI2c.h"
#include "bevMakerHmi.h"
#ifdef __cplusplus
extern "C"{
	#endif

/* types */
typedef struct PscCommand
{
	uint8_t cCommand;	/* Command from PSC. The following data is interpreted depending on the command */
	uint8_t cTarget;		/* usage is undefined */
	uint8_t cSizeOfPayload;		/* usage is undefined */
	uint8_t cCrc8;		/* usage is undefined */
	uint8_t cState;		/* usage is undefined */
	uint8_t cFlags;		/* usage is undefined */
} PscCommand_t;

typedef struct PscProgress
{
	uint8_t cCommand;	/* Command from PSC. The following data is interpreted depending on the command. Eg Catering Progress */
	uint8_t cTarget;		/* usage is undefined */
	uint8_t cSizeOfPayload;		/* usage is undefined */
	uint8_t cCrc8;		/* usage is undefined */
	uint16_t wRemaining;	/* usage is undefined */
	uint8_t cProgress;	/* usage is undefined */
	uint8_t cFlags;		/* usage is undefined */
} PscProgress_t;

enum PscEventLength_enum
{
	PscEventLength	= 4
};


typedef struct HmiToPscEvent
{
	uint8_t cPanelState;
	uint8_t cGainState;
	uint8_t cEvent[PscEventLength];
	uint8_t cEventCount;
	uint8_t cCrc8;
} HmiToPscEvent_t;

/* function prototypes */

/** \brief .
 *
 *  \return  Nothing
 */
void pscCommandManager(void);

/** \brief .
 *
 *  \return  Nothing
 */
void pscHALTprogressBarStep(uint8_t step, uint8_t cInt);

#ifdef __cplusplus
}
#endif

#endif
/*** END of FILE ***/
