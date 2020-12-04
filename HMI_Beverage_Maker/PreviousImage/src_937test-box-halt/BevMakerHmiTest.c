/** \file		BevMakerHmiTest.c
 *
 *  \brief		core code to run the HALT program
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "bevMakerTest.h"
#include "mainTimer.h"
#include "log.h"
#include "led.h"
#include "pscHALTCommand.h"
#include "pscHALTCommand_Private.h"
#include "keyscan.h"
#include "hmiManualReset.h"

enum {I2cReadInterval_ms = 50,};
enum {ProgressBarUpdateInterval_s = 1,};
enum {DisplayUpdateInterval_s = 1,};
enum {BacklightPauseIntervalOnButtonEvent = 10,};
enum {IndicatorBacklightUpdateInterval = 5,};

  
PscHALTCommand_t gHaltCommand;
uint32_t gdwWriteToHmiCount;
uint32_t gSequenceCount;
uint32_t gMarkAtButtonRelease_100ms;
uint32_t gI2cTrasmissionFailCount;
uint32_t gLeftButtonPressInterval;
uint32_t gCentreButtonPressInterval;
uint32_t gRightButtonPressInterval;

bool gHmiCommandPending;
bool gButtonBacklightSequence_Pause;
bool glastI2cSuccessful;
bool gNextI2CFailReportOnNewLine;


/** \brief Ready the next data to be sent to the HMI to control the progress bar HALT sequence
 *
 *  \return  Nothing
 */
static void manageProgressBarUpdate(void);

/** \brief Ready the next data to be sent to the HMI to control the OLED HALT sequence
 *
 *  \return  Nothing
 */
static void manageDisplayUpdate(void);

/** \brief Ready the next data to be sent to the HMI to control the Indicate backlight colour HALT sequence
 *
 *  \return  Nothing
 */
static void manageIndicatorBacklightUpdate(void);

/** \brief Ready the next data to be sent to the HMI to control the button backlight colour HALT sequence
 *
 *  \return  Nothing
 */
static void manageButtonsBacklightUpdate(void);

/** \brief Send the next HALT command to the target HMI, if due, via I2C
 *
 *  \return  Nothing
 */
static void manageHmiCmdUpdate(int i2cChannel);

/** \brief Read the HMI status via I2C
 *
 *  \return  Nothing
 */
static void readHmiStatus(int i2cChannel);

/** \brief check for changes of HMI button state 
 *
 *  \return  Nothing
 */
static void checkForButtonPress(uint32_t* pLeftButtonPressInterval, uint32_t* pCentreButtonPressInterval, uint32_t* pRightButtonPressInterval);

/** \brief Handler for when the left HMI button is pressed and then released
 *
 *  \return  Nothing
 */
static void OnHmiButtonRelease_Left(uint32_t* pButtonPressInterval);

/** \brief Handler for when the centre HMI button is pressed and then released
 *
 *  \return  Nothing
 */
static void OnHmiButtonRelease_Centre(uint32_t* pButtonPressInterval);

/** \brief Handler for when the right HMI button is pressed and then released
 *
 *  \return  Nothing
 */
static void OnHmiButtonRelease_Right(uint32_t* pButtonPressInterval);

/** \brief Checks if a HMI button press has temporarily paused the button backlight sequence
 *
 *  \return  Nothing
 */
static bool isButtonBacklightSequencePausedByPress(void);


