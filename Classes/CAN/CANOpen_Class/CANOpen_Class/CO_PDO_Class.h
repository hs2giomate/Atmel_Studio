/* 
* CO_PDO_Class.h
*
* Created: 8/5/2020 2:51:06 PM
* Author: GMateusDP
*/


#ifndef __CO_PDO_CLASS_H__
#define __CO_PDO_CLASS_H__
#include "CO_Definitions.h"



class CO_PDO_Class
{
//variables
public:
protected:
private:

//functions
public:
	CO_PDO_Class();
	~CO_PDO_Class();

	CO_ReturnError_t CO_RPDO_init(
			CO_RPDO_t              *RPDO,
			CO_EM_t                *em,
			CO_SDO_t               *SDO,
			CO_SYNC_t              *SYNC,
			uint8_t                *operatingState,
			uint8_t                 nodeId,
			uint32_t                defaultCOB_ID,
			uint8_t                 restrictionFlags,
			const CO_RPDOCommPar_t *RPDOCommPar,
			const CO_RPDOMapPar_t  *RPDOMapPar,
			uint32_t                idx_RPDOCommPar,
			uint32_t                idx_RPDOMapPar,
			CO_CANmodule_t         *CANdevRx,
			uint32_t                CANdevRxIdx);



	CO_ReturnError_t CO_TPDO_init(
			CO_TPDO_t              *TPDO,
			CO_EM_t                *em,
			CO_SDO_t               *SDO,
			uint8_t                *operatingState,
			uint8_t                 nodeId,
			uint32_t                defaultCOB_ID,
			uint8_t                 restrictionFlags,
			const CO_TPDOCommPar_t *TPDOCommPar,
			const CO_TPDOMapPar_t  *TPDOMapPar,
			uint32_t                idx_TPDOCommPar,
			uint32_t                idx_TPDOMapPar,
			CO_CANmodule_t         *CANdevTx,
			uint32_t                CANdevTxIdx);
	void CO_RPDO_process(CO_RPDO_t *RPDO, bool_t syncWas);
	

void CO_TPDO_process(
        CO_TPDO_t              *TPDO,
        CO_SYNC_t              *SYNC,
        bool_t                  syncWas,
        uint32_t                timeDifference_us);
	uint8_t CO_TPDOisCOS(CO_TPDO_t *TPDO);
protected:
private:
	CO_PDO_Class( const CO_PDO_Class &c );
	CO_PDO_Class& operator=( const CO_PDO_Class &c );
	
	



uint32_t CO_TPDOsend(CO_TPDO_t *TPDO);





}; //CO_PDO_Class

#endif //__CO_PDO_CLASS_H__
