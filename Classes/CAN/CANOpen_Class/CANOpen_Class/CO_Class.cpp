/* 
* CO_Class.cpp
*
* Created: 8/5/2020 2:12:25 PM
* Author: GMateusDP
*/


#include "CO_Class.h"
#include "CO_OD_Class.h"


static	CO_t	COO;
CO_t *CO=NULL;

#ifdef CO_USE_GLOBALS
	static	CO_CANrx_t          *CO_CANmodule_rxArray0;
	static	CO_CANtx_t          *CO_CANmodule_txArray0;
	static	CO_OD_extension_t   *CO_SDO_ODExtensions;
	static	CO_HBconsNode_t     *CO_HBcons_monitoredNodes;
	static	CO_CANmodule_t       COO_CANmodule;
	static	CO_CANrx_t           COO_CANmodule_rxArray0[CO_RXCAN_NO_MSGS];
	static	CO_CANtx_t           COO_CANmodule_txArray0[CO_TXCAN_NO_MSGS];
	static	CO_SDO_t             COO_SDO[CO_NO_SDO_SERVER];
	static	CO_OD_extension_t    COO_SDO_ODExtensions[CO_OD_NoOfElements];
	static	CO_EM_t              COO_EM;
	static	CO_EMpr_t            COO_EMpr;
	static	CO_NMT_t             COO_NMT;
	static	CO_SYNC_t            COO_SYNC;
	static	CO_RPDO_t            COO_RPDO[CO_NO_RPDO];
	static	CO_TPDO_t            COO_TPDO[CO_NO_TPDO];
	static	CO_HBconsumer_t      COO_HBcons;
	static	CO_HBconsNode_t      COO_HBcons_monitoredNodes[CO_NO_HB_CONS];
	#if CO_NO_SDO_CLIENT == 1
		static	CO_SDOclient_t       COO_SDOclient;
	#endif
	#if CO_NO_TRACE > 0
		static	CO_trace_t           COO_trace[CO_NO_TRACE];
		static	uint32_t             COO_traceTimeBuffers[CO_NO_TRACE][CO_TRACE_BUFFER_SIZE_FIXED];
		static	int32_t              COO_traceValueBuffers[CO_NO_TRACE][CO_TRACE_BUFFER_SIZE_FIXED];
		static	uint32_t            *CO_traceTimeBuffers[CO_NO_TRACE];
		static	int32_t             *CO_traceValueBuffers[CO_NO_TRACE];
	#endif
#endif

// default constructor
CO_Class::CO_Class()
{
	ptrCO = CO;
	coCanDescr=NULL;
	CO_memoryUsed=sizeof(COO);
} //CO_Class

// default destructor
CO_Class::~CO_Class()
{
} //~CO_Class



uint8_t CO_Class::CO_sendNMTcommand( uint8_t command, uint8_t nodeID){
	if(NMTM_txBuff == 0){
		/* error, CO_CANtxBufferInit() was not called for this buffer. */
		return CO_ERROR_TX_UNCONFIGURED; /* -11 */
	}
	NMTM_txBuff->data[0] = command;
	NMTM_txBuff->data[1] = nodeID;

	/* Apply NMT command also to this node, if set so. */
	if(nodeID == 0 || nodeID == CO->NMT->nodeId){
		switch(command){
			case CO_NMT_ENTER_OPERATIONAL:
			if((*CO->NMT->emPr->errorRegister) == 0) {
				CO->NMT->operatingState = CO_NMT_OPERATIONAL;
			}
			break;
			case CO_NMT_ENTER_STOPPED:
			CO->NMT->operatingState = CO_NMT_STOPPED;
			break;
			case CO_NMT_ENTER_PRE_OPERATIONAL:
			CO->NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
			break;
			case CO_NMT_RESET_NODE:
			CO->NMT->resetCommand = CO_RESET_APP;
			break;
			case CO_NMT_RESET_COMMUNICATION:
			CO->NMT->resetCommand = CO_RESET_COMM;
			break;
		}
	}

	return CAN_Send( NMTM_txBuff); /* 0 = success */
}

