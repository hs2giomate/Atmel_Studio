/*--------------------------------------------------------------------------

ConfigurationCommonDefinitions.h

Interface
Common data structures and definition for GAIN configuration data

Author: Steffen Simon

$Date: 2017-07-13 12:31:56 +0200 (Do, 13. Jul 2017) $
$Revision: 7343 $
$Author: steffen $

--------------------------------------------------------------------------*/
#ifndef CONFIGURATION_COMMONS_H
#define CONFIGURATION_COMMONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

enum
	{
	kDisplayBrightnessLow = 0,
	kDisplayBrightnessHigh = 1
	};

enum
	{
	kTemperatureUnitCelsius = 0,
	kTemperatureUnitFahrenheit = 1
	};

enum
	{
	kProgrammeA350 = 0,
	kProgrammeA380 = 1
	};

#ifdef MGS_ZST_Revision
#pragma pack(2)
#endif

typedef struct
	{
	//> Data structure size
	uint16	size;
	//> Data structure version
	uint16	version;

	uint8	numberOfCycleDescriptions;
	uint8	defaultCycleDescription;
	int16	sizeOfCycleDescription;

	//> Reserved
	uint8	reserved[8];
	} CycleDictionary;

#if (PARTNUMBER == 932)
typedef struct
	{
	uint8	numberOfEntries;
	uint8	flags;
	uint16	manualPower;
	uint16	manualDuration;
	uint8	cycleIndex[6];
	} LRUCycles;
#endif

typedef struct
	{
	//> Data structure size
	uint16	size;
	//> Data structure version
	uint16	version;
	//>	Part number, i.e. 0x0922 (steam oven, convection oven)
	uint16	partNumber;
	//>	Sub part number, i.e. 0x0000 (convection oven), 0x0001 (steam oven)
	uint16	subPartNumber;
	//>	Programme ID, i.e 0xA350, 0xA380, 0xB787, 0x1234, ...
	uint16	programmeID;

	//> Display brightness
	uint8	displayBrighness;
	//> Temperature unit
	uint8	temperatureUnit;
	//> Phase loss detection thresholds (mA)
	uint16	phaseLossDetectionThreshold[3];
	//> Reserved
	uint8	reserved[10];
	} ConfigurationDataCommons;

#ifdef MGS_ZST_Revision
#pragma pack()
#endif

#ifdef __cplusplus
	}
#endif

#endif	// CONFIGURATION_COMMONS_H