void runHalt(int i2cChannel)
{
	bool bPause = false;
	bool bPausePending = false;
	bool haltStopPending = false;
	fflush(stdout);

	/* Initialise the command payload to the HMI */
	gHaltCommand.sequenceCount = gSequenceCount;
	gHaltCommand.command = HaltCmd;
	gHaltCommand.progressBarStep = 0;	/* test box commands the HALT step of the progress bar */
	gHaltCommand.displayStep = displayRightIsOn_HaltStep;	/* test box commands the HALT step of the display */
	gHaltCommand.displayOn = 1;	/* 0 = display OFF, 1 = display ON */ 
	gHaltCommand.indicatorsColourIndex = Off_BacklightColourIndex;	/* test box commands the HALT colour of the Indicators */
	gHaltCommand.buttonsColourIndex = Off_BacklightColourIndex;	/* test box commands the HALT colour of the buttons */

	if(logOpen())
	{
		/* Green LED on as soon as HALT start successful */
		ledGreenOn();

		gHmiCommandPending = true;	/* Indicate an update is required to the HMI ASAP */

		/* Run until exit or process paused */
		for(int nLoop = 0; IsRun(); nLoop++)
		{
			if(bPause)
			{}	/* HALT paused */
			else
			{
				mainTimerService();
				
				if(IsRun())
				{
					readHmiStatus(i2cChannel);
				}
				
				hmrReadHmiManualResetSignal();
				
				manageProgressBarUpdate();
				manageDisplayUpdate();
				manageIndicatorBacklightUpdate();
				manageButtonsBacklightUpdate();
				
				if(IsRun())
				{
					manageHmiCmdUpdate(i2cChannel);
				}
				
			}

			if(haltStopPending)
			{	/* User requested HALT to be stopped */
				/* Exit the program */
				setRun(false);
			}
			else if(gHmiCommandPending)
			{}	/* do not pause if an I2c transmission is pending */
			else if(bPausePending)
			{
				bPausePending = false;
				bPause = true;
				gNextI2CFailReportOnNewLine = true;
				printf("\nHALT paused at  ");
				printfDateAndTime();
				logPause(gI2cTrasmissionFailCount);
				ledGreenOff();	/* Green LED off during pause */
			}	/* HALT paused */
			else
			{}
			
			if(IsRun())
			{
				if(kbhit())
				{
					int ch = getchar();
					if(ch == gcEscapeKey)
					{	/* Escape key pressed */
						haltStopPending = true;	/* Request HALT stop after HMI display switched off */
						gHaltCommand.displayOn = 0;	/* 0 = display OFF, 1 = display ON */ 
						gHmiCommandPending = true;	/* Indicate an update is required to the HMI ASAP */
					}
					else if(ch == gcSpaceKey)
					{	/* Space bar pressed */
						/* Pause/Resume */
						if(bPausePending)
						{}	/* Ignore further space bar activity while waiting to pause */
						else if(bPause)
						{	/* paused */
							bPause = false;
							bPausePending = false;
							printf("\nHALT resumed at ");
							printfDateAndTime();
							logResume();
							gHaltCommand.displayOn = 1;	/* 0 = display OFF, 1 = display ON */ 
							gHmiCommandPending = true;	/* Indicate an update is required to the HMI ASAP */
							ledGreenOn();	/* Green LED on while HALT running */
						}
						else
						{	/* not-paused */
							bPausePending = true;
							gHaltCommand.displayOn = 0;	/* 0 = display OFF, 1 = display ON */ 
							gHmiCommandPending = true;	/* Indicate an update is required to the HMI ASAP */
						}
					}
					else
					{}	/* No other key presses acknowledged */
				}
			}
			fflush(stdout);
		}
		
		if(bPause)
		{}	/* HALT paused */
		else
		{
			logFinish(gI2cTrasmissionFailCount);
		}
	}
	
	printf("\n\n");
}


bool gLastSequenceMatch;

PscHALTEvent_t gHmiEvent;
static void readHmiStatus(int i2cChannel)
{
	static uint32_t markLastHmiRead_ms = 0;
	
	uint32_t dwDiff = gdw_msTimeCount -markLastHmiRead_ms;
	if(dwDiff >= I2cReadInterval_ms)
	{	/* Every 50 ms */

		/* mark the time of the last read of HMI status */
		markLastHmiRead_ms = gdw_msTimeCount;
		
		uint8_t payloadCount = sizeof(PscHALTEvent_t);
		ssize_t rxCountResult = read(i2cChannel, (void*)&gHmiEvent, payloadCount);
		
		bool success = (rxCountResult == payloadCount);
		if(success == false)
		{	/* read failed */
			gI2cTrasmissionFailCount++;	/* Keep track of the number of I2C failures */
			ledRedOn();
			ledGreenOff();

			if(glastI2cSuccessful || gNextI2CFailReportOnNewLine)
			{
				fprintf(stderr, "\nI2C transmission failures : %lu", (unsigned long)gI2cTrasmissionFailCount);
				fflush(stderr);
			}
			else
			{
				fprintf(stderr, "\rI2C transmission failures : %lu", (unsigned long)gI2cTrasmissionFailCount);
				fflush(stderr);
			}
			gNextI2CFailReportOnNewLine = false;
			
			glastI2cSuccessful = false;
		}
		else
		{	/* Successful read */
			glastI2cSuccessful = true;
			
			if(gHmiEvent.sequenceCount == gHaltCommand.sequenceCount)
			{	/* sequence count returned by HMI matches that written from test box */
				gLastSequenceMatch = true;
				
				ledRedOff();
				ledGreenOn();
			}
			else
			{	/* sequence count mismatch */
				if(gLastSequenceMatch)
				{	/* first failure after previous match */
					fprintf(stderr, "\nTest box Sequence Count (%lu) does not match that returned from HMI (%lu)", (unsigned long)gHaltCommand.sequenceCount, (unsigned long)gHmiEvent.sequenceCount);
					fflush(stderr);
				}
				gLastSequenceMatch = false;
				
				ledRedOn();
				ledGreenOff();
			}
			checkForButtonPress(&gLeftButtonPressInterval, &gCentreButtonPressInterval, &gRightButtonPressInterval);
		}
		
		
		bool bRead = true;	/* record the result of the read operation */
		logAppendHaltEntry(bRead, success, gLeftButtonPressInterval, gCentreButtonPressInterval, gRightButtonPressInterval);
		/* Reset the recorded button press durations once they have been logged */
		gLeftButtonPressInterval = 0;
		gCentreButtonPressInterval = 0;
		gRightButtonPressInterval = 0;
		
	}
}

