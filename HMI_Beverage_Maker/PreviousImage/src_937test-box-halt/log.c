
/** \file		log.c
 *
 *  \brief		log HALT results to a file
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "log_Private.h"
#include "mainTimer.h"
#include "pscHALTCommand.h"
#include "main.h"
#include "hmiManualReset.h"

const char *gszFilenamePrefix = "HaltResults/HaltResults_";
const char *gszFilenameExt = ".csv";
char gszFilenameBuffer[255];
int gFileNumber;
FILE* gFileHandle = NULL;
const	int MaxLogFileCount = 999;

/** \brief Tests if the given file already exists
 *
 *  \return  Nothing
 */
static bool logIsFileExists(const char *szFilename);

/** \brief formats a filename for the data log. It will be tested for uniqueness
 *
 *  \return  Nothing
 */
static bool logGetNextFilename(void);

/** \brief searches for a unique filename for the data log that does not already exist
 *
 *  \return  Nothing
 */
static bool logGetNextFreeFilename(void);

/** \brief Re-open a data log file that is being used for this HALT run
 *
 *  \return  Nothing
 */
static bool logReopen(void);

/** \brief Append a string into the data log file
 *
 *  \return  Nothing
 */
static bool logAppendString(const char* str);

/** \brief Append the header at the top of the data log
 *
 *  \return  Nothing
 */
static bool logAppendHeader(void);


bool logOpen(void)
{
	bool bOK;

	bOK = logGetNextFreeFilename();
	if(bOK)
	{
		gFileHandle = fopen(gszFilenameBuffer, "w");
		if(gFileHandle == NULL)
		{
			bOK = false;
			fprintf(stderr, "\nFailed to create log %s"
				"\nCheck the directory exists"
				, gszFilenameBuffer);
			OnFatalError();
		}
		else
		{
			bOK = logAppendHeader();
		}
	}
	
	return bOK;
}

static bool logReopen(void)
{
	bool bOK = false;
	
	if(gFileHandle == NULL)
	{	/* file not already open */
		gFileHandle = fopen(gszFilenameBuffer, "a");
		if(gFileHandle == NULL)
		{
			fprintf(stderr, "\nFailed to re-open log %s", gszFilenameBuffer);
			OnFatalError();
		}
		else
		{
			bOK = true;
		}
	}
	else
	{	/* file already open */
		fprintf(stderr, "\nLog re-open request when already open");
	}
	
	return bOK;
}

static bool logAppendHeader(void)
{
	bool bOK;
	char buf[255];
	
	bOK = logAppendString("3604 Ipeco Beverage Maker HMI HALT");
	if(bOK)
	{
		bOK = logAppendString("\nHALT Started,");
	}
	if(bOK)
	{
		GetDateAndTime(buf, sizeof(buf));
		buf[sizeof(buf) -1] = 0;
		bOK = logAppendString(buf);
	}

	if(bOK)
	{
		bOK = logAppendString("\n\n"
			"Timestamp (ms)"
			",Sequence Count"
			",I2C transfer type (Read / Write)"
			",I2C Transfer OK / FAIL"
			",Command"
			",Progress-Bar Step"
			",Display Step"
			",Display On/Off"
			",Indicators Colour Index"
			",Buttons Colour Index"
			",External Flash Memory Success Count"
			",External Flash Memory Fail Count"
			",Left Button Status"
			",Centre Button Status"
			",Right Button Status"
			",Left Button Press Duration (ms)"
			",Centre Button Press Duration (ms)"
			",Right Button Press Duration (ms)"
			",HMI Manual Reset"
			",Uptime (s)"
			",Last Reset Reason"
			",ADC (Temperature)"
			",ADC (Vcc)"
			",ADC (HMI Supply)"
		);
	}

	return bOK;
}

