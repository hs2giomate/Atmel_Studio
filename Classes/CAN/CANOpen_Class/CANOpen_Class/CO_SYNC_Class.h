/* 
* CO_SYNC_Class.h
*
* Created: 8/5/2020 2:58:37 PM
* Author: GMateusDP
*/


#ifndef __CO_SYNC_CLASS_H__
#define __CO_SYNC_CLASS_H__
#include "CO_Definitions.h"


class CO_SYNC_Class
{
//variables
public:
protected:
private:
	CO_CANtx_t *tx_buffer;
	uint8_t node_ID;
	

//functions
public:
	CO_SYNC_Class();
	~CO_SYNC_Class();
	/**
 * Initialize SYNC object.
 *
 * Function must be called in the communication reset section.
 *
 * @param SYNC This object will be initialized.
 * @param em Emergency object.
 * @param SDO SDO server object.
 * @param operatingState Pointer to variable indicating CANopen device NMT internal state.
 * @param COB_ID_SYNCMessage From Object dictionary (index 0x1005).
 * @param communicationCyclePeriod From Object dictionary (index 0x1006).
 * @param synchronousCounterOverflowValue From Object dictionary (index 0x1019).
 * @param CANdevRx CAN device for SYNC reception.
 * @param CANdevRxIdx Index of receive buffer in the above CAN device.
 * @param CANdevTx CAN device for SYNC transmission.
 * @param CANdevTxIdx Index of transmit buffer in the above CAN device.
 *
 * @return #CO_ReturnError_t: CO_ERROR_NO or CO_ERROR_ILLEGAL_ARGUMENT.
 */
	CO_ReturnError_t SYNC_Init(
        CO_SYNC_t              *SYNC,
        CO_EM_t                *em,
        CO_SDO_t               *SDO,
        uint8_t                *operatingState,
        uint32_t                COB_ID_SYNCMessage,
        uint32_t                communicationCyclePeriod,
        uint8_t                 synchronousCounterOverflowValue,
        CO_CANmodule_t         *CANdevRx,
        uint32_t                CANdevRxIdx,
        CO_CANmodule_t         *CANdevTx,
        uint32_t                CANdevTxIdx);


/**
 * Process SYNC communication.
 *
 * Function must be called cyclically.
 *
 * @param SYNC This object.
 * @param timeDifference_us Time difference from previous function call in [microseconds].
 * @param ObjDict_synchronousWindowLength _Synchronous window length_ variable from
 * Object dictionary (index 0x1007).
 *
 * @return 0: No special meaning.
 * @return 1: New SYNC message recently received or was just transmitted.
 * @return 2: SYNC time was just passed out of window.
 */
uint8_t CO_SYNC_process(
        CO_SYNC_t              *SYNC,
        uint32_t                timeDifference_us,
        uint32_t                ObjDict_synchronousWindowLength);
		uint8_t		Send_SYNC_Signal(void);
protected:
private:
	CO_SYNC_Class( const CO_SYNC_Class &c );
	CO_SYNC_Class& operator=( const CO_SYNC_Class &c );

}; //CO_SYNC_Class

#endif //__CO_SYNC_CLASS_H__