uint8_t leftButtonStatus_Last;
uint8_t centreButtonStatus_Last;
uint8_t rightButtonStatus_Last;

uint32_t markLeftButtonPress_ms;
uint32_t markCentreButtonPress_ms;
uint32_t markRightButtonPress_ms;

static void checkForButtonPress(uint32_t* pLeftButtonPressInterval, uint32_t* pCentreButtonPressInterval, uint32_t* pRightButtonPressInterval)
{
	if(leftButtonStatus_Last != gHmiEvent.leftButtonStatus)
	{	/* status changed */
		leftButtonStatus_Last = gHmiEvent.leftButtonStatus;
		if(gHmiEvent.leftButtonStatus)
		{	/* press */
			markLeftButtonPress_ms = gdw_msTimeCount;

#ifdef MONITOR_SOME_ACTIVITY
			printf("\nLeft Button Press...");
#endif /* MONITOR_SOME_ACTIVITY */
		}
		else
		{	/* release */
			OnHmiButtonRelease_Left(pLeftButtonPressInterval);
		}
	}
	
	if(centreButtonStatus_Last != gHmiEvent.centreButtonStatus)
	{	/* status changed */
		centreButtonStatus_Last = gHmiEvent.centreButtonStatus;
		if(gHmiEvent.centreButtonStatus)
		{	/* press */
			markCentreButtonPress_ms = gdw_msTimeCount;

#ifdef MONITOR_SOME_ACTIVITY
			printf("\nCentre Button Press...");
#endif /* MONITOR_SOME_ACTIVITY */
		}
		else
		{	/* release */
			OnHmiButtonRelease_Centre(pCentreButtonPressInterval);
		}
	}
	
	if(rightButtonStatus_Last != gHmiEvent.rightButtonStatus)
	{	/* status changed */
		rightButtonStatus_Last = gHmiEvent.rightButtonStatus;
		if(gHmiEvent.rightButtonStatus)
		{	/* press */
			markRightButtonPress_ms = gdw_msTimeCount;

#ifdef MONITOR_SOME_ACTIVITY
			printf("\nRight Button Press...");
#endif /* MONITOR_SOME_ACTIVITY */
		}
		else
		{	/* release */
			OnHmiButtonRelease_Right(pRightButtonPressInterval);
		}
	}
}

static void OnHmiButtonRelease_Left(uint32_t* pButtonPressInterval)
{
	uint32_t diff = gdw_msTimeCount -markLeftButtonPress_ms;
	*pButtonPressInterval = diff;
#ifdef MONITOR_SOME_ACTIVITY
	printf("\rLeft Button Press-Release: %lu", (unsigned long)diff);
#endif /* MONITOR_SOME_ACTIVITY */

	gHaltCommand.buttonsColourIndex = Green_BacklightColourIndex;
	gButtonBacklightSequence_Pause = true;
	gMarkAtButtonRelease_100ms = gdw_100msTimeCount;
	
	gHmiCommandPending = true;	/* Indicate an update is required to the HMI */
}

static void OnHmiButtonRelease_Centre(uint32_t* pButtonPressInterval)
{
	uint32_t diff = gdw_msTimeCount -markCentreButtonPress_ms;
	*pButtonPressInterval = diff;
#ifdef MONITOR_SOME_ACTIVITY
	printf("\rCentre Button Press-Release: %lu", (unsigned long)diff);
#endif /* MONITOR_SOME_ACTIVITY */

	gHaltCommand.buttonsColourIndex = Red_BacklightColourIndex;
	gButtonBacklightSequence_Pause = true;
	gMarkAtButtonRelease_100ms = gdw_100msTimeCount;
	
	gHmiCommandPending = true;	/* Indicate an update is required to the HMI */
}

