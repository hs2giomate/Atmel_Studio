/* 
* MemoryManagment_Class.h
*
* Created: 7/2/2020 10:33:10 AM
* Author: GMateusDP
*/


#ifndef __MEMORYMANAGMENT_CLASS_H__
#define __MEMORYMANAGMENT_CLASS_H__
//#include "RingBuffer.h"
//#include "diagnostics.h"
#include "N25Q256_Class.h"
//#include "PersistenceServices.h"
//#include "States_Class.h"
//#include "DataLoger_Class.h"
#include "FlashHandler_Class.h"
//#include "FastStorage_Class.h"
#include "CRC32_Class.h"




class MemoryFlash_Class: public FlashHandler_Class,public CRC32_Class
{
//variables
public:
	 uint32_t    currentAddress;
	 RingBuffer	ringBuffer;
	 ECSConfiguration	appState;
	 bool initiated;
protected:
private:
	PTR_CTR_STATE	handlerCtrState;
	PTR_HVAC_STATE	handlerAppState;
	uint32_t     _currentAddress;
	FlashPartition logArea;
	uint32_t    _addressOverflow = false;
	volatile uint32_t head;
	volatile uint32_t tail;
	volatile uint32_t index;
	MemoryFlashStruct		header;
	uint32_t	crc32;
	uint8_t	*flashBuffer;
	uint32_t	stackpointerEventloger;
	
	
//functions
public:
	MemoryFlash_Class();
	~MemoryFlash_Class();
	bool	Init(uint32_t flashChipSize =N25Q_FLASH_SIZE);
	uint32_t GetNextAvailableAddress(uint16_t size=0);
	bool IsFull(void);
	uint32_t	ReadPage(RingBuffer *p);
	uint32_t	PagesAvailable(void);
	uint32_t	SaveRingBuffer(void);
	uint32_t	ReadControllerState(ECSConfiguration *as);
	uint32_t	ReadDeafultApplicationState(HVACState& );
	uint8_t		GetErrorCode(void);
	uint32_t	WriteValidApplicationState(HVACState& hs );
	uint32_t	WriteValidConfigurationData(ConfigurationData& cd);
	uint32_t	SaveApplicationState(HVACState& hs );
	uint32_t	SaveCRCAppState(uint32_t crc);
	uint32_t	SaveCurrentState(HVACState& hs);
	  uint32_t	SaveParameters(UserParameters& up);
	    uint32_t	SaveEventLog(uint8_t *evl);
protected:
private:
	MemoryFlash_Class( const MemoryFlash_Class &c );
	MemoryFlash_Class& operator=( const MemoryFlash_Class &c );
	void     _troubleshoot(uint8_t _code, bool printoverride = false);
	bool     SetChipID(uint32_t flashChipSize =N25Q_FLASH_SIZE );
	uint32_t NextIndex(int index);
	uint32_t	ReadControllerState(void);
	bool	AddressCheck(uint32_t _addr, uint32_t size);
		
}; //MemoryManagment_Class
extern	MemoryFlash_Class	memory;

#endif //__MEMORYMANAGMENT_CLASS_H__