CO_ReturnError_t CO_Class::Dummy_init(
can_async_descriptor   *CANbaseAddress,
uint8_t                 nodeId,
uint32_t                bitRate)
{
	coCanDescr=CANbaseAddress;
	if(  // sizeof(OD_TPDOCommunicationParameter_t) != sizeof(CO_TPDOCommPar_t)
	 sizeof(OD_TPDOMappingParameter_t) != sizeof(CO_TPDOMapPar_t)
	|| sizeof(OD_RPDOCommunicationParameter_t) != sizeof(CO_RPDOCommPar_t)
	||sizeof(OD_RPDOMappingParameter_t) != sizeof(CO_RPDOMapPar_t))
	{
		return CO_ERROR_PARAMETERS;
	}
	return	(CO_ReturnError_t)0;
}

/******************************************************************************/
CO_ReturnError_t CO_Class::CO_init(
can_async_descriptor   *CANbaseAddress,
uint8_t                 nodeId,
uint32_t                bitRate)
{
	coCanDescr=CANbaseAddress;
	coNode=nodeId;
	//coCanDescr=&CAN_CCU;
	
	#ifndef CO_USE_GLOBALS
		uint32_t errCnt;
	#endif
	#if CO_NO_TRACE > 0
		uint32_t CO_traceBufferSize[CO_NO_TRACE];
	#endif

	/* Verify parameters from CO_OD */
	if(   sizeof(OD_TPDOCommunicationParameter_t) != sizeof(CO_TPDOCommPar_t)
	|| sizeof(OD_TPDOMappingParameter_t) != sizeof(CO_TPDOMapPar_t)
	|| sizeof(OD_RPDOCommunicationParameter_t) != sizeof(CO_RPDOCommPar_t)
	|| sizeof(OD_RPDOMappingParameter_t) != sizeof(CO_RPDOMapPar_t))
	{
		return CO_ERROR_PARAMETERS;
	}

	#if CO_NO_SDO_CLIENT == 1
	//	if(sizeof(OD_SDOClientParameter_t) != sizeof(CO_SDOclientPar_t)){
	//		return CO_ERROR_PARAMETERS;
	//	}
	#endif


	/* Initialize CANopen object */
	#ifdef CO_USE_GLOBALS
		CO = &COO;

		CO->CANmodule[0]                    = &COO_CANmodule;
		CO_CANmodule_rxArray0               = &COO_CANmodule_rxArray0[0];
		CO_CANmodule_txArray0               = &COO_CANmodule_txArray0[0];
		for(i=0; i<CO_NO_SDO_SERVER; i++){
			CO->SDO[i]                      = &COO_SDO[i];
		}

		CO_SDO_ODExtensions                 = &COO_SDO_ODExtensions[0];
		CO->em                              = &COO_EM;
		CO->emPr                            = &COO_EMpr;
		CO->NMT                             = &COO_NMT;
		CO->SYNC                            = &COO_SYNC;
		for(i=0; i<CO_NO_RPDO; i++){
			CO->RPDO[i]                     = &COO_RPDO[i];
		}
		for(i=0; i<CO_NO_TPDO; i++){
			CO->TPDO[i]                     = &COO_TPDO[i];
		}
		
		CO->HBcons                          = &COO_HBcons;
		CO_HBcons_monitoredNodes            = &COO_HBcons_monitoredNodes[0];
		#if CO_NO_SDO_CLIENT == 1
			CO->SDOclient                       = &COO_SDOclient;
		#endif
		#if CO_NO_TRACE > 0
		for(i=0; i<CO_NO_TRACE; i++) {
			CO->trace[i]                    = &COO_trace[i];
			CO_traceTimeBuffers[i]          = &COO_traceTimeBuffers[i][0];
			CO_traceValueBuffers[i]         = &COO_traceValueBuffers[i][0];
			CO_traceBufferSize[i]           = CO_TRACE_BUFFER_SIZE_FIXED;
		}
		#endif
	#else
		if(CO == NULL){    /* Use malloc only once */
			CO = &COO;
			CO->CANmodule[0]                    = (CO_CANmodule_t *)    calloc(1, sizeof(CO_CANmodule_t));
			CO_CANmodule_rxArray0               = (CO_CANrx_t *)        calloc(CO_RXCAN_NO_MSGS, sizeof(CO_CANrx_t));
			CO_CANmodule_txArray0               = (CO_CANtx_t *)        calloc(CO_TXCAN_NO_MSGS, sizeof(CO_CANtx_t));
			for(i=0; i<CO_NO_SDO_SERVER; i++){
				CO->SDO[i]                      = (CO_SDO_t *)          calloc(1, sizeof(CO_SDO_t));
			}
			CO_SDO_ODExtensions                 = (CO_OD_extension_t*)  calloc(CO_OD_NoOfElements, sizeof(CO_OD_extension_t));
			CO->em                              = (CO_EM_t *)           calloc(1, sizeof(CO_EM_t));
			CO->emPr                            = (CO_EMpr_t *)         calloc(1, sizeof(CO_EMpr_t));
			CO->NMT                             = (CO_NMT_t *)          calloc(1, sizeof(CO_NMT_t));
			CO->SYNC                            = (CO_SYNC_t *)         calloc(1, sizeof(CO_SYNC_t));
			for(i=0; i<CO_NO_RPDO; i++){
				CO->RPDO[i]                     = (CO_RPDO_t *)         calloc(1, sizeof(CO_RPDO_t));
			}
			for(i=0; i<CO_NO_TPDO; i++){
				CO->TPDO[i]                     = (CO_TPDO_t *)         calloc(1, sizeof(CO_TPDO_t));
			}
			CO->HBcons                          = (CO_HBconsumer_t *)   calloc(1, sizeof(CO_HBconsumer_t));
			CO_HBcons_monitoredNodes            = (CO_HBconsNode_t *)   calloc(CO_NO_HB_CONS, sizeof(CO_HBconsNode_t));
			#if CO_NO_SDO_CLIENT == 1
			CO->SDOclient                       = (CO_SDOclient_t *)    calloc(1, sizeof(CO_SDOclient_t));
			#endif
			#if CO_NO_TRACE > 0
			for(i=0; i<CO_NO_TRACE; i++) {
				CO->trace[i]                    = (CO_trace_t *)        calloc(1, sizeof(CO_trace_t));
				CO_traceTimeBuffers[i]          = (uint32_t *)          calloc(OD_traceConfig[i].size, sizeof(uint32_t));
				CO_traceValueBuffers[i]         = (int32_t *)           calloc(OD_traceConfig[i].size, sizeof(int32_t));
				if(CO_traceTimeBuffers[i] != NULL && CO_traceValueBuffers[i] != NULL) {
					CO_traceBufferSize[i] = OD_traceConfig[i].size;
					} else {
					CO_traceBufferSize[i] = 0;
				}
			}
			#endif
	}

	CO_memoryUsed = sizeof(CO_CANmodule_t)
	+ sizeof(CO_CANrx_t) * CO_RXCAN_NO_MSGS
	+ sizeof(CO_CANtx_t) * CO_TXCAN_NO_MSGS
	+ sizeof(CO_SDO_t) * CO_NO_SDO_SERVER
	+ sizeof(CO_OD_extension_t) * CO_OD_NoOfElements
	+ sizeof(CO_EM_t)
	+ sizeof(CO_EMpr_t)
	+ sizeof(CO_NMT_t)
	+ sizeof(CO_SYNC_t)
	+ sizeof(CO_RPDO_t) * CO_NO_RPDO
	+ sizeof(CO_TPDO_t) * CO_NO_TPDO
	+ sizeof(CO_HBconsumer_t)
	+ sizeof(CO_HBconsNode_t) * CO_NO_HB_CONS
	#if CO_NO_SDO_CLIENT == 1
	+ sizeof(CO_SDOclient_t)
	#endif
	+ 0;
	#if CO_NO_TRACE > 0
		CO_memoryUsed += sizeof(CO_trace_t) * CO_NO_TRACE;
		for(i=0; i<CO_NO_TRACE; i++) {
			CO_memoryUsed += CO_traceBufferSize[i] * 8;
		}
	#endif

	errCnt = 0;
	if(CO->CANmodule[0]                 == NULL) errCnt++;
	if(CO_CANmodule_rxArray0            == NULL) errCnt++;
	if(CO_CANmodule_txArray0            == NULL) errCnt++;
	for(i=0; i<CO_NO_SDO_SERVER; i++){
		if(CO->SDO[i]                   == NULL) errCnt++;
	}
	if(CO_SDO_ODExtensions              == NULL) errCnt++;
	if(CO->em                           == NULL) errCnt++;
	if(CO->emPr                         == NULL) errCnt++;
	if(CO->NMT                          == NULL) errCnt++;
	if(CO->SYNC                         == NULL) errCnt++;
	for(i=0; i<CO_NO_RPDO; i++){
		if(CO->RPDO[i]                  == NULL) errCnt++;
	}
	for(i=0; i<CO_NO_TPDO; i++){
		if(CO->TPDO[i]                  == NULL) errCnt++;
	}
	if(CO->HBcons                       == NULL) errCnt++;
	if(CO_HBcons_monitoredNodes         == NULL) errCnt++;
	#if CO_NO_SDO_CLIENT == 1
		if(CO->SDOclient                    == NULL) errCnt++;
	#endif
	#if CO_NO_TRACE > 0
		for(i=0; i<CO_NO_TRACE; i++) {
			if(CO->trace[i]                 == NULL) errCnt++;
		}
	#endif

	if(errCnt != 0) return CO_ERROR_OUT_OF_MEMORY;
	#endif


	CO->CANmodule[0]->CANnormal = false;
	CO_CANsetConfigurationMode(coCanDescr);

	/* Verify CANopen Node-ID */
	if(nodeId<1 || nodeId>127)
	{
		CO_delete();
		return CO_ERROR_PARAMETERS;
	}


	err = CANmodule_init(
	CO->CANmodule[0],
	coCanDescr,
	CO_CANmodule_rxArray0,
	CO_RXCAN_NO_MSGS,
	CO_CANmodule_txArray0,
	CO_TXCAN_NO_MSGS,
	bitRate);

	if(err){CO_delete(); return err;}

	for (i=0; i<CO_NO_SDO_SERVER; i++)
	{
		uint32_t COB_IDClientToServer;
		uint32_t COB_IDServerToClient;
		if(i==0){
			/*Default SDO server must be located at first index*/
			COB_IDClientToServer = CO_CAN_ID_RSDO + nodeId;
			COB_IDServerToClient = CO_CAN_ID_TSDO + nodeId;
			}else{
			COB_IDClientToServer = OD_SDOServerParameter[i].COB_IDClientToServer;
			COB_IDServerToClient = OD_SDOServerParameter[i].COB_IDServerToClient;
		}

		err = CO_SDO_init(
			CO->SDO[i],
			COB_IDClientToServer,
			COB_IDServerToClient,
			OD_H1200_SDO_SERVER_PARAM+i,
			i==0 ? 0 : CO->SDO[0],
			&CO_ObjDict[0],
			CO_OD_NoOfElements,
			CO_SDO_ODExtensions,
			nodeId,
			CO->CANmodule[0],
			CO_RXCAN_SDO_SRV+i,
			CO->CANmodule[0],
			CO_TXCAN_SDO_SRV+i);
	}

	if(err){CO_delete(); return err;}


	err = EM_Init(
			CO->em,
			CO->emPr,
			CO->SDO[0],
			&OD_errorStatusBits[0],
			ODL_errorStatusBits_stringLength,
			&OD_errorRegister,
			&OD_preDefinedErrorField[0],
			ODL_preDefinedErrorField_arrayLength,
			CO->CANmodule[0],
			CO_TXCAN_EMERG,
			CO_CAN_ID_EMERGENCY + (uint32_t)nodeId);

	if(err){CO_delete(); return err;}


	err = NMT_Init(
	CO->NMT,
	CO->emPr,
	nodeId,
	500,
	CO->CANmodule[0],
	CO_RXCAN_NMT,
	CO_CAN_ID_NMT_SERVICE,
	CO->CANmodule[0],
	CO_TXCAN_HB,
	CO_CAN_ID_HEARTBEAT + nodeId);

	if(err){CO_delete(); return err;}


	if (nodeId==1)
	{
			NMTM_txBuff = CAN_Tx_BufferInit(/* return pointer to 8-byte CAN data buffer, which should be populated */
				
			CO_TXCAN_NMT,     /* index of specific buffer inside CAN module */
			0x0000,           /* CAN identifier */
			0,                /* rtr */
			2,                /* number of data bytes */
			0);
				

	}else{

	}
       
	err = SYNC_Init(
	CO->SYNC,
	CO->em,
	CO->SDO[0],
	&CO->NMT->operatingState,
	OD_COB_ID_SYNCMessage,
	OD_communicationCyclePeriod,
	OD_synchronousCounterOverflowValue,
	CO->CANmodule[0],
	CO_RXCAN_SYNC,
	CO->CANmodule[0],
	CO_TXCAN_SYNC);

	if(err){CO_delete(); return err;}





	for(i=0; i<CO_NO_RPDO; i++){
		CO_CANmodule_t *CANdevRx = CO->CANmodule[0];
		CANdevRxIdx = CO_RXCAN_RPDO + i;

		err = CO_RPDO_init(
		CO->RPDO[i],
		CO->em,
		CO->SDO[0],
		CO->SYNC,
		&CO->NMT->operatingState,
		nodeId,
		((i<4) ? (CO_CAN_ID_RPDO_1+i*0x100) : 0),
		0,
		(CO_RPDOCommPar_t*) &OD_RPDOCommunicationParameter[i],
		(CO_RPDOMapPar_t*) &OD_RPDOMappingParameter[i],
		OD_H1400_RXPDO_1_PARAM+i,
		OD_H1600_RXPDO_1_MAPPING+i,
		CANdevRx,
		CANdevRxIdx);

		if(err){CO_delete(); return err;}
	}


	for(i=0; i<CO_NO_TPDO; i++){
		err = CO_TPDO_init(
		CO->TPDO[i],
		CO->em,
		CO->SDO[0],
		&CO->NMT->operatingState,
		nodeId,
		((i<4) ? (CO_CAN_ID_TPDO_1+i*0x100) : 0),
		0,
		(CO_TPDOCommPar_t*) &OD_TPDOCommunicationParameter[i],
		(CO_TPDOMapPar_t*) &OD_TPDOMappingParameter[i],
		OD_H1800_TXPDO_1_PARAM+i,
		OD_H1A00_TXPDO_1_MAPPING+i,
		CO->CANmodule[0],
		CO_TXCAN_TPDO+i);
		if(err){CO_delete(); return err;}
	}

	HB_SetNode(coNode);
	err = CO_HBconsumer_init(
	CO->HBcons,
	CO->em,
	CO->SDO[0],
	&OD_consumerHeartbeatTime[0],
	CO_HBcons_monitoredNodes,
	CO_NO_HB_CONS,
	CO->CANmodule[0],
	CO_RXCAN_CONS_HB);

	if(err){CO_delete(); return err;}


	#if CO_NO_SDO_CLIENT == 1
		err = CO_SDOclient_init(
		CO->SDOclient,
		CO->SDO[0],
		(CO_SDOclientPar_t*) &OD_SDOClientParameter[0],
		CO->CANmodule[0],
		CO_RXCAN_SDO_CLI,
		CO->CANmodule[0],
		CO_TXCAN_SDO_CLI);

		if(err){CO_delete(); return err;}
	#endif


	#if CO_NO_TRACE > 0
	for(i=0; i<CO_NO_TRACE; i++) {
		CO_trace_init(
		CO->trace[i],
		CO->SDO[0],
		OD_traceConfig[i].axisNo,
		CO_traceTimeBuffers[i],
		CO_traceValueBuffers[i],
		CO_traceBufferSize[i],
		&OD_traceConfig[i].map,
		&OD_traceConfig[i].format,
		&OD_traceConfig[i].trigger,
		&OD_traceConfig[i].threshold,
		&OD_trace[i].value,
		&OD_trace[i].min,
		&OD_trace[i].max,
		&OD_trace[i].triggerTime,
		OD_INDEX_TRACE_CONFIG + i,
		OD_INDEX_TRACE + i);
	}
	#endif

	ptrCO=CO;
	return CO_ERROR_NO;
}