static void OnHmiButtonRelease_Right(uint32_t* pButtonPressInterval)
{
	uint32_t diff = gdw_msTimeCount -markRightButtonPress_ms;
	*pButtonPressInterval = diff;
#ifdef MONITOR_SOME_ACTIVITY
	printf("\rRight Button Press-Release: %lu", (unsigned long)diff);
#endif /* MONITOR_SOME_ACTIVITY */

	gHaltCommand.buttonsColourIndex = Blue_BacklightColourIndex;
	gButtonBacklightSequence_Pause = true;
	gMarkAtButtonRelease_100ms = gdw_100msTimeCount;
	
	gHmiCommandPending = true;	/* Indicate an update is required to the HMI */
}

uint32_t dwTemp;
/*
1 The Progress Bar shall be initialised with all segments off.
	The HMI will subsequently increase the illuminatation of the progress bar in response
	to commands sent from the test box and in a sequence of steps, the number
      of which shall be equal to the count of segments.
   At each step, one segment will be added to the illumination, in order, starting from 
      the left hand side and progressing to the right.
   The colour of the illumination shall be white.
   The interval between each step shall be one second.
   Once the the progress bar is fully illuminated, the test box shall send another message,
	after a one second delay, commanding the HMI to switch off all progress bar LEDs for
	one second.
	This sequence will be repeated throughout the duration of the test.

	step :
	0 = All off
	1 - 20 = number of segments illuminated white

*/
static void manageProgressBarUpdate(void)
{
	static uint32_t dwSeconds = 0;

	uint32_t dwDiff = gdwSecondsAlive -dwSeconds;
	if(dwDiff >= ProgressBarUpdateInterval_s)
	{	/* Every 1 s */
		dwSeconds = gdwSecondsAlive;
		dwTemp = dwSeconds;
	
		uint8_t progressBarStep = gHaltCommand.progressBarStep;
		progressBarStep++;	/* increase the progress bar to the next step */
		if(progressBarStep > ProgressBarSegmentCount)
		{	/* progress bar fully illuminated */
			progressBarStep = 0;	/* progress bar all off */
		}
		gHaltCommand.progressBarStep = progressBarStep;
		
		gHmiCommandPending = true;	/* Indicate an update is required to the HMI */
	}
}

/*
2 The display shall be divided into two logical equal halves, left and right, with
   one of the two halves being illuminated (white) at any time.
   The illumination shall alternate between the two halves with each half
   being illuminated for 3 seconds.
	The test box will issue a command to the HMI to switch the display on at the start of the test
	and when the test is resumed.
	The test box will issue a command to the HMI to switch the display off at the end of the test
	and when the test is paused.

*/
static void manageDisplayUpdate(void)
{
	static uint32_t dwSeconds = 0;

	uint32_t dwDiff = gdwSecondsAlive -dwSeconds;
	if(dwDiff >= DisplayUpdateInterval_s)
	{	/* Every 3 s */
		dwSeconds = gdwSecondsAlive;
		dwTemp = dwSeconds;
	
		if(gHaltCommand.displayStep == displayLeftIsOn_HaltStep)
		{
			gHaltCommand.displayStep = displayRightIsOn_HaltStep;
		}
		else
		{
			gHaltCommand.displayStep = displayLeftIsOn_HaltStep;
		}
		
		gHmiCommandPending = true;	/* Indicate an update is required to the HMI */
	}
}

static bool isButtonBacklightSequencePausedByPress(void)
{
	if(gButtonBacklightSequence_Pause)
	{
		uint32_t dwDiff = gdw_100msTimeCount -gMarkAtButtonRelease_100ms;
		if(dwDiff >= BacklightPauseIntervalOnButtonEvent)
		{	/* 1 s elapsed since button release */
			gButtonBacklightSequence_Pause = false;
		}
	}

	return gButtonBacklightSequence_Pause;
}

