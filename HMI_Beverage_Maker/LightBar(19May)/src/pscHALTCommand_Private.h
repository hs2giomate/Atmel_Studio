/** \file		.h
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

#ifndef PSC_COMMAND_PRIVATE_H
#define PSC_COMMAND_PRIVATE_H

#ifdef __cplusplus
extern "C"{
	#endif

/* types */

/*
	Structure of HALT command data sent from test box
*/
typedef struct PscHALTCommand
{
	uint32_t sequenceCount;	/* The test box increments this before each command is sent */
	uint8_t cCommand;	/* command from the test box */
	uint8_t progressBarStep;	/* test box commands the HALT step of the progress bar */
	uint8_t displayStep;	/* test box commands the HALT step of the display */
	uint8_t displayOn;	/* 0 = display OFF, 1 = display ON */
	uint8_t indicatorsColourIndex;	/* test box commands the HALT colour of the Indicators */
	uint8_t buttonsColourIndex;	/* test box commands the HALT colour of the buttons */
} PscHALTCommand_t;


enum PscCmd_enum
{
	void_Cmd	= 0,
	HaltCmd = 1,
	Rs485loopBackTestCmd = 2,
	Rs485DisableCmd = 3,
	LowPowerTestCmd = 4,
	LowPowerDisableCmd = 5,
	ProxTestCmd = 6,
	ProxDisableCmd = 7,
	QuadTestCmd = 8,
	QuadDisableCmd = 9,
	AmbientLightTestCmd = 10,
	AmbientLightDisableCmd = 11,
};

enum HaltDisplayStep_enum
{
	displayRightIsOn_HaltStep = 0,
	displayLeftIsOn_HaltStep = 1,
};

enum HaltProgressBarStep_enum
{
	AllOff_ProgressBarStep = 0,
};

enum HaltBacklightsColourIndex_enum
{
	Off_BacklightColourIndex = 0,
	Red_BacklightColourIndex = 1,
	Green_BacklightColourIndex = 2,
	Blue_BacklightColourIndex = 3,
	White_BacklightColourIndex = 4,
};

#ifdef __cplusplus
}
#endif

#endif
/* function prototypes */

/*** END of FILE ***/
