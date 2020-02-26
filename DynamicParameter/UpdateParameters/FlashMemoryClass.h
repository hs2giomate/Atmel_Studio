/*
 * FlashMemoryClass.h
 *
 * Created: 1/30/2020 11:32:02 AM
 *  Author: GMateusDP
 */ 


#ifndef FLASHMEMORYCLASS_H_
#define FLASHMEMORYCLASS_H_

#include "Parameters.h"
#include "string.h"
#include "SPI_Asyn_Class.h"
#include "atmel_start.h"

#define kFlashRead			0x03
#define kFlashWREnable		0x06
#define kFlashWRinhibit		0x04
#define kFlashHighSpeedRead	0x0B
#define kFlash4KSectorErase	0x20
#define kFlash32KBlockErase	0x52
#define kFlash64KBlockErase	0xD8
#define kFlashChipErase		0x60
#define	kFlashByteProgram	0x02
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
	uint8_t	applicationData[240 * 1024];			//	256 KByte

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
		userParameters			parameters;
		uint8_t					configurationDataSector[4096];
	};
	union
	{
		applicationStateStruct	applicationState;
		uint8_t					applicationStateSector[4096];
	};
};

typedef struct flashMemoryStruct	flashMemoryStruct;

static	flashMemoryStruct*   flashMemory= NULL;

class FlashMemoryClass: public SPI_Asyn_Class
{
public:
	FlashMemoryClass(uint8_t);
	FlashMemoryClass(userParameters*);
	//~FlashMemoryClass();
	void flushConfigurationData( userParameters *data);
	void initializeConfigurationData(userParameters *data);
	void readConfigurationData(userParameters *data);
	
	void	flashOpen(void);
	uint8_t	readFlash(void* dest, uint32_t src, uint32_t size);
	void	CE_Low_Flash(void);
	void	CE_High_Flash(void);
	uint8_t	Send_Byte(uint8_t byte);
	uint8_t	Get_Byte(void);
	void	WREN(void);
	void	WRDI(void);
	uint8_t	writeFlash(uint32_t dest, const void* src, uint32_t size);
	void	eraseFlash(uint32_t dest, uint32_t size);
	void	WaitBusy(void);
	uint8_t	ReadStatusRegister(void);
	void	flashClose(void);
	void	Sector_Erase(uint32_t Dst);
	
	
	flashMemoryStruct*		memory;
	userParameters			appParameters;
	
	

	//static SPIClass 	spi0;
	static SPI_Asyn_Class 	spi0;
protected:
	uint32_t coreCalculateCRC32(const void *buffer, uint32 size);
private:
uint8_t spiPort;
};


#endif /* FLASHMEMORYCLASS_H_ */