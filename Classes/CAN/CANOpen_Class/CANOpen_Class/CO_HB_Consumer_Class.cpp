/* 
* CO_HB_Consumer_Class.cpp
*
* Created: 8/5/2020 2:14:54 PM
* Author: GMateusDP
*/


#include "CO_HB_Consumer_Class.h"
#include "CO_Driver_Class.h"
#include "CO_Emergency_Class.h"

CO_HB_Consumer_Class	*ptrHBConsumerClass;


static void CO_HBcons_receive(void *object,const CO_CANrxMsg_t *msg){
	CO_HBconsNode_t *HBconsNode;

	HBconsNode = (CO_HBconsNode_t*) object; /* this is the correct pointer type of the first argument */

	/* verify message length */
	if(msg->DLC == 1){
		/* copy data and set 'new message' flag. */
		HBconsNode->NMTstate = msg->data[0];
		HBconsNode->CANrxNew = true;
	}
}
static void CO_HBcons_monitoredNodeConfig(
CO_HBconsumer_t        *HBcons,
uint8_t                 idx,
uint32_t                HBconsTime)
{
	uint32_t COB_ID;
	uint32_t NodeID;
	CO_HBconsNode_t *monitoredNode;

	if(idx >= HBcons->numberOfMonitoredNodes) return;

	NodeID = (uint32_t)((HBconsTime>>16)&0xFF);
	monitoredNode = &HBcons->monitoredNodes[idx];
	monitoredNode->time = (uint32_t)HBconsTime;
	monitoredNode->NMTstate = 0;
	monitoredNode->monStarted = false;
	COB_ID = (uint32_t)idx + 0x700;
	// 	/* is channel used */
// 	if(NodeID && monitoredNode->time){
// 		COB_ID = NodeID + 0x700;
// 	}
// 	else{
// 		COB_ID = 0;
// 		monitoredNode->time = 0;
// 	}

	/* configure Heartbeat consumer CAN reception */
	ptrCODriverClass->CAN_Rx_BufferInit(
	HBcons->CANdevRxIdxStart + idx,
	COB_ID,
	0x7FF,
	0,
	(void*)&HBcons->monitoredNodes[idx],
	CO_HBcons_receive);
}
static CO_SDO_abortCode_t CO_ODF_1016(CO_ODF_arg_t *ODF_arg){
	CO_HBconsumer_t *HBcons;
	uint32_t value;
	CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

	HBcons = (CO_HBconsumer_t*) ODF_arg->object;
	value =ptrCODriverClass->CO_getUint32(ODF_arg->data);

	if(!ODF_arg->reading){
		uint8_t NodeID;
		uint32_t HBconsTime;

		NodeID = (value >> 16U) & 0xFFU;
		HBconsTime = value & 0xFFFFU;

		if((value & 0xFF800000U) != 0){
			ret = CO_SDO_AB_PRAM_INCOMPAT;
		}
		else if((HBconsTime != 0) && (NodeID != 0)){
			uint8_t i;
			/* there must not be more entries with same index and time different than zero */
			for(i = 0U; i<HBcons->numberOfMonitoredNodes; i++){
				uint32_t objectCopy = HBcons->HBconsTime[i];
				uint8_t NodeIDObj = (objectCopy >> 16U) & 0xFFU;
				uint32_t HBconsTimeObj = objectCopy & 0xFFFFU;
				if(((ODF_arg->subIndex-1U) != i) && (HBconsTimeObj != 0) && (NodeID == NodeIDObj)){
					ret = CO_SDO_AB_PRAM_INCOMPAT;
				}
			}
		}
		else{
			ret = CO_SDO_AB_NONE;
		}

		/* Configure */
		if(ret == CO_SDO_AB_NONE){
			CO_HBcons_monitoredNodeConfig(HBcons, ODF_arg->subIndex-1U, value);
		}
	}

	return ret;
}


// default constructor
CO_HB_Consumer_Class::CO_HB_Consumer_Class()
{
} //CO_HB_Consumer_Class

// default destructor
CO_HB_Consumer_Class::~CO_HB_Consumer_Class()
{
} //~CO_HB_Consumer_Class


