/* 
* FlashMemoryClass.cpp
*
* Created: 2/5/2020 12:36:16 PM
* Author: GMateusDP
*/


#include "FlashMemoryClass.h"
#include "driver_init.h"

flashMemoryStruct*   flashLayout= NULL;
// default constructor
FlashMemoryClass::FlashMemoryClass()
{
		appParameters=defaultParameters;
		spiPort=0;
		memoryMap=flashLayout;
} //FlashMemoryClass
FlashMemoryClass::FlashMemoryClass(spi_m_sync_descriptor *spi_des)
{
	appParameters=defaultParameters;
	spi_descriptor=spi_des;
	memoryMap=flashLayout;
} //FlashMemoryClass

// default destructor
FlashMemoryClass::~FlashMemoryClass()
{
} //~FlashMemoryClass



FlashMemoryClass::FlashMemoryClass(uint8_t sp){
	spiPort=sp;
	memoryMap=flashLayout;
	
}




FlashMemoryClass::FlashMemoryClass(UserParameters* p){
	
	appParameters=*p;
	memoryMap=flashLayout;
}


void	FlashMemoryClass::Init(UserParameters* p){
		appParameters=*p;
	memoryMap=0x00;
	
	spi.init();
	flashOpen();
	//WriteStatusRegister(kFlashInitialStatus);

	flashClose();
	
}
void	FlashMemoryClass::Init(spi_m_sync_descriptor *spi_des){
	spi_descriptor=spi_des;
	spi.SetDescriptor(spi_descriptor);
	spi.init();


	memoryMap=0x00;
	
	flashOpen();
	//WriteStatusRegister(kFlashInitialStatus);
	flashClose();
	
}
void	FlashMemoryClass::Init(void){
	spi.SetDescriptor(spi_descriptor);
	spi.init();


	memoryMap=0x00;
	
	flashOpen();
//	WriteStatusRegister(0x02);
//	SoftwareReset();
//	ChipErase();
	status=ReadStatusRegister();
//	functionsRegister=ReadFunctionRegister();
	
	flashClose();
	
}

void FlashMemoryClass::flushConfigurationData(UserParameters *data)
{

	UserParameters	appData;
	appData=*data;
	flashOpen();
//	writeFlash((uint32_t)&memoryMap->parameters,&appData, sizeof(userParameters));
	WriteAddress((uint8_t*)&appData,(uint32_t)&memoryMap->parameters, sizeof(UserParameters));
	flashClose();

}

void FlashMemoryClass::readConfigurationData(UserParameters *data)
{
	UserParameters	appData;

	flashOpen();
	readFlash(&appData, (uint32_t)&memoryMap->parameters, sizeof(UserParameters));
	flashClose();


		memcpy(data, &appData, sizeof(UserParameters));

		
	
}

UserParameters FlashMemoryClass::readConfigurationData(UserParameters& p)
{
	
	flashOpen();
	ReadAddress((uint8_t*)&p, (uint32_t)&memoryMap->parameters, sizeof(UserParameters));
	//readFlash((void*)&p, (uint32_t)&memoryMap->parameters, sizeof(userParameters));
	flashClose();
	return	p;

}


void FlashMemoryClass::initializeConfigurationData(UserParameters *data){
	garbage=0;
	flushConfigurationData(data);
	return;
}

uint32_t FlashMemoryClass::coreCalculateCRC32(const void *buffer, uint32_t size)
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



void FlashMemoryClass::flashOpen(void)
{
	garbage=spi.Enable();
	
	
}
uint8_t FlashMemoryClass::readFlash(void* dest, uint32_t src, uint32_t size)
{
	CE_Low_Flash();
	Send_Byte(kFlashRead);
	//	send 3 address bytes
	Send_Byte(((src & 0xFFFFFF) >> 16));
	Send_Byte(((src & 0xFFFF) >> 8));
	Send_Byte(src & 0xFF);
	spi.Read((uint8_t *)dest, (uint16_t)size);
	CE_High_Flash();
	return (uint8_t)src;
}
void FlashMemoryClass::CE_Low_Flash(void)
{
	garbage=0;
	//PORT->Group[0].OUTCLR.reg = (1UL << 27);
		gpio_set_pin_level(CS_FLASH,
		// <y> Initial level
		// <id> pad_initial_level
		// <false"> Low
		// <true"> High
		false);
		gpio_set_pin_level(CS_FRAM,true);
}

