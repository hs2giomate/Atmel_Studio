/* 
* FRAM_Memory_Class.h
*
* Created: 2/14/2020 1:01:44 PM
* Author: GMateusDP
*/


#ifndef __FRAM_MEMORY_CLASS_H__
#define __FRAM_MEMORY_CLASS_H__

#include "Parameters.h"
#include "string.h"
#include "SPI_Asyn_Class.h"


#define kFlashRead			0x03
#define kFlashWREnable		0x06
#define kFlashWRinhibit		0x04
#define kFlashHighSpeedRead	0x0B
#define kFlash4KSectorErase	0x20
#define kFlash32KBlockErase	0x52
#define kFlash64KBlockErase	0xD8
#define kFlashChipErase		0x60
#define	kFlashByteProgram	0x02
#define	kFRAMWrite			0x02
#define	kFlashWordProgram	0xAD
#define kFlashReadSR		0x05
#define kFlashEnableWriteSR	0x50
#define kFlashWriteSR		0x01
#define kFlashEnableWrite	0x06
#define kFlashDisableWrite	0x04
#define kFlashReadID		0x90
#define kFlashReadJEDECID	0x9F
#define kFlashEnableSOBusy	0x70
#define kFlashDisableSOBusy	0x80




//static userParameters* Parameters=0x400;

struct FRAMMemoryStruct
{
	
	uint8_t					applicationStateSector[0x400];

	union
	{
		userParameters			parameters;
		uint8_t					ParametersSector[0x200-sizeof(parameters)];
	};
	union
	{
		userPreferences			preferences;
		uint8_t					preferencesSector[0x200-sizeof(preferences)];
	};
			
};

typedef struct FRAMMemoryStruct	FRAMMemoryStruct;

static	FRAMMemoryStruct*   framMemory= NULL;

class FRAM_Memory_Class
{
//variables
public:
	FRAMMemoryStruct*		memory;
	userParameters			appParameters;
		//static SPIClass 	spi0;
		static SPI_Asyn_Class 	spi0;
protected:
private:
	uint8_t spiPort;
	uint32_t	enable;

//functions
public:
	FRAM_Memory_Class();
	FRAM_Memory_Class(uint8_t);
	FRAM_Memory_Class(userParameters*);
	~FRAM_Memory_Class();
	void flushConfigurationData( userParameters *data);
	void flushParameters( userParameters *data);
	void initializeConfigurationData(userParameters *data);
	void readConfigurationData(userParameters *data);
	void readParameters(userParameters *data);
	void	init(userParameters*);
	void	flashOpen(void);
	uint8_t	readFRAM(void* dest, uint32_t src, uint32_t size);
	void	CE_Low_FRAM(void);
	void	CE_High_FRAM(void);
	uint8_t	Send_Byte(uint8_t byte);
	uint8_t	Get_Byte(void);
	void	WREN(void);
	void	WRDI(void);
	uint8_t	writeFRAM(uint32_t dest, const void* src, uint32_t size);
	void	eraseFlash(uint32_t dest, uint32_t size);
	void	WaitBusy(void);
	uint8_t	ReadStatusRegister(void);
	uint8_t	WriteStatusRegister(uint8_t);
	void	flashClose(void);
	void	Sector_Erase(uint32_t Dst);
protected:
uint32_t coreCalculateCRC32(const void *buffer, uint32_t size);
private:
	FRAM_Memory_Class( const FRAM_Memory_Class &c );
	FRAM_Memory_Class& operator=( const FRAM_Memory_Class &c );

}; //FRAM_Memory_Class

#endif //__FRAM_MEMORY_CLASS_H__
