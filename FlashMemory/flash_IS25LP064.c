/*
 * flash_IS25LP064.c
 *
 * Created: 1/8/2020 1:54:29 PM
 *  Author: GMateusDP
 */ 

#include "flashIS25LP064.h"
#include "coreTypes.h"

static flashMemoryStruct*		flashMemory = NULL;


const configurationDataStruct	defaultConfiguration =
{
	0,
	0xCAFEBABE,
	sizeof(configurationDataStruct),
	kConfigurationDataStructVersion,

	29,			//	uint16		slow speed currentLimitation;	//	Current Limitation
	1,			//	uint16		changeDirectionSpeed;			//	Motorvelocity during Delay
	0,			//	uint16		changeDirectionToStowDelay;		//	Delay wenn Button während Deploy gedrückt wird
	200,		//	uint16		changeDirectionToDeployDelay;	//	Delay wenn Button während Stow gedrückt wird
	5.0,		//	float		slowSpeed;						//	Motorvelocity Slow
	135.0,		//	float		slowSpeedOffset;				//	Distance from deploy position for entering slow speed
	0.4595,		//	float		distanceCalibrationValue;		//	Calibration, Motorsignals vs. Distance
	8000,		//	time_t		stowTimeoutTime;
	8000,		//	time_t		deployTimeoutTime;
};

int main(void){
	 configurationDataStruct	configuration = defaultConfiguration;
	coreSPIConfigure(kSPI0, 24000000, 8);
	readConfigurationData(&configuration);
	coreSystemTimerWait(1);
	PORT->Group[1].OUTTGL.reg = (1UL << 8);
	PORT->Group[0].OUTCLR.reg = (1UL << 3);
	PORT->Group[0].OUTCLR.reg = (1UL << 2);
	flushConfigurationData(&configuration);
}




void initializeApplicationState(applicationStateStruct *state)
{
	flushApplicationState(state);
}


uint32 coreCalculateCRC32(const void *buffer, uint32 size)
{
	uint32	idx;
	uint32	data, crc32;
	uint8*	pData = (uint8*)buffer;
	uint16	bit;
	
	crc32 = 0xFFFFFFFF;
	for (idx = 0; idx < size; idx++)
	{
		data = (uint32)*pData++;
		for (bit = 0; bit<8; bit++)
		{
			if ((crc32 ^ data) & 1)
			crc32 = (crc32 >> 1) ^ 0xEDB88320L;
			else
			crc32 = (crc32 >> 1) ^ 0;
			data >>= 1;
		}
	}
	crc32 ^= 0xFFFFFFFF;

	return crc32;
}

void flushApplicationState(applicationStateStruct *state)
{
	state->crc32 = coreCalculateCRC32(state, sizeof(applicationStateStruct));
	flashOpen();
	state->magic = 0xCAFEBABE;
	state->size = sizeof(applicationStateStruct);
	state->version = kApplicationStateStructVersion;
	eraseFlash((uint32_t)&flashMemory->applicationState, sizeof(applicationStateStruct));
	writeFlash((uint32_t)&flashMemory->applicationState, state, sizeof(applicationStateStruct));
	flashClose();
}

void readApplicationState(applicationStateStruct *state)
{
	applicationStateStruct	appState;

	flashOpen();
	readFlash(&appState, (uint32_t)&flashMemory->applicationState, sizeof(applicationStateStruct));
	flashClose();

	if  ((appState.magic == 0xCAFEBABE) && (appState.size == sizeof(applicationStateStruct)) && (appState.version == kApplicationStateStructVersion))
	{
		memcpy(state, &appState, sizeof(applicationStateStruct));
	}
	else
	{
		initializeApplicationState(state);
	}
}



void initializeConfigurationData(const configurationDataStruct *data)
{
	flushConfigurationData(data);
}

void flushConfigurationData(const configurationDataStruct *data)
{
	configurationDataStruct	appData(*data);

	appData.crc32 = coreCalculateCRC32(data, sizeof(configurationDataStruct));
	flashOpen();
	appData.magic = 0xCAFEBABE;
	appData.size = sizeof(configurationDataStruct);
	appData.version = kConfigurationDataStructVersion;
	eraseFlash((uint32_t)&flashMemory->configurationData, sizeof(configurationDataStruct));
	writeFlash((uint32_t)&flashMemory->configurationData, &appData, sizeof(configurationDataStruct));
	flashClose();
}

void readConfigurationData(configurationDataStruct *data)
{
	configurationDataStruct	appData;

	flashOpen();
	readFlash(&appData, (uint32_t)&flashMemory->configurationData, sizeof(configurationDataStruct));
	flashClose();

	if  ((appData.magic == 0xCAFEBABE) && (appData.size == sizeof(configurationDataStruct)) && (appData.version == kConfigurationDataStructVersion))
	{
		memcpy(data, &appData, sizeof(configurationDataStruct));
		PORT->Group[0].OUTSET.reg = (1UL << 3);
		
	}
	else
	{
		initializeConfigurationData(&defaultConfiguration);
		memcpy(data, &defaultConfiguration, sizeof(configurationDataStruct));
		PORT->Group[0].OUTSET.reg = (1UL << 2);
	}
}