/* 
* CO_HB_Consumer_Class.h
*
* Created: 8/5/2020 2:14:55 PM
* Author: GMateusDP
*/


#ifndef __CO_HB_CONSUMER_CLASS_H__
#define __CO_HB_CONSUMER_CLASS_H__
#include "CO_Definitions.h"


class CO_HB_Consumer_Class
{
//variables
public:
	CO_CANrxMsg_t *receivedHBMsg;
protected:
private:
	uint8_t	coNode ;
	bool	slaveOperational;
	

//functions
public:
	CO_HB_Consumer_Class();
	~CO_HB_Consumer_Class();
	CO_ReturnError_t CO_HBconsumer_init(
        CO_HBconsumer_t        *HBcons,
        CO_EM_t                *em,
        CO_SDO_t               *SDO,
        const uint32_t          HBconsTime[],
        CO_HBconsNode_t         monitoredNodes[],
        uint8_t                 numberOfMonitoredNodes,
        CO_CANmodule_t         *CANdevRx,
        uint32_t                CANdevRxIdxStart);

	void	HB_SetNode(uint8_t);
	bool	IsSlaveOpeational(void);
/**
 * Process Heartbeat consumer object.
 *
 * Function must be called cyclically.
 *
 * @param HBcons This object.
 * @param NMTisPreOrOperational True if this node is NMT_PRE_OPERATIONAL or NMT_OPERATIONAL.
 * @param timeDifference_ms Time difference from previous function call in [milliseconds].
 */
void CO_HBconsumer_process(
        CO_HBconsumer_t        *HBcons,
        bool_t                  NMTisPreOrOperational,
        uint32_t                timeDifference_ms);
protected:
private:
	CO_HB_Consumer_Class( const CO_HB_Consumer_Class &c );
	CO_HB_Consumer_Class& operator=( const CO_HB_Consumer_Class &c );

}; //CO_HB_Consumer_Class

#endif //__CO_HB_CONSUMER_CLASS_H__