/******************************************************************************/
void CO_Class::CO_delete(void){
	#ifndef CO_USE_GLOBALS
	uint32_t i;
	#endif

	CO_CANsetConfigurationMode(coCanDescr);
	CANmodule_Disable();

	#ifndef CO_USE_GLOBALS
		#if CO_NO_TRACE > 0
			for(i=0; i<CO_NO_TRACE; i++) {
				free(CO->trace[i]);
				free(CO_traceTimeBuffers[i]);
				free(CO_traceValueBuffers[i]);
			}
		#endif
		#if CO_NO_SDO_CLIENT == 1
		free(CO->SDOclient);
		#endif
		free(CO_HBcons_monitoredNodes);
		free(CO->HBcons);
		for(i=0; i<CO_NO_RPDO; i++){
			free(CO->RPDO[i]);
		}
		for(i=0; i<CO_NO_TPDO; i++){
			free(CO->TPDO[i]);
		}
		free(CO->SYNC);
		free(CO->NMT);
		free(CO->emPr);
		free(CO->em);
		free(CO_SDO_ODExtensions);
		for(i=0; i<CO_NO_SDO_SERVER; i++){
			free(CO->SDO[i]);
		}
		free(CO_CANmodule_txArray0);
		free(CO_CANmodule_rxArray0);
		free(CO->CANmodule[0]);
		CO = NULL;
	#endif
}


