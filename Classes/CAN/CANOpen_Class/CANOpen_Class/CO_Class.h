/* 
* CO_Class.h
*
* Created: 8/5/2020 2:12:26 PM
* Author: GMateusDP
*/


#ifndef __CO_CLASS_H__
#define __CO_CLASS_H__

#include "CO_Definitions.h"
#include "CO_Driver_Class.h"
#include "CO_OD_Class.h"
#include "CO_SDO_Class.h"
#include "CO_SYNC_Class.h"
#include "CO_PDO_Class.h"
#include "CO_HB_Consumer_Class.h"
#if CO_NO_SDO_CLIENT == 1
	#include "CO_SDO_Master.h"
#endif
#if CO_NO_TRACE > 0
	#include "CO_Trace_Class.h"
#endif
#ifndef CO_USE_GLOBALS
	#include <stdlib.h> /*  for malloc, free */
#endif









#if CO_NO_TRACE > 0
   #ifdef CO_USE_GLOBALS
		#ifndef CO_TRACE_BUFFER_SIZE_FIXED
			 #define CO_TRACE_BUFFER_SIZE_FIXED 100
		#endif
  #endif
#endif


/* Verify features from CO_OD *************************************************/
    /* generate error, if features are not correctly configured for this project */
    #if        CO_NO_NMT_MASTER                           >  1     \
            || CO_NO_SYNC                                 != 1     \
            || CO_NO_EMERGENCY                            != 1     \
            || CO_NO_SDO_SERVER                           == 0     \
            || (CO_NO_SDO_CLIENT != 0 && CO_NO_SDO_CLIENT != 1)    \
            || (CO_NO_RPDO < 1 || CO_NO_RPDO > 0x200)              \
            || (CO_NO_TPDO < 1 || CO_NO_TPDO > 0x200)              \
            || ODL_consumerHeartbeatTime_arrayLength      == 0     \
            || ODL_errorStatusBits_stringLength           < 10
        #error Features from CO_OD.h file are not corectly configured for this project!
    #endif


/* Indexes for CANopenNode message objects ************************************/
    #ifdef ODL_consumerHeartbeatTime_arrayLength
        #define CO_NO_HB_CONS   ODL_consumerHeartbeatTime_arrayLength
    #else
        #define CO_NO_HB_CONS   0
    #endif

    #define CO_RXCAN_NMT       0                                      /*  index for NMT message */
    #define CO_RXCAN_SYNC      1                                      /*  index for SYNC message */
    #define CO_RXCAN_RPDO     (CO_RXCAN_SYNC+CO_NO_SYNC)              /*  start index for RPDO messages */
    #define CO_RXCAN_SDO_SRV  (CO_RXCAN_RPDO+CO_NO_RPDO)              /*  start index for SDO server message (request) */
    #define CO_RXCAN_SDO_CLI  (CO_RXCAN_SDO_SRV+CO_NO_SDO_SERVER)     /*  start index for SDO client message (response) */
    #define CO_RXCAN_CONS_HB  (CO_RXCAN_SDO_CLI+CO_NO_SDO_CLIENT)     /*  start index for Heartbeat Consumer messages */
    /* total number of received CAN messages */
    #define CO_RXCAN_NO_MSGS (1+CO_NO_SYNC+CO_NO_RPDO+CO_NO_SDO_SERVER+CO_NO_SDO_CLIENT+CO_NO_HB_CONS)

    #define CO_TXCAN_NMT       0                                      /*  index for NMT master message */
    #define CO_TXCAN_SYNC      CO_TXCAN_NMT+CO_NO_NMT_MASTER          /*  index for SYNC message */
    #define CO_TXCAN_EMERG    (CO_TXCAN_SYNC+CO_NO_SYNC)              /*  index for Emergency message */
    #define CO_TXCAN_TPDO     (CO_TXCAN_EMERG+CO_NO_EMERGENCY)        /*  start index for TPDO messages */
    #define CO_TXCAN_SDO_SRV  (CO_TXCAN_TPDO+CO_NO_TPDO)              /*  start index for SDO server message (response) */
    #define CO_TXCAN_SDO_CLI  (CO_TXCAN_SDO_SRV+CO_NO_SDO_SERVER)     /*  start index for SDO client message (request) */
    #define CO_TXCAN_HB       (CO_TXCAN_SDO_CLI+CO_NO_SDO_CLIENT)     /*  index for Heartbeat message */
    /* total number of transmitted CAN messages */
    #define CO_TXCAN_NO_MSGS (CO_NO_NMT_MASTER+CO_NO_SYNC+CO_NO_EMERGENCY+CO_NO_TPDO+CO_NO_SDO_SERVER+CO_NO_SDO_CLIENT+1)

extern  CO_t *CO;

class CO_Class: public CO_Driver_Class, public CO_OD_Class
{
//variables
public:
	  CO_t *ptrCO;
	
protected:
private:
	
		uint32_t CO_memoryUsed; /* informative */
		
		 can_async_descriptor  *coCanDescr;
		uint32_t CANdevRxIdx;
		uint32_t ms50;
		uint32_t i;
		CO_ReturnError_t err;
		CO_CANtx_t *NMTM_txBuff;
		uint8_t	coNode;
		uint32_t *timer_next;
		
//functions
public:
	CO_Class();
	~CO_Class();
	
	 uint8_t CO_sendNMTcommand( uint8_t command, uint8_t nodeID);



CO_ReturnError_t CO_init(
        can_async_descriptor *CANbaseAddress,
        uint8_t                 NodeId,
        uint32_t                BitRate);
CO_ReturnError_t Dummy_init(
		can_async_descriptor *CANbaseAddress,
		uint8_t                 NodeId,
		uint32_t                BitRate);



void CO_delete(void);



CO_NMT_reset_cmd_t Process(uint32_t                timeDifference_ms=1000,
      uint32_t               *timerNext_ms=NULL);


bool_t CO_process_SYNC_RPDO(uint32_t                timeDifference_us);



void CO_process_TPDO( bool_t                  syncWas,
        uint32_t                timeDifference_us);
	//	CO_NMT_reset_cmd_t Process(uint32_t                timeDifference_ms);

protected:
private:
	CO_Class( const CO_Class &c );
	CO_Class& operator=( const CO_Class &c );

}; //CO_Class

#endif //__CO_CLASS_H__
