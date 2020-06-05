/*--------------------------------------------------------------------------

NSDMessage.h

Interface
NSD Message Object

Autor: Steffen Simon

$Date: 2017-09-11 15:33:39 +0200 (Mo, 11. Sep 2017) $
$Revision: 7534 $

--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT

#ifndef NSD_MESSAGE_H
#define NSD_MESSAGE_H

#include "coreNSDStack.h"

class NSDMessage
	{
	public:
		NSDMessage(void);
		
		void	setCANMessage(const CANMessage& message);
		bool	checkAndSetCANMessage(uint8 deviceID, const CANMessage& message);
		void	setCANMessage(tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data);
		bool	checkAndSetCANMessage(uint8 deviceID, tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data);

		void	setMessageIdentifier(GAINMessage msg);
		void	setMessageIdentifier(MGCUMessage msg);
		void	setPayloadData(uint8 mask, uint8 byteOffset, uint8 bitOffset, uint8 value);
		void	setPayloadDataBit(uint8 byteOffset, uint8 bitOffset, bool value);

		/**	Read accessors for STA_GAIN
		*/
		GAINMode	getGAINMode(void) const;
		GAINState	getGAINState(void) const;
		uint16		getGAINPB_Remaining_Time(void) const;
		uint16		getGAINUsed_Power_Budget(void) const;
		uint16		getGAINFallback_Power_Budget(void) const;
		uint8		getGAINImpl_Functionalities(void) const;
		bool		getGAINGAIN_Is_Primary(void) const;
		uint8		getGAINFeeder_ID(void) const;
		bool		getGAINMGCU_Is_Present(void) const;

		/**	Write accessors for STA_GAIN
		*/
		void	setGAINMode(GAINMode mode);
		void	setGAINState(GAINState state);
		void	setGAINUsed_Power_Budget(uint16 v);
		void	setGAINFallback_Power_Budget(uint16 v);
		void	setGAINPB_Remaining_Time(uint16 v);
		void	setGAINImpl_Functionalities(uint8 v);
		void	setGAINGAIN_Is_Primary(bool v);
		void	setGAINFeeder_ID(uint8 v);
		void	setGAINMGCU_Is_Present(bool v);

		/**	Read accessors for GAIN_PT
		*/
		uint16	getGAINPTStandbyPowerThreshold(void) const;
		uint16	getGAINPTMaximumPowerThreshold(void) const;
		uint16	getGAINPTMinimumPowerThreshold(void) const;
		uint16	getGAINPTFallbackPowerThreshold(void) const;

		/**	Write accessors for GAIN_PT
		*/
		void	setGAINPTStandbyPowerThreshold(uint16 v);
		void	setGAINPTMaximumPowerThreshold(uint16 v);
		void	setGAINPTMinimumPowerThreshold(uint16 v);
		void	setGAINPTFallbackPowerThreshold(uint16 v);

		/**	Read accessors for PBR
		*/
		bool	getPBRPowerBudgetType(void) const;
		uint16	getPBRNominalPowerBudget(void) const;
		uint16	getPBRMinimalPowerBudget(void) const;
		uint16	getPBRNeededTimeWithNominalPB(void) const;
		uint16	getPBRNeededTimeWithMinimalPB(void) const;
		
		/**	Write accessors for PBR
		*/
		void	setPBRPowerBudgetType(bool v);
		void	setPBRNominalPowerBudget(uint16 v);
		void	setPBRMinimalPowerBudget(uint16 v);
		void	setPBRNeededTimeWithNominalPB(uint16 v);
		void	setPBRNeededTimeWithMinimalPB(uint16 v);

		/**	Read accessors for STA_MGCU
		*/
		MGCUState	getMGCUState(void) const;
		bool		getMGCUDisinfection_Mode(void) const;
		uint8		getMGCUImplemented_Functionalities(void) const;
		uint8		getMGCUPower_Info_Feeder(void) const;
		bool		getMGCUCentral_Power_Valid_On_Feeder(void) const;

		/**	Write accessors for STA_MGCU
		*/
		void	setMGCUState(MGCUState v);
		void	setMGCUDisinfection_Mode(bool v);
		void	setMGCUImplemented_Functionalities(uint8 v);
		void	setMGCUPower_Info_Feeder(uint8 v);
		void	setMGCUCentral_Power_Valid_On_Feeder(bool v);

		/**	Read accessors for PBM
		*/
		uint16	getPBMPowerBudgetMax(void) const;
		uint16	getPBMTime(void) const;

		/**	Write accessors for PBM
		*/
		void	setPBMPowerBudgetMax(uint16 v);
		void	setPBMTime(uint16 v);

		/*	Write accessors for DIAG_WARNING, DIAG_MALFUNCTION, DIAG_FAILURE
		*/
		void	setDIAGMessageDirtyBit(bool);
		void	setWarningPendingBit(bool);
		void	setMalfunctionPendingBit(bool);
		void	setFailurePendingBit(bool);

		void	addDIAG_WarningBit(NSDWarningBit v);
		void	addDIAG_MalfunctionBit(NSDMalfunctionBit v);
		void	addDIAG_FailureBit(NSDFailureBit v);

		bool	removeDIAG_WarningBit(NSDWarningBit v);
		bool	removeDIAG_MalfunctionBit(NSDMalfunctionBit v);
		bool	removeDIAG_FailureBit(NSDFailureBit v);

		void	clearDIAG(void);
		
		/*	Read accessors for DIAG_WARNING, DIAG_MALFUNCTION, DIAG_FAILURE
		*/
		bool	getDIAGMessageDirtyBit(void) const;
		bool	getWarningPendingBit(void) const;
		bool	getMalfunctionPendingBit(void) const;
		bool	getFailurePendingBit(void) const;
		/**	Read accessors to ARINC 825 identifier definition
		*/
		uint8	getLCC(void) const;
		uint8	getSourceFID(void) const;

		bool	getRSD(void) const;
		bool	getLCL(void) const;
		bool	getPVT(void) const;

		uint8	getMessageCode(void) const;
		uint8	getDeviceID(void) const;
		uint8	getRCI(void) const;

		/**	Write accessors to ARINC 825 identifier definition
		*/
		void	setMessageCode(uint8 v);
		void	setDeviceID(uint8 v);
		void	setRCI(uint8 v);

		bool	getSMT(void) const;
		uint8	getServerFID(void) const;
		uint8	getSID(void) const;

		/**	Read accessors to CAN message
		*/
		uint8	getDataLengthCode(void) const;
		byte*	getData(void);
	
		CANMessage	message;
	};
	
