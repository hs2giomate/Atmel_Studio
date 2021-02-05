/* 
* CO_Driver_Class.h
*
* Created: 8/5/2020 3:44:15 PM
* Author: GMateusDP
*/


#ifndef __CO_DRIVER_CLASS_H__
#define __CO_DRIVER_CLASS_H__
#include "compiler.h"
#include "stdint.h"
#include "CO_Definitions.h"
#include "interface.h"


#define CO_LITTLE_ENDIAN

#include "CO_SDO_Class.h"
#include "CO_Emergency_Class.h"
#include "CO_SYNC_Class.h"
#include "CO_NMT_Heartbeat_Class.h"
#include "CO_PDO_Class.h"
#include "CO_HB_Consumer_Class.h"
#if CO_NO_SDO_CLIENT == 1
	#include "CO_SDO_Master.h"
#endif
#if CO_NO_TRACE > 0
	#include "CO_Trace_Class.h"
#endif
class CO_Driver_Class: public CO_Emergency_Class,public	CO_NMT_Heartbeat_Class, public CO_SYNC_Class, 
	public CO_PDO_Class, public CO_HB_Consumer_Class
	#if CO_NO_SDO_CLIENT == 1
		,public CO_SDO_Master, 
	 #endif
	 #if CO_NO_TRACE > 0
		,public CO_Trace_Class
	 #endif
{
//variables
public:
	volatile uint32_t HalCanErrorCode;
	volatile	bool	receivedRxCOCallback,receivedtxCOCallback,receivedIntCOCallback,canopen_ready;
	uint8_t co_rx_buffer_64[64];
	can_message receivedCOMsg;
	CO_CANmodule_t* RxFifo_Callback_CanModule_p;
	can_message msgHeader;
	uint8_t		*error_flags_array;
	int16_t	current_IQFilt,current_IDFilt;
	uint16_t	*compressor_temperatures;
	
protected:
private:
	can_async_descriptor  *canDescr;
	can_message msg;
	CO_ReturnError_t Error,ret,err;
	CO_CANtx_t *buffer;
	uint32_t tpdoDeleted;
	CO_EM_t* em;
	bool_t msgMatched;
	CO_CANrx_t *MsgBuff;
	uint8_t            data[64];

	CO_CANrxMsg_t	CANmessage;
	uint32_t index;
	uint32_t i;
	CO_CANmodule_t         *CAN_Module;
	can_filter  CO_Filter;
	uint8_t slave_node;
	tagSTATUSFLAGS	*status_flags;
	tagERRORFLAGS	*error_flags;
	
//functions
public:
	CO_Driver_Class();
	~CO_Driver_Class();
	void CO_CANsetConfigurationMode(can_async_descriptor *descr);
	CO_ReturnError_t CO_CANsetNormalMode(void);
	CO_ReturnError_t CANmodule_init(
	CO_CANmodule_t         *CANmodule,
	can_async_descriptor   *HALCanObject,
	CO_CANrx_t              rxArray[],
	uint32_t                rxSize,
	CO_CANtx_t              txArray[],
	uint32_t                txSize,
	uint32_t                CANbitRate);
	void CANmodule_Disable(void);
	void CANClearPendingSyncPDOs(void);
	virtual void ProcessInterrupt_Rx(void);
		CO_CANtx_t *CAN_Tx_BufferInit(uint32_t                index,
		uint32_t                ident,
		bool_t                  rtr,
		uint8_t                 noOfBytes,
		bool_t                  syncFlag);
	void CAN_VerifyErrors(void);
	CO_ReturnError_t CAN_Send(CO_CANtx_t *buffer);
	CO_ReturnError_t CAN_Rx_BufferInit(	uint32_t                index,
		uint32_t                ident,
		uint32_t                mask,
		bool_t                  rtr,
		void                   *object,
		void                  (*pFunct)(void *object, const CO_CANrxMsg_t *message));
		void CAN_Polling_Tx(void);
		uint8_t Set_Slave_Node(uint8_t sn);
		uint8_t Get_Slave_ID(void){return slave_node;}

protected:
private:
	CO_Driver_Class( const CO_Driver_Class &c );
	CO_Driver_Class& operator=( const CO_Driver_Class &c );
	
	uint32_t CANrxMsg_ReadIdent(can_message *msg);
	


	void	CAN_RxFifo1MsgPendingCallback(void);
	void	PrepareTxMessage(can_message *msgHeader, CO_CANtx_t *buffer);
	void	ConvertCANMsg2CORxMsg(can_message& msgHeader, CO_CANrxMsg_t *rxMsg);

}; //CO_Driver_Class

extern CO_Driver_Class		*canopen_driver;

#endif //__CO_DRIVER_CLASS_H__
