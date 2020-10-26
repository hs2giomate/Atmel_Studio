/* 
* FRAM_Memory_Class.cpp
*
* Created: 2/14/2020 1:01:44 PM
* Author: GMateusDP
*/


#include "FRAM_Memory_Class.h"


FRAMMemoryStruct*   framMemory=NULL;

// default constructor
FRAM_Memory_Class::FRAM_Memory_Class()
{
		appParameters=defaultParameters;
		spiPort=0;
		memoryAddress=framMemory;
} //FRAM_Memory_Class

// default destructor
FRAM_Memory_Class::~FRAM_Memory_Class()
{
} //~FRAM_Memory_Class

FRAM_Memory_Class::FRAM_Memory_Class(uint8_t sp){
	spiPort=sp;
	memoryAddress=framMemory;
	
}
FRAM_Memory_Class::FRAM_Memory_Class(spi_m_sync_descriptor *spi_des)
{
	appParameters=defaultParameters;
	spi_descriptor=spi_des;
	memoryAddress=framMemory;
} //FlashMemoryClass



FRAM_Memory_Class::FRAM_Memory_Class( UserParameters* p){
	
	appParameters=*p;
	spiPort=0;
	memoryAddress=framMemory;
}


void	FRAM_Memory_Class::Init(UserParameters* p){
		appParameters=*p;
		spiPort=0;
	memoryAddress=0x00;
	
	spi.init();
	FramOpen();
	WriteStatusRegister(0x00);
	FramClose();
	
}
void	FRAM_Memory_Class::Init(void){
	spi.SetDescriptor(spi_descriptor);
	spi.init();


	memoryAddress=0x00;
	
	FramOpen();
	//	WriteStatusRegister(0x02);
	//	SoftwareReset();
	//	ChipErase();
	status=ReadStatusRegister();
	//	functionsRegister=ReadFunctionRegister();
	
	FramClose();
	
}

void FRAM_Memory_Class::flushConfigurationData(UserParameters *data)
{
//	gpio_set_pin_level(LED0,true);
	UserParameters	appData;
	appData=*data;
	FramOpen();
	

	//eraseFlash((uint32_t)&memory->parameters, sizeof(userParameters));
	WriteAddress((uint8_t*)&appData,(uint32_t)&memoryAddress->parameters, sizeof(UserParameters));
	FramClose();
	//gpio_set_pin_level(LED0,true);
}
void FRAM_Memory_Class::flushParameters(UserParameters *data)
{
	//gpio_set_pin_level(LED0,true);
	UserParameters	appData;
	appData=*data;
	FramOpen();
	
	//eraseFlash((uint32_t)&memory->parameters, sizeof(userParameters));
	WriteFRAM((uint32_t)&memoryAddress->parameters, &appData, sizeof(UserParameters));
	FramClose();
	//gpio_set_pin_level(LED0,true);
}

void FRAM_Memory_Class::readConfigurationData(UserParameters *data)
{
	UserParameters	appData;

	FramOpen();
	ReadFRAM(&appData, (uint32_t)&memoryAddress->parameters, sizeof(UserParameters));
	FramClose();

		memcpy(data, &appData, sizeof(UserParameters));

		
	
}
void FRAM_Memory_Class::readParameters(UserParameters *framParameters)
{
	UserParameters	appData;

	FramOpen();
	ReadFRAM(&appData, (uint32_t)&memoryAddress->parameters, sizeof(UserParameters));
	FramClose();

	if  ((appData.flapperValveOffset == 46)){
		memcpy(framParameters, &appData, sizeof(UserParameters));
	//	gpio_set_pin_level(LED0,true);
		
		
	}
}

UserParameters FRAM_Memory_Class::readConfigurationData(UserParameters& p)
{
	
	FramOpen();
	ReadAddress((uint8_t*)&p, (uint32_t)&memoryAddress->parameters, sizeof(UserParameters));
	FramClose();
	return	p;

}

