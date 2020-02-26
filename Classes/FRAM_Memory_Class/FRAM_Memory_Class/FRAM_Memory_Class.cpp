/* 
* FRAM_Memory_Class.cpp
*
* Created: 2/14/2020 1:01:44 PM
* Author: GMateusDP
*/


#include "FRAM_Memory_Class.h"
#include "SPI_Asyn_Class.h"

// default constructor
FRAM_Memory_Class::FRAM_Memory_Class()
{
		appParameters=defaultParameters;
		spiPort=0;
		memory=framMemory;
} //FRAM_Memory_Class

// default destructor
FRAM_Memory_Class::~FRAM_Memory_Class()
{
} //~FRAM_Memory_Class
SPI_Asyn_Class FRAM_Memory_Class::spi0(0);
//userParameters	FRAM_Memory_Class::appParameters;

FRAM_Memory_Class::FRAM_Memory_Class(uint8_t sp){
	spiPort=sp;
	memory=framMemory;
	
}




FRAM_Memory_Class::FRAM_Memory_Class( userParameters* p){
	
	appParameters=*p;
	spiPort=0;
	memory=framMemory;
}


void	FRAM_Memory_Class::init(userParameters* p){
		appParameters=*p;
		spiPort=0;
	memory=0x00;
	
	spi0.init();
	flashOpen();
	WriteStatusRegister(0x00);
	flashClose();
	
}

void FRAM_Memory_Class::flushConfigurationData(userParameters *data)
{
	gpio_set_pin_level(Write_Fail,true);
	userParameters	appData;
	appData=*data;
	flashOpen();
	

	//eraseFlash((uint32_t)&memory->parameters, sizeof(userParameters));
	writeFRAM((uint32_t)&memory->parameters, &appData, sizeof(userParameters));
	flashClose();
	gpio_set_pin_level(Write_OK,true);
}
void FRAM_Memory_Class::flushParameters(userParameters *data)
{
	gpio_set_pin_level(Write_Fail,true);
	userParameters	appData;
	appData=*data;
	flashOpen();
	
	//eraseFlash((uint32_t)&memory->parameters, sizeof(userParameters));
	writeFRAM((uint32_t)&memory->parameters, &appData, sizeof(userParameters));
	flashClose();
	gpio_set_pin_level(Write_OK,true);
}

void FRAM_Memory_Class::readConfigurationData(userParameters *data)
{
	userParameters	appData;

	flashOpen();
	readFRAM(&appData, (uint32_t)&memory->parameters, sizeof(userParameters));
	flashClose();

	if  ((appData.deployParkingLimit == 4600)){
		memcpy(data, &appData, sizeof(userParameters));
		gpio_set_pin_level(Read_OK,true);
	}else{
		gpio_set_pin_level(Read_Fail,true);
		initializeConfigurationData(&appParameters);
		memcpy(data, &appParameters, sizeof(userParameters));
		
	}
}
void FRAM_Memory_Class::readParameters(userParameters *framParameters)
{
	userParameters	appData;

	flashOpen();
	readFRAM(&appData, (uint32_t)&memory->parameters, sizeof(userParameters));
	flashClose();

	if  ((appData.deployParkingLimit == 4600)){
		memcpy(framParameters, &appData, sizeof(userParameters));
		gpio_set_pin_level(Read_OK,true);
		
		
	}
}


void FRAM_Memory_Class::initializeConfigurationData(userParameters *data){
	
	flushConfigurationData(data);
	return;
}

