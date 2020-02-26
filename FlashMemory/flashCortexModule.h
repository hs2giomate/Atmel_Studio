/*
 * flashCortexModule.h
 *
 * Created: 1/24/2020 11:44:52 AM
 *  Author: GMateusDP
 */ 


#ifndef FLASHCORTEXMODULE_H_
#define FLASHCORTEXMODULE_H_
#include <string.h>
#include "coreController.h"
#include "coreSPI.h"
#include "coreTypes.h"
#include "samd21.h"
#include "coreSystemTimer.h"
#ifdef __cplusplus
extern "C" {
	#endif


struct configurationDataStruct
{
	uint32_t	crc32;
	uint32_t	magic;
	uint32_t	size;
	uint32_t	version;
	uint32_t	portAStatus;
	uint32_t	portBStatus;

	float		fastSpeed;					//	Motorvelocity Fast
	float		slowSpeed;					//	Motorvelocity Slow
	float		positionZ;					//	Position Z (slow)
	float		positionCarrierInGuide;		//	Position Carrier in Guide
	float		positionA;					//	Position A
	float		positionB;					//	Position B
	float		positionC;					//	Position C
	float		maxLossOfVelocity;			//	max. loss of velocity
	float		detectLossOfVelocity;		//	time intervall to detect loss of velocity
	float		distanceCalibrationValue;	//	Calibration, Motorsignals vs. Distance

	float		highCurrentPositionCOffset;	//	Position where to switch to highCurrentLimit
	uint16		lowCurrentLimit;			//	Current Limitation: Number of Steps for electronic resistor
	uint16		highCurrentLimit;			//	Current Limitation: Number of Steps for electronic resistor
};

typedef struct configurationDataStruct	configurationDataStruct;




static const uint32_t	kApplicationStateStructVersion = 1;
static const uint32_t	kConfigurationDataStructVersion = 2;


static  configurationDataStruct	defaultConfiguration =
{
	0,
	0xCAFEBABE,
	sizeof(configurationDataStruct),
	2,
	0xFFFFFFFF,
	0xFFFFFFFF,
	60.0,		//	float		fastSpeed;					//	Motorvelocity Fast
	30.0,		//	float		slowSpeed;					//	Motorvelocity Slow
	395.0,		//	float		positionZ;					//	Position Z (slow)
	397.0,		//	float		positionCarrierInGuide;		//	Position Carrier in Guide
	414.0,		//	float		positionA;					//	Position A
	402.0,		//	float		positionB;					//	Position B
	438.0,		//	float		positionC;					//	Position C
	0.0,		//	float		maxLossOfVelocity;			//	max. loss of velocity
	0.0,		//	float		detectLossOfVelocity;		//	time intervall to detect loss of velocity
	0.18018,	//	float		distanceCalibrationValue;	//	Calibration, Motorsignals vs. Distance
	0.0,		//	float		highCurrentPositionCOffset;	//	Position where to switch to High Current Limit
	12,			//	uint16		lowCurrentLimit;			//	Current Limitation A
	24,			//	uint16		highCurrentLimit;			//	Current Limitation B
};

struct firmwareDescriptionStruct
{
	char		magic[8];
	uint32_t	flags;
	uint32_t	imageSize;
	char		url[128];
	char		hwVersion[128];
	char		swVersion[128];
	uint32_t	crc32;
};

typedef struct firmwareDescriptionStruct	firmwareDescriptionStruct;

struct firmwareBufferStruct
{
	uint8_t	bootloaderData[16 * 1024];				//	16 KByte
	uint8_t	applicationData[240 * 1024];			//	236 KByte

	union
	{
		firmwareDescriptionStruct	description;		//	4 KByte
		uint8_t	descriptionData[4 * 1024];		//	4 KByte
	};
	
	union
	{
		firmwareDescriptionStruct	descriptionCache;	//	4 KByte
		uint8_t	descriptionCacheData[4 * 1024];			//	4 KByte
	};

	uint8_t	reserved[248 * 1024];			//	248 KByte
};

typedef struct firmwareBufferStruct	firmwareBufferStruct;

struct applicationStateStruct
{
	uint32_t	crc32;
	uint32_t	magic;
	uint32_t	size;
	uint32_t	version;
};

typedef struct applicationStateStruct	applicationStateStruct;

struct flashMemoryStruct
{
	firmwareBufferStruct		firmwareBuffer;
	union
	{
		configurationDataStruct	configurationData;
		uint8_t					configurationDataSector[4096];
	};
	union
	{
		applicationStateStruct	applicationState;
		uint8_t					applicationStateSector[4096];
	};
};

typedef struct flashMemoryStruct	flashMemoryStruct;





void flushApplicationState(applicationStateStruct *state);
void initializeApplicationState(applicationStateStruct *state);
void readApplicationState(applicationStateStruct *state);
void initDriverFlashing(void);

void flushConfigurationData( configurationDataStruct *data);
void initializeConfigurationData(configurationDataStruct *data);
void readConfigurationData(configurationDataStruct *data);
uint32 coreCalculateCRC32(const void *buffer, uint32 size);


#ifdef __cplusplus
}
#endif
#endif /* FLASHCORTEXMODULE_H_ */