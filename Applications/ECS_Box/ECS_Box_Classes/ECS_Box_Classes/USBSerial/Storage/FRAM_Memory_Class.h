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
#include "SPI_Syn_Class.h"


#define kFRAM_READ			0x03
#define kFRAM_WREN			0x06
#define kFRAM_WRDI			0x04
#define	kFRAM_WRITE			0x02
#define kFRAM_RDSR			0x05
#define kFRAM_WRSR			0x01
#define FRAM_MEMORY_SEGMENT_SIZE 512
#define FRAM_BUFFER_SIZE	64
#define FRAM_HEADER_SIZE	3
#define FRAM_TEST_SIZE		0x100



//static userParameters* Parameters=0x400;

struct FRAMMemoryStruct
{
	
	uint8_t					applicationStateSector[FRAM_MEMORY_SEGMENT_SIZE];

	union
	{
		UserParameters			parameters;
		uint8_t					ParametersSector[FRAM_MEMORY_SEGMENT_SIZE-sizeof(parameters)];
	};
	union
	{
		userPreferences			preferences;
		uint8_t					preferencesSector[FRAM_MEMORY_SEGMENT_SIZE-sizeof(preferences)];
	};
		uint8_t					EventsLog[FRAM_MEMORY_SEGMENT_SIZE];
	
			
};


typedef struct FRAMMemoryStruct	FRAMMemoryStruct;

extern	FRAMMemoryStruct*   framMemory;

class FRAM_Memory_Class
{
//variables
public:

protected:
private:
	uint8_t spiPort,status;
	uint32_t	enable;
	uint8_t		instruction[FRAM_HEADER_SIZE];
	FRAMMemoryStruct*		memoryAddress;
	UserParameters			appParameters;
	//static SPIClass 	spi0;
	spi_m_sync_descriptor		*spi_descriptor;
	SPI_Syn_Class 	spi;
	uint8_t		localBuffer[FRAM_BUFFER_SIZE+FRAM_HEADER_SIZE];
//functions
public:
	FRAM_Memory_Class();
	FRAM_Memory_Class(uint8_t);
	FRAM_Memory_Class(UserParameters*);
	FRAM_Memory_Class(spi_m_sync_descriptor *);
	~FRAM_Memory_Class();
	void flushConfigurationData( UserParameters *data);
	void flushParameters( UserParameters *data);
	void initializeConfigurationData(UserParameters *data);
	void readConfigurationData(UserParameters *data);
	void readParameters(UserParameters *data);
	UserParameters readConfigurationData(UserParameters& p);
	void	Init(UserParameters*);
	void	Init();
	uint8_t	ReadFRAM(void* dest, uint32_t src, uint32_t size);

	uint8_t	WriteFRAM(uint32_t dest, const void* src, uint32_t size);

	void	WaitBusy(void);



	bool	IsReady(void);
	uint32_t	ReadAddress(uint8_t *p,uint16_t addr, uint32_t size);
	uint32_t	WriteAddress(uint8_t *p,uint16_t addr, uint32_t size);
protected:
uint32_t coreCalculateCRC32(const void *buffer, uint32_t size);
private:
	FRAM_Memory_Class( const FRAM_Memory_Class &c );
	FRAM_Memory_Class& operator=( const FRAM_Memory_Class &c );
		void	CE_Low_FRAM(void);
		void	CE_High_FRAM(void);
		uint8_t	Send_Byte(uint8_t byte);
		uint8_t	Get_Byte(void);
		void	WREN(void);
		void	WRDI(void);
		void	FramClose(void);
		void	FramOpen(void);
		uint8_t	ReadStatusRegister(void);
		uint8_t	WriteStatusRegister(uint8_t);
}; //FRAM_Memory_Class

extern FRAM_Memory_Class fram;

#endif //__FRAM_MEMORY_CLASS_H__
