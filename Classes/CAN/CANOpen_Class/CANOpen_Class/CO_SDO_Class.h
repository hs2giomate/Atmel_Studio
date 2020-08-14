/* 
* CO_SDO_Class.h
*
* Created: 8/5/2020 2:36:36 PM
* Author: GMateusDP
*/


#ifndef __CO_SDO_CLASS_H__
#define __CO_SDO_CLASS_H__
#include "CO_Definitions.h"
#include "CO_CRC16_Class.h"

typedef union{
    uint8_t  u8[8];  /**< 8 bytes */
    uint32_t u16[4]; /**< 4 words */
    uint32_t u32[2]; /**< 2 double words */
}CO_bytes_t;




#if CO_SDO_BUFFER_SIZE < 7
#error CO_SDO_BUFFER_SIZE must be greater than 7
#endif

class CO_SDO_Class: public	CO_CRC16_Class
{
//variables
public:
protected:
private:
	CO_SDO_t *SDO;
//functions
public:
	CO_SDO_Class();
	~CO_SDO_Class();
	CO_ReturnError_t CO_SDO_init(
		CO_SDO_t               *SDO_init,
		uint32_t                COB_IDClientToServer,
		uint32_t                COB_IDServerToClient,
		uint32_t                ObjDictIndex_SDOServerParameter,
		CO_SDO_t               *parentSDO,
		const CO_OD_entry_t     OD[],
		uint32_t                ODSize,
		CO_OD_extension_t       ODExtensions[],
		uint8_t                 nodeId,
		CO_CANmodule_t         *CANdevRx,
		uint32_t                CANdevRxIdx,
		CO_CANmodule_t         *CANdevTx,
		uint32_t                CANdevTxIdx);
	
	int8_t CO_SDO_process(
	
		bool_t                  NMTisPreOrOperational,
		uint32_t                timeDifference_ms,
		uint32_t                SDOtimeoutTime,
		uint32_t               *timerNext_ms);
	void CO_OD_configure(
		uint32_t                index,
		CO_SDO_abortCode_t    (*pODFunc)(CO_ODF_arg_t *ODF_arg),
		void                   *object,
		uint8_t                *flags,
		uint8_t                 flagsSize);
	void CO_setUint32(uint8_t data[], const uint32_t value);
	uint32_t CO_getUint32(const uint8_t data[]);
	void CO_memcpy(uint8_t dest[], const uint8_t src[], const uint32_t size);
	void CO_memcpySwap2(void* dest, const void* src);
	void CO_memcpySwap4(void* dest, const void* src);
	uint32_t CO_OD_find( uint32_t index);
	uint32_t CO_OD_getAttribute( uint32_t entryNo, uint8_t subIndex);
	uint32_t CO_OD_getLength(uint32_t entryNo, uint8_t subIndex);
	void* CO_OD_getDataPointer(uint32_t entryNo, uint8_t subIndex);
	uint32_t CO_SDO_initTransfer( uint32_t index, uint8_t subIndex);
	uint32_t CO_SDO_readOD( uint32_t SDOBufferSize);
	uint32_t CO_SDO_writeOD(uint32_t length);
	
protected:
private:
	CO_SDO_Class( const CO_SDO_Class &c );
	CO_SDO_Class& operator=( const CO_SDO_Class &c );


uint32_t CO_getUint16(const uint8_t data[]);

void CO_setUint16(uint8_t data[], const uint32_t value);

void CO_memcpySwap8(void* dest, const void* src);


void CO_SDO_initCallback(  void                  (*pFunctSignal)(void));

















uint8_t* CO_OD_getFlagsPointer( uint32_t entryNo, uint8_t subIndex);












	

}; //CO_SDO_Class

#endif //__CO_SDO_CLASS_H__
