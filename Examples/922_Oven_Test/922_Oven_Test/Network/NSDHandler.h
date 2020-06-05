/*--------------------------------------------------------------------------

NSDHandler.h

Interface
NSD-Protokoll

Autor: Steffen Simon

Copyright (c) 2010, 2013 Steffen Simon.
All rights reserved.

$Date: 2018-06-21 17:20:22 +0200 (Do, 21. Jun 2018) $
$Revision: 7936 $

--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT

#ifndef NSD_HANDLER_H
#define NSD_HANDLER_H

#include "NSDMessage.h"

enum
	{
	kNSDEventClass = 0x2000
	};

enum
	{
	kNSDMGCUConnectionLost = 0x400,
	kNSDMGCUConnectionEstablished,
	kNSDCentralizedPowerLost,
	kNSDCentralizedPowerEstablished,
	kNSDDisinfectionModeStarted,
	kNSDDisinfectionModeFinished,
	kNSDPinProgrammingError,
	kNSDPowerBudgetChanged,
	kNSDSendStowageInfo,
	kNSDAbortPowerBudgetRequest,
	kNSDSelftestRequest,
	};

struct NSDDeviceState
	{
	bool	isAlive;
	int8	timeoutCounter;
	tick_t	lastLifepulse;
	uint16	usedPower;
	uint16	maximumPower;
	};
	
class NSDHandler
	{
	public:
		NSDHandler(void);


		void	initializeHandler(uint16 nvmBase);
		bool	connectionStateDetermined(void);

		uint16	setPinCode(uint32 pinCode);
		uint16	resume(uint32 pinCode, GAINMode mode, GAINState state, uint16 usedPower, tick_t timeRemaining);

		void	setGetControllerTicksHandler(getControllerTicksHandlerType);
		void	setCANWriteMessageHandler(CANWriteMessageHandlerType);
		void	setReadGAINConfigurationDataHandler(readGAINConfigurationDataHandlerType);
		void	setWriteGAINConfigurationDataHandler(writeGAINConfigurationDataHandlerType);
		void	setNetworkStateChangedHandler(networkStateChangedHandlerType);
		void	setPowerControlStateChangedHandler(powerControlStateChangedHandlerType);
		void	setSelfTestRequestHandler(selfTestRequestHandlerType);
		
		void	handlePeriodicTask(MGCUStateBits& mgcu);
		
		//	STA_GAIN related set methods
		void	setGAINMode(GAINMode v, bool sync);
		void	setGAINFallbackPowerBudget(uint16 v, bool sync);
		void	setGAINState(GAINState v, bool sync);
		void	setGAINPowerBudgetUsed(uint16 v, bool sync);
		void	setGAINRemainingTime(tick_t v, bool sync);
		void	syncGAINStatus(void);

		//	CCOTF_DEMONSTRATOR related set methods
	#ifdef CCOTF_DEMONSTRATOR
		void	setGAINProgress(int8 v, bool sync);
		void	syncGAINDigitalSensorStatus(uint32 bits);
		void	syncGAINAnalogueSensorStatus(float v[], uint8 nSensors);
	#endif
		//	PHSM_GAIN related set methods
		void	setPHSMValueNumberOfReceiveErrorsDetected(uint16 v);
		void	setPHSMValueNumberOfTransmitErrorsDetected(uint16 v);
		void	setPHSMValueNumberOfCANResets(uint16 v);
		void	setPHSMValueREC(uint8 v);
		void	setPHSMValueTEC(uint8 v);

		void	sendPBR(uint16 nominal, int16 nominalTime, uint16 minimal, int16 minimalTime);
		void	sendPBR(uint16 nominal, tick_t nominalTime, uint16 minimal, tick_t minimalTime);
		void	cancelPBR(void);
		
		/**
			DIAG
		*/
		void	sendDIAG(bool force);

		void	addDIAGCode(NSDDiagCode v);
		void	removeDIAGCode(NSDDiagCode v);

		void	clearDIAG(void);

		void	messageReceived(const CANMessage&);
	
		bool	MGCUIsPresent(void) const;
		bool	CentralizedPowerControlAvailable(void) const;

		GAINState	getGAINState(void) const;
	#ifdef NIT
		uint8	getGAINDeviceType(void) const;
	#endif
 		uint8	getGAINFeederID(void) const;
		uint8	getGAINDeviceID(void) const;
		bool	getGAINIsPrimary(void) const;
		bool	getGAINMaintenancePin(void) const;
		bool	getGAINParityPin(void) const;
		//	uint8	getGAINPinProgrammingErrorState(void) const;
		
		uint32	getACFlightInfoUTCValue(void) const;

	private:
	    void	sendNSDMessage(uint8);

	#if DEBUG_NSD > 0
		void	logNSDMessage(const NSDMessage&, bool direction);
	#endif

	    void	handleSTA_MGCU(const NSDMessage&);
	    void	handlePBM(const NSDMessage&);
	    void	handleFLIGHT_INFO(const NSDMessage&);
	    void	handleDIAG_CMD(const NSDMessage&);
	#ifdef CCOTF_DEMONSTRATOR
	    void	handleCAN_BRIDGE_REMOTE(const NSDMessage&);
	#endif
	    void	handleDataUploadRequest(const NSDMessage&);
	    void	handleDataUpload(const NSDMessage&);
	    void	handleDataDownloadRequest(const NSDMessage&);
	    void	handleDataDownload(void);

		getControllerTicksHandlerType			getControllerTicksCallback;
		CANWriteMessageHandlerType				CANWriteMessageCallback;
		readGAINConfigurationDataHandlerType	readGAINConfigurationDataCallback;
		writeGAINConfigurationDataHandlerType	writeGAINConfigurationDataCallback;
		networkStateChangedHandlerType			networkStateChangedCallback;
		powerControlStateChangedHandlerType		powerControlStateChangedCallback;
		selfTestRequestHandlerType				selfTestRequestCallback;

		MGCUOperationalState		mgcuState;

 		NSDMessage		messages[kNumberOfGAINMessages];	
		NSDMessage		lastMessage;

		uint32			flightInfoUTCValue;

		tick_t			NSDStartupTime;
	#ifdef NIT
		uint8			gainDeviceType;
	#endif
		uint8			gainFeederID;
		uint8			gainDeviceID;
		uint8			gainFallbackType;

		GAINMode		currentGAINMode;
		GAINState		currentGAINState;

		tick_t			sendNextPBRTime;

		uint16			InhibitTimerDT;
		uint16			SequenceTimeoutTimer;
		uint16			TimerSTA_GAIN;
		uint16			TimeoutTimerSTA_MGCU;
		uint16			TimerPHSM_GAIN;

    	/** Data Transfer
    	*/
		uint32	CRC;
		uint8	CRCBuffer[4];
		uint16	minimumMessageSeperationTime;
		uint16	minimumBlockSeperationTime;
		uint16	transferBlockSize;

		uint8*	dataTransferBuffer;
		int8	dataTransferState;
		
		tick_t	nextDataTransfer;
		tick_t	dataTransferAbortionTime;

		uint16	dataBlockNumber;
		uint16	dataBlockDataIndex;
		uint16	transferBlockDataIndex;
		//	uint16	dataBlockOffset;
		uint16	dataBlockSize;
		bool	isPowerControlCapable;
	};