/*
3 All back-lighting for the indicators and buttons shall illuminate to the colour 
	commanded from the test box, throughout the duration of the test.
   The test box shall send commands that result in all back-light LEDs repeatedly 
	cycling through the colour sequence red-green-blue-white; Each colour in the 
	sequence shall endure for 5 seconds. When a HMI button is pressed, the test 
	box will delay the next command in the sequence until
   an interval of one second has ellapsed since the button press.
*/
static void manageIndicatorBacklightUpdate(void)
{
	static uint32_t dwSeconds = 0;

	if(isButtonBacklightSequencePausedByPress())
	{}	/* Pause the sequence immediately after a button press */
	else
	{
		uint32_t dwDiff = gdwSecondsAlive -dwSeconds;
		if((dwSeconds == 0) && (gdwSecondsAlive == 1))
		{	/* One second after program started */
			/* Force the indicator backighting timing to start now 
				rather than waiting for the first interval to elapse */
			dwDiff = IndicatorBacklightUpdateInterval;
		}
		if(dwDiff >= IndicatorBacklightUpdateInterval)
		{	/* Every 5 s */
			dwSeconds = gdwSecondsAlive;
			dwTemp = dwSeconds;
		
			uint8_t indicatorsColourIndex = gHaltCommand.indicatorsColourIndex;
			indicatorsColourIndex++;	/* step through the colour sequence */
			if(indicatorsColourIndex > White_BacklightColourIndex)
			{	/* end of sequence */
				indicatorsColourIndex = Red_BacklightColourIndex;	/* reset to start of sequence */
			}
			gHaltCommand.indicatorsColourIndex = indicatorsColourIndex;
			
			gHmiCommandPending = true;	/* Indicate an update is required to the HMI */
		}
	}
}

/*
3 All back-lighting for the indicators and buttons shall illuminate to the colour 
	commanded from the test box, throughout the duration of the test.
   The test box shall send commands that result in all back-light LEDs repeatedly 
	cycling through the colour sequence red-green-blue-white; Each colour in the 
	sequence shall endure for 5 seconds. When a HMI button is pressed, the test 
	box will delay the next command in the sequence until
   an interval of one second has ellapsed since the button press.
   
4 Upon detecting the power button pressed and subsequntly released, the test box
   shall send a message to the HMI commanding all button back-lighting illuminate green;
   The colour shall endure until the HMI commands otherwise.

5 Upon detecting the middle button pressed and subsequntly released, the test box
   shall send a message to the HMI commanding all button back-lighting illuminate red;
   The colour shall endure until the HMI commands otherwise.
   
6 Upon detecting the rightmost button pressed and subsequntly released, the test box
   shall send a message to the HMI commanding all button back-lighting illuminate blue;
   The colour shall endure until the HMI commands otherwise.
*/
static void manageButtonsBacklightUpdate(void)
{
	if(isButtonBacklightSequencePausedByPress())
	{}	/* Pause the sequence immediately after a button press */
	else
	{
		gHaltCommand.buttonsColourIndex = gHaltCommand.indicatorsColourIndex;
	}
}

			
			
static void manageHmiCmdUpdate(int i2cChannel)
{
	static bool firstSuccessfulWrite = false;
	ssize_t txCountResult;
	
	if(gHmiCommandPending)
	{	/* update is required to the HMI */
		gHmiCommandPending = false;
		
		gSequenceCount++;	/* Increment the sequence count before each tx */
		gHaltCommand.sequenceCount = gSequenceCount;
		gHaltCommand.command = HaltCmd;
		
		uint8_t payloadCount = sizeof(PscHALTCommand_t);
		txCountResult = write(i2cChannel, (const void*)&gHaltCommand, payloadCount);
		bool success = (txCountResult == payloadCount);
		if(success == false)
		{	/* write failed */
			gI2cTrasmissionFailCount++;	/* Keep track of the number of I2C failures */
			ledRedOn();
			ledGreenOff();

			if(glastI2cSuccessful || gNextI2CFailReportOnNewLine)
			{
				fprintf(stderr, "\nI2C transmission failures : %lu", (unsigned long)gI2cTrasmissionFailCount);
				fflush(stderr);
			}
			else
			{
				fprintf(stderr, "\rI2C transmission failures : %lu", (unsigned long)gI2cTrasmissionFailCount);
				fflush(stderr);
			}
			gNextI2CFailReportOnNewLine = false;

			glastI2cSuccessful = false;

			if(firstSuccessfulWrite == false)
			{	/* All I2C write transmissions to the HMI have so far failed */
				/* Don't increment the sequence count until contact is made with the HMI */
				gSequenceCount = 0;
			}
		}
		else
		{	/* write successful */
			gdwWriteToHmiCount++;
			glastI2cSuccessful = true;
			ledRedOff();
			ledGreenOn();
			
			if(firstSuccessfulWrite == false)
			{	/* This is the first successful I2C write transmission to the HMI */
				firstSuccessfulWrite = true;
			}
		}
	
		bool bRead = false;	/* record the result of the write operation */
		logAppendHaltEntry(bRead, success, 0,0,0);
	}
}

/**** END OF FILE ****/