inline void NSDMessage::setCANMessage(const CANMessage& v)
	{
	coreNSD_setCANMessage(&message, v.timestamp, v.identifier, v.dataLengthCode, v.data);
	}
	
inline bool NSDMessage::checkAndSetCANMessage(uint8 deviceID, const CANMessage& v)
	{
	if (coreNSD_isValidIdentifier(v.identifier, deviceID))
		{
		message = v;
		return true;
		}
	else
		return false;
	}

inline void NSDMessage::setCANMessage(tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data)
	{
	coreNSD_setCANMessage(&message, timestamp, identifier, dataLengthCode, data);
	}
	
inline bool NSDMessage::checkAndSetCANMessage(uint8 deviceID, tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data)
	{
	return coreNSD_checkAndSetCANMessage(deviceID, &message, timestamp, identifier, dataLengthCode, data);
	}

inline void NSDMessage::setMessageIdentifier(GAINMessage msg)
	{
	coreNSD_setGAINMessageIdentifier(&message, msg);
	}

inline void NSDMessage::setMessageIdentifier(MGCUMessage msg)
	{
	coreNSD_setMGCUMessageIdentifier(&message, msg);
	}

inline void NSDMessage::setPayloadData(uint8 mask, uint8 byteOffset, uint8 bitOffset, uint8 value)
	{
	coreNSD_setCANMessagePayloadData(message.data, mask, byteOffset, bitOffset, value);
	}