void FRAM_Memory_Class::initializeConfigurationData(UserParameters *data){
	
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



void FRAM_Memory_Class::FramOpen(void)
{
	spi.Enable();
	
	
}
bool  FRAM_Memory_Class::IsReady(void){
	
	FramOpen();
	status = ReadStatusRegister();
	FramClose();
	return	(status&0x02)==1;
}
uint8_t FRAM_Memory_Class::ReadFRAM(void* dest, uint32_t src, uint32_t size)
{
	CE_Low_FRAM();
	Send_Byte(kFRAM_READ);
	//	send 3 address bytes
	//Send_Byte(((src & 0xFFFFFF) >> 16));
	Send_Byte(((src & 0xFFFF) >> 8));
	Send_Byte(src & 0xFF);
	spi.Read((uint8_t *)dest, (uint16_t)size);
	CE_High_FRAM();
	return (uint8_t)src;
}
void FRAM_Memory_Class::CE_Low_FRAM(void)
{

	//PORT->Group[0].OUTCLR.reg = (1UL << 27);
		gpio_set_pin_level(CS_FRAM,
		// <y> Initial level
		// <id> pad_initial_level
		// <false"> Low
		// <true"> High
		false);
		gpio_set_pin_level(CS_FLASH,true);
}

void FRAM_Memory_Class::CE_High_FRAM(void)
{

	//PORT->Group[0].OUTSET.reg = (1UL << 27);
	gpio_set_pin_level(CS_FRAM,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);
	gpio_set_pin_level(CS_FLASH,true);
}
uint8_t FRAM_Memory_Class::Send_Byte(uint8_t byte)
{
	
	spi.Write(&byte,1);
	return byte;
}

uint8_t FRAM_Memory_Class::Get_Byte(void)
{
	uint8_t byte;
	spi.Read(&byte,1);
	
	return byte;
}

void FRAM_Memory_Class::WREN(void)
{
	CE_Low_FRAM();			// enable device
	Send_Byte(kFRAM_WREN);	// send WREN command
	CE_High_FRAM();			// disable device
}


void FRAM_Memory_Class::WRDI(void)
{
	CE_Low_FRAM();			// enable device
	Send_Byte(kFRAM_WRDI);	// send WRDI command
	CE_High_FRAM();			// disable device
}
uint8_t FRAM_Memory_Class::WriteFRAM(uint32_t dest, const void* src, uint32_t size){
	const uint8_t*	srcPtr = (const uint8_t*)src;
		WREN();
		
	while ((ReadStatusRegister()&0x02)==0);
	
	while (size > 0){
		uint16_t	i;
		CE_Low_FRAM();
		Send_Byte(kFRAM_WRITE);
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
	

void  FRAM_Memory_Class::WaitBusy(void){
		uint8_t	status = ReadStatusRegister();
		
		while ((status & kFRAM_READ) == kFRAM_READ)	// waste time until not busy
		status = ReadStatusRegister();
	}
uint8_t  FRAM_Memory_Class::ReadStatusRegister(void){
		uint8_t byte = 0;
		CE_Low_FRAM();			// enable device
		Send_Byte(kFRAM_RDSR);	// send RDSR command
		byte = Get_Byte();		/* receive byte */
		CE_High_FRAM();			// disable device
		return byte;
	}
uint8_t  FRAM_Memory_Class::WriteStatusRegister(uint8_t byte){
		
		CE_Low_FRAM();			// enable device
		Send_Byte(kFRAM_WRSR);	// send RDSR command
		Send_Byte(byte);
		CE_High_FRAM();			// disable device
		return byte;
	}
void FRAM_Memory_Class::FramClose(void){
	
		spi.Disable();
	}

uint32_t	FRAM_Memory_Class::WriteAddress(uint8_t *p,uint16_t addr, uint32_t size){
	uint32_t	w;
	//while(!IsReady());
	FramOpen();
	WREN();
	while ((ReadStatusRegister()&0x02)==0);
	CE_Low_FRAM();
	localBuffer[0]=kFRAM_WRITE;
	localBuffer[1]=(uint8_t)((addr & 0xFF00) >> 8);
	localBuffer[2]=(uint8_t)(addr&0xFF);
	for (uint32_t i = FRAM_HEADER_SIZE; i <(size)+FRAM_HEADER_SIZE; i++)
	{
		localBuffer[i]=*p;
		p++;
	}


	w=spi.Write(&localBuffer[0],size+FRAM_HEADER_SIZE);

	CE_High_FRAM();
	WRDI();
	FramClose();
	return	w;
	/*return (uint32_t)writeFlash(addr,(void*)p,size);*/
}
uint32_t	FRAM_Memory_Class::ReadAddress(uint8_t *p,uint16_t addr, uint32_t size){
	//while(!IsReady());
	FramOpen();
	CE_Low_FRAM();
	instruction[0]=kFRAM_READ;
	instruction[1]=(uint8_t)((addr & 0xFFFF) >> 8);
	instruction[2]=(uint8_t)(addr&0xFF);
	
	uint32_t	r=spi.Write(instruction,FRAM_HEADER_SIZE);
	r=spi.Read(p,size);
	CE_High_FRAM();
	FramClose();
	
	return	r;
	/*return	(uint32_t)readFlash((void*)p,addr,size);*/
}

FRAM_Memory_Class fram(&SPI_HI3593);