uint32_t FRAM_Memory_Class::coreCalculateCRC32(const void *buffer, uint32_t size)
{
	uint32_t	idx;
	uint32_t	data, crc32;
	uint8_t*	pData = (uint8_t*)buffer;
	uint16_t	bit;
	
	crc32 = 0xFFFFFFFF;
	for (idx = 0; idx < size; idx++)
	{
		data = (uint32_t)*pData++;
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



void FRAM_Memory_Class::flashOpen(void)
{
	spi0.enable();
	
	
}
uint8_t FRAM_Memory_Class::readFRAM(void* dest, uint32_t src, uint32_t size)
{
	CE_Low_FRAM();
	Send_Byte(kFlashRead);
	//	send 3 address bytes
	//Send_Byte(((src & 0xFFFFFF) >> 16));
	Send_Byte(((src & 0xFFFF) >> 8));
	Send_Byte(src & 0xFF);
	spi0.read((uint8_t *)dest, (uint16_t)size);
	CE_High_FRAM();
	return (uint8_t)src;
}
void FRAM_Memory_Class::CE_Low_FRAM(void)
{

	//PORT->Group[0].OUTCLR.reg = (1UL << 27);
		gpio_set_pin_level(Flash_CS,
		// <y> Initial level
		// <id> pad_initial_level
		// <false"> Low
		// <true"> High
		false);
}

void FRAM_Memory_Class::CE_High_FRAM(void)
{

	//PORT->Group[0].OUTSET.reg = (1UL << 27);
	gpio_set_pin_level(Flash_CS,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);
}
uint8_t FRAM_Memory_Class::Send_Byte(uint8_t byte)
{
	
	spi0.write(&byte,1);
	return byte;
}

uint8_t FRAM_Memory_Class::Get_Byte(void)
{
	uint8_t byte;
	spi0.read(&byte,1);
	
	return byte;
}

void FRAM_Memory_Class::WREN(void)
{
	CE_Low_FRAM();			// enable device
	Send_Byte(kFlashWREnable);	// send WREN command
	CE_High_FRAM();			// disable device
}


void FRAM_Memory_Class::WRDI(void)
{
	CE_Low_FRAM();			// enable device
	Send_Byte(kFlashWRinhibit);	// send WRDI command
	CE_High_FRAM();			// disable device
}
uint8_t FRAM_Memory_Class::writeFRAM(uint32_t dest, const void* src, uint32_t size){
	const uint8_t*	srcPtr = (const uint8_t*)src;
		WREN();
		
		while ((ReadStatusRegister()&0x02)==0);
	
	while (size > 0){
		uint16_t	i;
		

		
		CE_Low_FRAM();
		Send_Byte(kFRAMWrite);
		//Send_Byte(((dest & 0xFFFFFF) >> 16));
		Send_Byte(((dest & 0xFFFF) >> 8));
		Send_Byte(dest & 0xFF);
	
		for (i=0; (i < 256) && (i<size); i++){
			Send_Byte(*srcPtr++);
		}
		CE_High_FRAM();
		WRDI();
		size -= i;
		dest += i;
		}
		return *srcPtr;

	}
	
void FRAM_Memory_Class::eraseFlash(uint32_t dest, uint32_t size)
	{
	uint32_t	eraseRemaining = size;
	uint32_t	eraseAddress = dest;

	while (eraseRemaining > 0)
		{
		WREN();
		if (eraseRemaining <= 4 * 1024)
			{
			Sector_Erase(eraseAddress);
			eraseRemaining = 0;
			}
		else
			{
			Sector_Erase(eraseAddress);
			eraseAddress += (4 * 1024);
			eraseRemaining -= (4 * 1024);
			}
/*
		else if (eraseRemaining >= 64 * 1024)
			{
			Block_Erase_64K(eraseAddress);
			eraseAddress += (64 * 1024);
			eraseRemaining -= (64 * 1024);
			}
		else if (eraseRemaining >= 32 * 1024)
			{
			Block_Erase_32K(eraseAddress);
			eraseAddress += (32 * 1024);
			eraseRemaining -= (32 * 1024);
			}
*/
		WaitBusy();
		WRDI();
		}
	}
void  FRAM_Memory_Class::WaitBusy(void){
		uint8_t	status = ReadStatusRegister();
		
		while ((status & kFlashRead) == kFlashRead)	// waste time until not busy
		status = ReadStatusRegister();
	}
uint8_t  FRAM_Memory_Class::ReadStatusRegister(void){
		uint8_t byte = 0;
		CE_Low_FRAM();			// enable device
		Send_Byte(kFlashReadSR);	// send RDSR command
		byte = Get_Byte();		/* receive byte */
		CE_High_FRAM();			// disable device
		return byte;
	}
uint8_t  FRAM_Memory_Class::WriteStatusRegister(uint8_t byte){
		
		CE_Low_FRAM();			// enable device
		Send_Byte(kFlashWriteSR);	// send RDSR command
		Send_Byte(byte);
		CE_High_FRAM();			// disable device
		return byte;
	}
void FRAM_Memory_Class::flashClose(void){
	
		spi0.disable();
	}
void FRAM_Memory_Class::Sector_Erase(uint32_t Dst)
{
	CE_Low_FRAM();				// enable device
	Send_Byte(kFlash4KSectorErase);		// send Sector Erase command
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); // send 3 address bytes
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High_FRAM();				// disable device
}