inline void NSDHandler::setGetControllerTicksHandler(getControllerTicksHandlerType v)
	{
	getControllerTicksCallback = v;
	}

inline void NSDHandler::setCANWriteMessageHandler(CANWriteMessageHandlerType v)
	{
	CANWriteMessageCallback = v;
	}

inline void NSDHandler::setReadGAINConfigurationDataHandler(readGAINConfigurationDataHandlerType v)
	{
	readGAINConfigurationDataCallback = v;
	}

inline void NSDHandler::setWriteGAINConfigurationDataHandler(writeGAINConfigurationDataHandlerType v)
 	{
	writeGAINConfigurationDataCallback = v;
	}

inline void NSDHandler::setNetworkStateChangedHandler(networkStateChangedHandlerType v)
	{
	networkStateChangedCallback = v;
	}

inline void NSDHandler::setPowerControlStateChangedHandler(powerControlStateChangedHandlerType v)
 	{
	powerControlStateChangedCallback = v;
	}

inline void NSDHandler::setSelfTestRequestHandler(selfTestRequestHandlerType v)
 	{
	selfTestRequestCallback = v;
	}

inline bool NSDHandler::MGCUIsPresent(void) const
	{
	return 	mgcuState.bits.isPresent;
	}

inline bool NSDHandler::CentralizedPowerControlAvailable(void) const
	{
	return 	isPowerControlCapable && mgcuState.bits.isPresent && mgcuState.bits.centralizedPowerAvailable;
	}

inline GAINState NSDHandler::getGAINState(void) const
	{
	return 	currentGAINState;
	}
	
#ifdef NIT
inline uint8 NSDHandler::getGAINDeviceType(void) const
	{
	return 	gainDeviceType;
	}
#endif

inline uint8 NSDHandler::getGAINFeederID(void) const
	{
	return 	gainFeederID;
	}

inline uint8 NSDHandler::getGAINDeviceID(void) const
	{
	return 	gainDeviceID;
	}

inline bool NSDHandler::getGAINIsPrimary(void) const
	{
#ifdef OFFLINE
	return false;
#else
	return 	gainFallbackType == kType_Primary;
#endif
	}

inline bool NSDHandler::getGAINMaintenancePin(void) const
	{
	return 	((configurationData.block00.pinCode & (1 << 9)) != 0);
	}

inline bool NSDHandler::getGAINParityPin(void) const
	{
	return 	((configurationData.block00.pinCode & (1 << 10)) != 0);
	}

/*
inline uint8 NSDHandler::getGAINPinProgrammingErrorState(void) const
	{
	return 	pinProgrammingErrorState;
	}
*/
		
inline uint32 NSDHandler::getACFlightInfoUTCValue(void) const
	{
	return coreSystemTimerSeconds();
	}

extern NSDHandler	NSD;

#endif 	//	NSD_HANDLER_H

#endif	//	NSD_SUPPORT