void FlashMemoryClass::CE_High_Flash(void)
{
	garbage=0;
	//PORT->Group[0].OUTSET.reg = (1UL << 27);
	gpio_set_pin_level(CS_FLASH,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);
	gpio_set_pin_level(CS_FRAM,true);
}
uint8_t FlashMemoryClass::Send_Byte(uint8_t byte)
{
	//WaitBusy();
	spi.Write(&byte,1);
	return byte;
}

uint8_t FlashMemoryClass::Get_Byte(void)
{
	uint8_t byte;
	//WaitBusy();
	spi.Read(&byte,1);
	
	return byte;
}

void FlashMemoryClass::WREN(void)
{
	do
	{
		CE_Low_Flash();			// enable device
		Send_Byte(kFlashWREnable);	// send WREN command
		//WaitBusy();
		CE_High_Flash();
		status=ReadStatusRegister();
	} while ((status&&(0x02))==0);

}


void FlashMemoryClass::WRDI(void)
{
	CE_Low_Flash();			// enable device
	Send_Byte(kFlashWRinhibit);	// send WRDI command
//	WaitBusy();
	CE_High_Flash();			// disable device
	
}


void FlashMemoryClass::ChipErase(void)
{
	flashOpen();
	CE_Low_Flash();	
	WREN();
	Send_Byte(kFlashChipErase);	// send WRDI command
	//	WaitBusy();
	CE_High_Flash();			// disable device
	delay_ms(100);
	flashClose();
}
uint8_t FlashMemoryClass::writeFlash(uint32_t dest, const void* src, uint32_t size){
	const uint8_t*	srcPtr = (const uint8_t*)src;
		
	while (size > 0){
		uint16_t	i;
		
		WREN();
		CE_Low_Flash();
		Send_Byte(0x02);
		Send_Byte(((dest & 0xFFFFFF) >> 16));
		Send_Byte(((dest & 0xFFFF) >> 8));
		Send_Byte(dest & 0xFF);
	
		for (i=0; (i < 256) && (i<size); i++){
			Send_Byte(*srcPtr++);
		}
		CE_High_Flash();
		WRDI();
		size -= i;
		dest += i;
		}
		return *srcPtr;

	}
	
void FlashMemoryClass::eraseFlash(uint32_t dest, uint32_t size)
	{
	uint32_t	eraseRemaining = size;
	uint32_t	eraseAddress = dest;
	flashOpen();
	while (eraseRemaining > 0)
	{
	WREN();
	if (eraseRemaining <= 4 * 1024)
		{
		Sector_Erase(eraseAddress);
		eraseRemaining = 0;
		}
	else if (eraseRemaining < 64 * 1024)
		{
		Sector_Erase(eraseAddress);
		eraseAddress += (4 * 1024);
		eraseRemaining -= (4 * 1024);
		}

	else if (eraseRemaining >= 64 * 1024)
		{
		Block_Erase_64K(eraseAddress);
		eraseAddress += (64 * 1024);
		eraseRemaining -= (64 * 1024);
		}

/*		else if (eraseRemaining >= 32 * 1024)
		{
		Block_Erase_32K(eraseAddress);
		eraseAddress += (32 * 1024);
		eraseRemaining -= (32 * 1024);
		}
*/
	WaitBusy();
	WRDI();
	}
	flashClose();
}
	