inline void NSDMessage::setPayloadDataBit(uint8 byteOffset, uint8 bitOffset, bool value)
	{
	coreNSD_setCANMessagePayloadDataBit(message.data, byteOffset, bitOffset, value);
	}


/**	Read accessors for STA_GAIN
*/
inline GAINMode NSDMessage::getGAINMode(void) const
	{
	return coreNSD_getGAINMode(message.data);
	}

inline GAINState NSDMessage::getGAINState(void) const
	{
	return coreNSD_getGAINState(message.data);
	}

inline uint16 NSDMessage::getGAINPB_Remaining_Time(void) const
	{
	return coreNSD_getGAINPB_Remaining_Time(message.data);
	}

inline uint16 NSDMessage::getGAINUsed_Power_Budget(void) const
	{
	return coreNSD_getGAINUsed_Power_Budget(message.data);
	}

inline uint16 NSDMessage::getGAINFallback_Power_Budget(void) const
	{
	return coreNSD_getGAINFallback_Power_Budget(message.data);
	}

inline uint8 NSDMessage::getGAINImpl_Functionalities(void) const
	{
	return coreNSD_getGAINImpl_Functionalities(message.data);
	}

inline bool NSDMessage::getGAINGAIN_Is_Primary(void) const
	{
	return coreNSD_getGAINGAIN_Is_Primary(message.data);
	}

inline uint8 NSDMessage::getGAINFeeder_ID(void) const
	{
	return coreNSD_getGAINFeeder_ID(message.data);
	}

inline bool NSDMessage::getGAINMGCU_Is_Present(void) const
	{
	return coreNSD_getGAINMGCU_Is_Present(message.data);
	}

/**	Write accessors for STA_GAIN
*/
inline void NSDMessage::setGAINMode(GAINMode v)
	{
	coreNSD_setGAINMode(message.data, v);
	}

inline void NSDMessage::setGAINState(GAINState v)
	{
	coreNSD_setGAINState(message.data, v);
	}

inline void NSDMessage::setGAINUsed_Power_Budget(uint16 v)
	{
	coreNSD_setGAINUsed_Power_Budget(message.data, v);
	}

inline void NSDMessage::setGAINFallback_Power_Budget(uint16 v)
	{
	coreNSD_setGAINFallback_Power_Budget(message.data, v);
	}

inline void NSDMessage::setGAINPB_Remaining_Time(uint16 v)
	{
	coreNSD_setGAINPB_Remaining_Time(message.data, v);
	}

inline void NSDMessage::setGAINImpl_Functionalities(uint8 v)
	{
	coreNSD_setGAINImpl_Functionalities(message.data, v);
	}

inline void NSDMessage::setGAINGAIN_Is_Primary(bool v)
	{
	coreNSD_setGAINGAIN_Is_Primary(message.data, v);
	}

inline void NSDMessage::setGAINFeeder_ID(uint8 v)
	{
	coreNSD_setGAINFeeder_ID(message.data, v);
	}

inline void NSDMessage::setGAINMGCU_Is_Present(bool v)
	{
	coreNSD_setGAINMGCU_Is_Present(message.data, v);
	}

/**	Read accessors for GAIN_PT
*/
inline uint16 NSDMessage::getGAINPTStandbyPowerThreshold(void) const
	{
	return coreNSD_getGAINPTStandbyPowerThreshold(message.data);
	}

inline uint16 NSDMessage::getGAINPTMaximumPowerThreshold(void) const
	{
	return coreNSD_getGAINPTMaximumPowerThreshold(message.data);
	}

inline uint16 NSDMessage::getGAINPTMinimumPowerThreshold(void) const
	{
	return coreNSD_getGAINPTMinimumPowerThreshold(message.data);
	}

inline uint16 NSDMessage::getGAINPTFallbackPowerThreshold(void) const
	{
	return coreNSD_getGAINPTFallbackPowerThreshold(message.data);
	}