extern PscHALTCommand_t gHaltCommand;
extern PscHALTEvent_t gHmiEvent;
bool logAppendHaltEntry(bool isRead, bool success, uint32_t leftButtonPressInterval, uint32_t centreButtonPressInterval, uint32_t rightButtonPressInterval)
{
	bool bOK;
	char buf[2000];

	if(isRead)
	{	/* recording the result of an i2c read transfer */
		sprintf(buf, "\n"
			"%lu"	/*Timestamp (ms)*/
			",%lu"	/*Sequence Count*/
			",R"	/*I2C transfer type (Read / Write)*/
			",%s"	/*I2C Transfer Success / Failure*/
			","	/*Command*/
			","	/*Progress-Bar Step*/
			","	/*Display Step*/
			","	/*Display On/Off*/
			","	/*Indicators Colour Index*/
			","	/*Buttons Colour Index*/
			",%lu"	/*External Flash Memory Success Count*/
			",%lu"	/*External Flash Memory Fail Count*/
			",%u"	/*Left Button Status*/
			",%u"	/*Centre Button Status*/
			",%u"	/*Right Button Status*/
			",%lu"	/*Left Button Press Duration (ms)*/
			",%lu"	/*Centre Button Press Duration (ms)*/
			",%lu"	/*Right Button Press Duration (ms)*/
			",%u"	/*HMI Manual Reset*/
			",%lu"	/*Uptime (s)*/
			",%u"	/*Last Reset Reason*/
			",%u"	/*Temperature ADC*/
			",%u"	/*Vcc ADC*/
			",%u"	/*HMI Supply ADC*/
			
			, (unsigned long)gdw_msTimeCount
			, (unsigned long)gHmiEvent.sequenceCount
			, (success ? "OK" : "FAIL")
			, (unsigned long)gHmiEvent.externalFlashMemSuccessCount
			, (unsigned long)gHmiEvent.externalFlashMemFailedCount
			, (unsigned)gHmiEvent.leftButtonStatus
			, (unsigned)gHmiEvent.centreButtonStatus
			, (unsigned)gHmiEvent.rightButtonStatus
			, (unsigned long)leftButtonPressInterval
			, (unsigned long)centreButtonPressInterval
			, (unsigned long)rightButtonPressInterval
			, (unsigned int)gHmiManualResetState
			, (unsigned long)gHmiEvent.secondsAlive
			, (unsigned)gHmiEvent.lastReasonForReset
			, (unsigned)gHmiEvent.temperatureAdcResult
			, (unsigned)gHmiEvent.vccAdcResult
			, (unsigned)gHmiEvent.hmiSupplyVoltageAdcResult
			);
	}
	else
	{	/* recording the result of an i2c write transfer */
		sprintf(buf, "\n"
			"%lu"	/*Timestamp (ms)*/
			",%lu"	/*Sequence Count*/
			",W"	/*I2C transfer type (Read / Write)*/
			",%s"	/*I2C Transfer Success / Failure*/
			",%u"	/*Command*/
			",%u"	/*Progress-Bar Step*/
			",%u"	/*Display Step*/
			",%u"	/*Display On/Off*/
			",%u"	/*Indicators Colour Index*/
			",%u"	/*Buttons Colour Index*/
			","	/*External Flash Memory Success Count*/
			","	/*External Flash Memory Fail Count*/
			","	/*Left Button Status*/
			","	/*Centre Button Status*/
			","	/*Right Button Status*/
			","	/*Left Button Press Duration (ms)*/
			","	/*Centre Button Press Duration (ms)*/
			","	/*Right Button Press Duration (ms)*/
			","	/*HMI Manual Reset*/
			","	/*Uptime (s)*/
			","	/*Last Reset Reason*/
			","	/*Temperature ADC*/
			","	/*Vcc ADC*/
			","	/*HMI Supply ADC*/
			
			, (unsigned long)gdw_msTimeCount
			, (unsigned long)gHaltCommand.sequenceCount
			, (success ? "OK" : "FAIL")
			, (unsigned int)gHaltCommand.command
			, (unsigned int)gHaltCommand.progressBarStep
			, (unsigned int)gHaltCommand.displayStep
			, (unsigned int)gHaltCommand.displayOn
			, (unsigned int)gHaltCommand.indicatorsColourIndex
			, (unsigned int)gHaltCommand.buttonsColourIndex
			);
		
	}

	
	bOK = logAppendString(buf);

	return bOK;
}

