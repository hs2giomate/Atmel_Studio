/*--------------------------------------------------------------------------

ConfigurationCommonDefinitions.h

Interface
Common data structures and definition for GAIN configuration data

Author: Steffen Simon
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



typedef struct
	{
	//> Data structure size
	uint32_t	size;
	//> Data structure version
	uint32_t	version;

	uint8	numberOfCycleDescriptions;
	uint8	defaultCycleDescription;
	int32_t	sizeOfCycleDescription;

	//> Reserved
	//uint8	reserved[8];
	} CycleDictionary;


typedef struct
	{
	uint8	numberOfEntries;
	uint8	flags;
	uint32_t	manualPower;
	uint32_t	manualDuration;
	uint8	cycleIndex[6];
	} LRUCycles;

typedef struct
	{
	//> Data structure size
	uint32_t	size;
	//> Data structure version
	uint32_t	version;
	//>	Part number, i.e. 0x0922 (steam oven, convection oven)
	uint32_t	partNumber;
	//>	Sub part number, i.e. 0x0000 (convection oven), 0x0001 (steam oven)
	uint32_t	subPartNumber;
	//>	Programme ID, i.e 0xA350, 0xA380, 0xB787, 0x1234, ...
	uint32_t	programmeID;

	//> Display brightness
	uint8_t	displayBrighness;
	//> Temperature unit
	uint8	temperatureUnit;
	//> Phase loss detection thresholds (mA)
	uint32_t	phaseLossDetectionThreshold[3];

	uint8	hmiView;	//!> Enables second HMI view. It is possible to switch between two views by maintancen tool

	//> Reserved
	uint8	reserved[9];
	} ConfigurationDataCommons;



#ifdef __cplusplus
	}
#endif

#endif	// CONFIGURATION_COMMONS_H