void	FlashMemoryClass::Block_Erase_64K(uint32_t Dst){

		CE_Low_Flash();				// enable device
		Send_Byte(kFlash64KBlockErase);		// send Sector Erase command
		Send_Byte(((Dst & 0xFFFFFF) >> 16)); // send 3 address bytes
		Send_Byte(((Dst & 0xFFFF) >> 8));
		Send_Byte(Dst & 0xFF);
		CE_High_Flash();				// disable device
		
}
uint8_t  FlashMemoryClass::WaitBusy(void){
	status = ReadStatusRegister();
		
		while ((status & kFlashWriteSR) == 1){// waste time until not busy
			status = ReadStatusRegister();
		}
		
		return	status;
	}
bool  FlashMemoryClass::IsReady(void){
	
	flashOpen();
	status = ReadStatusRegister();
	flashClose();
	return	(status&0x01)==0;
}
uint8_t  FlashMemoryClass::ReadStatusRegister(void){
			status=ReadSingleRegister(kFlashReadSR);
		return status;
	}
	
uint8_t  FlashMemoryClass::ReadFunctionRegister(void){
			// disable device
		return ReadSingleRegister(kFlashReadFunction);
	}
	
uint8_t  FlashMemoryClass::ReadSingleRegister(uint8_t r){
	uint8_t b = 0;
	CE_Low_Flash();			// enable device
	Send_Byte(r);	// send RDSR command
	b = Get_Byte();		/* receive byte */
	CE_High_Flash();			// disable device
	return b;
}
uint8_t  FlashMemoryClass::WriteStatusRegister(uint8_t byte){
	
	CE_Low_Flash();			// enable device
	Send_Byte(kFlashWriteSR);	// send RDSR command
	Send_Byte(byte);		/* receive byte */
	CE_High_Flash();			// disable device
	return byte;
}

void FlashMemoryClass::flashClose(void){
	garbage=0;
		spi.Disable();
	}
void FlashMemoryClass::Sector_Erase(uint32_t Dst)
{
	CE_Low_Flash();				// enable device
	Send_Byte(kFlash4KSectorErase);		// send Sector Erase command
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); // send 3 address bytes
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High_Flash();				// disable device
}
void FlashMemoryClass::SoftwareReset()
{
	CE_Low_Flash();				// enable device
	Send_Byte(0x66);
	WaitBusy();
	CE_High_Flash();
	delay_ms(100);
	CE_Low_Flash();
	Send_Byte(0x99);
	CE_High_Flash();				// disable device
}
uint32_t	FlashMemoryClass::WriteAddress(uint8_t *p,uint32_t addr, uint32_t size){
	uint32_t	w;
	while(!IsReady());
	flashOpen();
	WREN();
	CE_Low_Flash();
	localBuffer[0]=kFlashByteProgram;
	localBuffer[1]=(uint8_t)((addr & 0xFFFFFF) >> 16);
	localBuffer[2]=(uint8_t)((addr & 0xFFFF) >> 8);
	localBuffer[3]=(uint8_t)(addr&0xFF);
	for (uint32_t i = 4; i <(size)+4; i++)
	{
		localBuffer[i]=*p;
		p++;
	}


	w=spi.Write(&localBuffer[0],size+4);

	CE_High_Flash();
	WRDI();
	flashClose();
	return	w;
	/*return (uint32_t)writeFlash(addr,(void*)p,size);*/
}
uint32_t	FlashMemoryClass::ReadAddress(uint8_t *p,uint32_t addr, uint32_t size){
	uint8_t	dummy;
	while(!IsReady());
	flashOpen();
	CE_Low_Flash();
	instruction[0]=kFlashRead;
	instruction[1]=(uint8_t)((addr & 0xFFFFFF) >> 16);
	instruction[2]=(uint8_t)((addr & 0xFFFF) >> 8);
	instruction[3]=(uint8_t)(addr&0xFF);
		
	uint32_t	r=spi.Write(instruction,4);
	r=spi.Read(p,size);
	CE_High_Flash();
	flashClose();
	
	return	r;
	/*return	(uint32_t)readFlash((void*)p,addr,size);*/
}
FlashMemoryClass	flash(&SPI_HI3593);