static bool logAppendString(const char* str)
{
	bool bOK = false;
	size_t strLength;

	if(gFileHandle == NULL)
	{	/* log file handle not valid */
		fprintf(stderr, "\nAttempt to write to CLOSED log file");
		OnFatalError();
	}
	else
	{	/* log appears to be open */
		strLength = strlen(str);
		if(strLength == fwrite(str, 1, strLength, gFileHandle))
		{
			bOK = true;
		}
		else
		{
			fprintf(stderr, "\nFailed to write to log");
			bOK = false;
			OnFatalError();
		}
	}

	return bOK;
}

void logClose(void)
{
	if(gFileHandle != NULL)
	{
		fflush(gFileHandle);
		fclose(gFileHandle);
		gFileHandle = NULL;
	}
}

static bool logGetNextFreeFilename(void)
{
	bool bOK = false;
	bool bFileAlreadyExists;

	do
	{
		bOK = logGetNextFilename();
		if(bOK)
		{
			bFileAlreadyExists = logIsFileExists(gszFilenameBuffer);
		}
		
	} while(bFileAlreadyExists && bOK);
	
	return bOK;
}

static bool logGetNextFilename(void)
{
	bool bOK = false;

	if(gFileNumber < MaxLogFileCount)
	{
		gFileNumber++;
		sprintf(gszFilenameBuffer, "%s%d%s", gszFilenamePrefix, gFileNumber, gszFilenameExt);
		bOK = true;
	}
	else
	{
		fprintf(stderr, "\nToo many log files, please delete some. Limit is %d", MaxLogFileCount);
		OnFatalError();
	}

	return bOK;
}

static bool logIsFileExists(const char *szFilename)
{
	bool bExists = false;
	
	if( access( szFilename, F_OK ) != -1 ) 
	{	/* file already exists */
		bExists = true;
	}
	
	return bExists;
}

void logPause(uint32_t i2cTrasmissionFailCount)
{
	bool bOK;
	char buf[255];
	
	bOK = logAppendString("\n\nHALT Paused,");
	if(bOK)
	{
		GetDateAndTime(buf, sizeof(buf));
		buf[sizeof(buf) -1] = 0;
		bOK = logAppendString(buf);
	}
		
	if(bOK)
	{
		sprintf(buf, ".    %lu I2C Failures", (unsigned long)i2cTrasmissionFailCount);
		bOK = logAppendString(buf);
	}

	logClose();
}

void logResume(void)
{
	bool bOK = logReopen();

	if(bOK)
	{
		char buf[255];
		
		bOK = logAppendString("\nHALT Resumed,");
		if(bOK)
		{
			GetDateAndTime(buf, sizeof(buf));
			buf[sizeof(buf) -1] = 0;
			bOK = logAppendString(buf);
		}
	}
}

void logFinish(uint32_t i2cTrasmissionFailCount)
{
	if(gFileHandle != NULL)
	{
		char buf[255];
		
		bool bOK = logAppendString("\n\nHALT Stopped,");
		if(bOK)
		{
			GetDateAndTime(buf, sizeof(buf));
			buf[sizeof(buf) -1] = 0;
			bOK = logAppendString(buf);
		}
		
		if(bOK)
		{
			sprintf(buf, ".    %lu I2C Failures", (unsigned long)i2cTrasmissionFailCount);
			bOK = logAppendString(buf);
		}

		logClose();
	}
}

/**** END OF FILE ****/
