/*
 * flashMemoryCortexModule.c
 *
 * Created: 1/24/2020 11:47:48 AM
 *  Author: GMateusDP
 */ 
#include "flashCortexModule.h"
static flashMemoryStruct*		flashMemory = NULL;
void initDriverFlashing(void){
	coreControllerInitialize();
	corePortsInitialize();
	coreSPIInitialize();
	coreSystemTimerInitialize();
	
	coreSystemTimerOpen();
	coreSPIConfigure(kSPI0, 2400000, 8);
}
void initializeConfigurationData(configurationDataStruct *data)
{
	flushConfigurationData(data);
}

void flushConfigurationData(configurationDataStruct *data)
{
	PORT->Group[0].OUTSET.reg = (1UL << 7);
	configurationDataStruct	appData;

	appData.crc32 = coreCalculateCRC32(data, sizeof(configurationDataStruct));
	flashOpen();
	appData.magic = 0xCAFEBABE;
	appData.size = sizeof(configurationDataStruct);
	appData.version = kConfigurationDataStructVersion;
	eraseFlash((uint32_t)&flashMemory->configurationData, sizeof(configurationDataStruct));
	writeFlash((uint32_t)&flashMemory->configurationData, &appData, sizeof(configurationDataStruct));
	flashClose();
	PORT->Group[0].OUTCLR.reg = (1UL << 7);
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
		PORT->Group[0].OUTSET.reg = (1UL << 4);
	}
	else
	{
		PORT->Group[1].OUTSET.reg = (1UL << 9);
		initializeConfigurationData(&defaultConfiguration);
		memcpy(data, &defaultConfiguration, sizeof(configurationDataStruct));
		
	}
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