/******************************************************************************/
CO_NMT_reset_cmd_t CO_Class::Process(uint32_t                timeDifference_ms,
uint32_t               *timerNext_ms)
{
	uint8_t i;
	bool_t NMTisPreOrOperational = false;
	CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
	ms50 = 0;
	if(CO->NMT->operatingState == CO_NMT_PRE_OPERATIONAL || CO->NMT->operatingState == CO_NMT_OPERATIONAL){
		
		NMTisPreOrOperational = true;
	}

	



	for(i=0; i<CO_NO_SDO_SERVER; i++){
		CO_SDO_process(
	
		NMTisPreOrOperational,
		timeDifference_ms,
		1000,
		timerNext_ms);
	}

	EM_process(
	CO->emPr,
	NMTisPreOrOperational,
	timeDifference_ms * 10,
	OD_inhibitTimeEMCY);


	reset = NMT_Process(
	timeDifference_ms,
	OD_producerHeartbeatTime,
	OD_NMTStartup,
	OD_errorRegister,
	OD_errorBehavior,
	timerNext_ms);

	CO_HBconsumer_process(
	CO->HBcons,
	NMTisPreOrOperational,
	timeDifference_ms);

	return reset;
}


/******************************************************************************/
bool_t CO_Class::CO_process_SYNC_RPDO(uint32_t                timeDifference_us)
{
	uint32_t i;
	bool_t syncWas = false;

	switch(CO_SYNC_process(CO->SYNC, timeDifference_us, OD_synchronousWindowLength)){
		case 1:     //immediately after the SYNC message
		syncWas = true;
		break;
		case 2:     //outside SYNC window
		CANClearPendingSyncPDOs();
		break;
	}

	for(i=0; i<CO_NO_RPDO; i++){
		CO_RPDO_process(CO->RPDO[i], syncWas);
	}

	return syncWas;
}


/******************************************************************************/
void CO_Class::CO_process_TPDO(bool_t                  syncWas,
uint32_t                timeDifference_us)
{
	uint32_t i;

	/* Verify PDO Change Of State and process PDOs */
	for(i=0; i<CO_NO_TPDO; i++){
		if(!CO->TPDO[i]->sendRequest) CO->TPDO[i]->sendRequest = CO_TPDOisCOS(CO->TPDO[i]);
		CO_TPDO_process(CO->TPDO[i], CO->SYNC, syncWas, timeDifference_us);
	}
}