/******************************************************************************/
CO_ReturnError_t CO_HB_Consumer_Class::CO_HBconsumer_init(
CO_HBconsumer_t        *HBcons,
CO_EM_t                *em,
CO_SDO_t               *SDO,
const uint32_t          HBconsTime[],
CO_HBconsNode_t         monitoredNodes[],
uint8_t                 numberOfMonitoredNodes,
CO_CANmodule_t         *CANdevRx,
uint32_t                CANdevRxIdxStart)
{
	uint8_t i;

	/* verify arguments */
	if(HBcons==NULL || em==NULL || SDO==NULL || HBconsTime==NULL ||
	monitoredNodes==NULL || CANdevRx==NULL){
		return CO_ERROR_ILLEGAL_ARGUMENT;
	}

	/* Configure object variables */
	HBcons->em = em;
	HBcons->HBconsTime = HBconsTime;
	HBcons->monitoredNodes = monitoredNodes;
	HBcons->numberOfMonitoredNodes = numberOfMonitoredNodes;
	HBcons->allMonitoredOperational = 0;
	HBcons->CANdevRx = CANdevRx;
	HBcons->CANdevRxIdxStart = CANdevRxIdxStart;

	for(i=0; i<HBcons->numberOfMonitoredNodes; i++){

			CO_HBcons_monitoredNodeConfig(HBcons, i, HBcons->HBconsTime[i]);
	
		
	}
	

	/* Configure Object dictionary entry at index 0x1016 */
	ptrCODriverClass->CO_OD_configure( OD_H1016_CONSUMER_HB_TIME, CO_ODF_1016, (void*)HBcons, 0, 0);

	return CO_ERROR_NO;
}


/******************************************************************************/
void CO_HB_Consumer_Class::CO_HBconsumer_process(
CO_HBconsumer_t        *HBcons,
bool_t                  NMTisPreOrOperational,
uint32_t                timeDifference_ms)
{
	uint8_t i;
	uint8_t AllMonitoredOperationalCopy;
	CO_HBconsNode_t *monitoredNode;

	AllMonitoredOperationalCopy = 5;
	monitoredNode = &HBcons->monitoredNodes[0];

	if(NMTisPreOrOperational){
		for(i=0; i<HBcons->numberOfMonitoredNodes; i++){
			if(monitoredNode->time){/* is node monitored */
				/* Verify if new Consumer Heartbeat message received */
				if(monitoredNode->CANrxNew){
					if(monitoredNode->NMTstate){
						/* not a bootup message */
						monitoredNode->monStarted = true;
						monitoredNode->timeoutTimer = 0;  /* reset timer */
						timeDifference_ms = 0;
					}
					monitoredNode->CANrxNew = false;
				}
				/* Verify timeout */
				if(monitoredNode->timeoutTimer < monitoredNode->time) monitoredNode->timeoutTimer += timeDifference_ms;

				if(monitoredNode->monStarted){
					if(monitoredNode->timeoutTimer >= monitoredNode->time){
						ptrCODriverClass->EM_ErrorReport(HBcons->em, CO_EM_HEARTBEAT_CONSUMER, CO_EMC_HEARTBEAT, i);
						monitoredNode->NMTstate = 0;
					}
					else if(monitoredNode->NMTstate == 0){
						/* there was a bootup message */
						ptrCODriverClass->EM_ErrorReport(HBcons->em, CO_EM_HB_CONSUMER_REMOTE_RESET, CO_EMC_HEARTBEAT, i);
					}
				}
				if(monitoredNode->NMTstate != CO_NMT_OPERATIONAL)
				AllMonitoredOperationalCopy = 0;
			}
			monitoredNode++;
		}
	}
	else{ /* not in (pre)operational state */
		for(i=0; i<HBcons->numberOfMonitoredNodes; i++){
			monitoredNode->NMTstate = 0;
			monitoredNode->CANrxNew = false;
			monitoredNode->monStarted = false;
			monitoredNode++;
		}
		AllMonitoredOperationalCopy = 0;
	}
	HBcons->allMonitoredOperational = AllMonitoredOperationalCopy;
}

void	CO_HB_Consumer_Class::HB_SetNode(uint8_t nd){
	coNode=nd;
}