/**	Write accessors for GAIN_PT
*/
inline void NSDMessage::setGAINPTStandbyPowerThreshold(uint16 v)
	{
	coreNSD_setGAINPTStandbyPowerThreshold(message.data, v);
	}

inline void NSDMessage::setGAINPTMaximumPowerThreshold(uint16 v)
	{
	coreNSD_setGAINPTMaximumPowerThreshold(message.data, v);
	}

inline void NSDMessage::setGAINPTMinimumPowerThreshold(uint16 v)
	{
	coreNSD_setGAINPTMinimumPowerThreshold(message.data, v);
	}

inline void NSDMessage::setGAINPTFallbackPowerThreshold(uint16 v)
	{
	coreNSD_setGAINPTFallbackPowerThreshold(message.data, v);
	}


/**	Read accessors for PBR
*/
inline bool NSDMessage::getPBRPowerBudgetType(void) const
	{
	return coreNSD_getPBRNominalPowerBudget(message.data);
	}

inline uint16 NSDMessage::getPBRNominalPowerBudget(void) const
	{
	return coreNSD_getPBRNominalPowerBudget(message.data);
	}

inline uint16 NSDMessage::getPBRMinimalPowerBudget(void) const
	{
	return coreNSD_getPBRMinimalPowerBudget(message.data);
	}

inline uint16 NSDMessage::getPBRNeededTimeWithNominalPB(void) const
	{
	return coreNSD_getPBRNeededTimeWithNominalPB(message.data);
	}

inline uint16 NSDMessage::getPBRNeededTimeWithMinimalPB(void) const
	{
	return coreNSD_getPBRNeededTimeWithMinimalPB(message.data);
	}

/**	Write accessors for PBR
*/
inline void NSDMessage::setPBRPowerBudgetType(bool v)
	{
	coreNSD_setPBRPowerBudgetType(message.data, v);
	}

inline void NSDMessage::setPBRNominalPowerBudget(uint16 v)
	{
	coreNSD_setPBRNominalPowerBudget(message.data, v);
	}

inline void NSDMessage::setPBRMinimalPowerBudget(uint16 v)
	{
	coreNSD_setPBRMinimalPowerBudget(message.data, v);
	}

inline void NSDMessage::setPBRNeededTimeWithNominalPB(uint16 v)
	{
	coreNSD_setPBRNeededTimeWithNominalPB(message.data, v);
	}

inline void NSDMessage::setPBRNeededTimeWithMinimalPB(uint16 v)
	{
	coreNSD_setPBRNeededTimeWithMinimalPB(message.data, v);
	}


inline MGCUState NSDMessage::getMGCUState(void) const
	{
	return coreNSD_getMGCUState(message.data);
	}

inline bool NSDMessage::getMGCUDisinfection_Mode(void) const
	{
	return coreNSD_getMGCUDisinfection_Mode(message.data);
	}

inline uint8 NSDMessage::getMGCUImplemented_Functionalities(void) const
	{
	return coreNSD_getMGCUImplemented_Functionalities(message.data);
	}

inline uint8 NSDMessage::getMGCUPower_Info_Feeder(void) const
	{
	return coreNSD_getMGCUPower_Info_Feeder(message.data);
	}

inline bool NSDMessage::getMGCUCentral_Power_Valid_On_Feeder(void) const
	{
	return coreNSD_getMGCUCentral_Power_Valid_On_Feeder(message.data);
	}


/**	Write accessors for STA_MGCU
*/
inline void NSDMessage::setMGCUState(MGCUState v)
	{
	coreNSD_setMGCUState(message.data, v);
	}

inline void NSDMessage::setMGCUDisinfection_Mode(bool v)
	{
	coreNSD_setMGCUDisinfection_Mode(message.data, v);
	}

inline void NSDMessage::setMGCUImplemented_Functionalities(uint8 v)
	{
	coreNSD_setMGCUImplemented_Functionalities(message.data, v);
	}

inline void NSDMessage::setMGCUPower_Info_Feeder(uint8 v)
	{
	coreNSD_setMGCUPower_Info_Feeder(message.data, v);
	}

inline void NSDMessage::setMGCUCentral_Power_Valid_On_Feeder(bool v)
	{
	coreNSD_setMGCUCentral_Power_Valid_On_Feeder(message.data, v);
	}


/**	Read accessors for PBM
*/
inline uint16 NSDMessage::getPBMPowerBudgetMax(void) const
	{
	return coreNSD_getPBMPowerBudgetMax(message.data);
	}

inline uint16 NSDMessage::getPBMTime(void) const
	{
	return coreNSD_getPBMTime(message.data);
	}


/**	Write accessors for PBM
*/
inline void NSDMessage::setPBMPowerBudgetMax(uint16 v)
	{
	coreNSD_setPBMPowerBudgetMax(message.data, v);
	}

inline void NSDMessage::setPBMTime(uint16 v)
	{
	coreNSD_setPBMTime(message.data, v);
	}

inline void NSDMessage::setDIAGMessageDirtyBit(bool v)
	{
	if (v)
		message.data[0] |= (1 << 0);
	else
		message.data[0] &= ~(1 << 0);
	}

inline void NSDMessage::setWarningPendingBit(bool v)
	{
	if (v)
		message.data[0] |= (1 << 6);
	else
		message.data[0] &= ~(1 << 6);
	}

inline void NSDMessage::setMalfunctionPendingBit(bool v)
	{
	if (v)
		message.data[0] |= (1 << 5);
	else
		message.data[0] &= ~(1 << 5);
	}

inline void NSDMessage::setFailurePendingBit(bool v)
	{
	if (v)
		message.data[0] |= (1 << 4);
	else
		message.data[0] &= ~(1 << 4);
	}
		
inline bool NSDMessage::getDIAGMessageDirtyBit(void) const
	{
	return (message.data[0] & (1 << 0)) != 0;
	}

inline bool NSDMessage::getWarningPendingBit(void) const
	{
	return (message.data[0] & (1 << 6)) != 0;
	}

inline bool NSDMessage::getMalfunctionPendingBit(void) const
	{
	return (message.data[0] & (1 << 5)) != 0;
	}

inline bool NSDMessage::getFailurePendingBit(void) const
	{
	return (message.data[0] & (1 << 4)) != 0;
	}


inline uint8 NSDMessage::getLCC(void) const
	{
	return coreNSD_getLCC(message.identifier);
	}

inline uint8 NSDMessage::getSourceFID(void) const
	{
	return coreNSD_getSourceFID(message.identifier);
	}

inline bool NSDMessage::getRSD(void) const
	{
	return (coreNSD_getRSD(message.identifier) != 0);
	}

inline bool NSDMessage::getLCL(void) const
	{
	return (coreNSD_getLCL(message.identifier) != 0);
	}

inline bool NSDMessage::getPVT(void) const
	{
	return (coreNSD_getPVT(message.identifier) != 0);
	}

inline uint8 NSDMessage::getMessageCode(void) const
	{
	return coreNSD_getMessageCode(message.identifier);
	}

inline uint8 NSDMessage::getDeviceID(void) const
	{
	return coreNSD_getDeviceID(message.identifier);
	}

inline uint8 NSDMessage::getRCI(void) const
	{
	return coreNSD_getRCI(message.identifier);
	}

inline uint8 NSDMessage::getDataLengthCode(void) const
	{
	return message.dataLengthCode;
	}

inline byte* NSDMessage::getData(void)
	{
	return message.data;
	}



inline bool NSDMessage::getSMT(void) const
	{
	return (coreNSD_getSMT(message.identifier) != 0);
	}

inline uint8 NSDMessage::getServerFID(void) const
	{
	return coreNSD_getServerFID(message.identifier);
	}

inline uint8 NSDMessage::getSID(void) const
	{
	return coreNSD_getSID(message.identifier);
	}

#endif	//	NSD_MESSAGE_H

#endif	//	NSD_SUPPORT

