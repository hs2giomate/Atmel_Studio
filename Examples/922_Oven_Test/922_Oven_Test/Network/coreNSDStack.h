/*--------------------------------------------------------------------------

coreNSDStack.h

Interface
NSD core functionallity

Author: Steffen Simon

Copyright (c) 2010, 2013 Steffen Simon.
All rights reserved.

$Date: 2018-06-06 11:13:54 +0200 (Mi, 06. Jun 2018) $
$Revision: 7928 $

--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT

#ifndef CORE_NSD_STACK_H
#define CORE_NSD_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"
#include "coreCAN.h"
#include "coreNSDDiagnostics.h"

/**
 * \defgroup coreNSD Core NSD related definitions
*/
/*@{*/

/**	NSD message codes
	Refer to NSD part 2 issue 3 NSD_ATA2536_MN008-1
*/
#define	kMessageCode_STA_MGCU ((uint8)2) 			/* 00000010 */
#define	kMessageCode_STA_GAIN ((uint8)4) 			/* 00000100 */
#define	kMessageCode_PBR ((uint8)6) 				/* 00000110 */
#define	kMessageCode_PBM ((uint8)8) 				/* 00001000 */
#define	kMessageCode_DIAG_FAILURE ((uint8)10) 		/* 00001010 */
#define	kMessageCode_DIAG_MALFUNCTION ((uint8)11) 	/* 00001011 */
#define	kMessageCode_DIAG_WARNING ((uint8)12) 		/* 00001100 */
#define	kMessageCode_SES_MGCU ((uint8)24) 			/* 00011000 */
#define	kMessageCode_SES_GAIN ((uint8)25) 			/* 00011001 */
#define	kMessageCode_FLIGHT_INFO ((uint8)30) 		/* 00011110 */
#define	kMessageCode_GAIN_PT ((uint8)32) 			/* 00100000 */
#define	kMessageCode_DIAG_CMD ((uint8)34) 			/* 00100010 */

#ifdef CCOTF_DEMONSTRATOR
#define	kMessageCode_CAN_BRIDGE_REMOTE ((uint8)0x60)		/* 01100000 */
#define	kMessageCode_GAIN_CONNECT ((uint8)0x61) 			/* 01100001 */
#define	kMessageCode_GAIN_PROGRESS ((uint8)0x62) 			/* 01100010 */
#define	kMessageCode_GAIN_DIGITAL_SENSORS ((uint8)0x63) 	/* 01100011 */
#define	kMessageCode_GAIN_ANALOGUE_SENSORS ((uint8)0x64)	/* 01100100 */
#endif

#define	kMessageCode_PHSM ((uint8)126) 				/* 01111110 */
#define	kMessageCode_DT ((uint8)123) 				/* 01111011 */

#define	kMulticastAddress ((uint8)0x00)

/**	GAIN fallback type
	Refer to NSD part 2 issue 3 NSD_ATA2536_??????
*/
typedef enum
	{
	kType_Secondary = 0,
	kType_Primary = 1,	
	} GAINFallbackType;

/**	GAIN mode codes
	Refer to NSD part 2 issue 3 NSD_ATA2536_NM007-02
*/
typedef enum
	{
	kMode_Undefined = 0,
	kMode_Centralized = 1,	
	kMode_Decentralized = 2,
	kMode_Fallback = 3,
	kMode_Workshop = 4,
	} GAINMode;

/**	MGCU status codes
	Refer to NSD part 2 issue 4 NSD350-V-25-36-382-02
*/
typedef enum
	{
	kMGCUState_Undefined = 0,
	kMGCUState_Operational = 2,	
	kMGCUState_Maintenance = 3,
	kMGCUState_Error = 4,
	} MGCUState;

/**	GAIN status codes
	Refer to NSD part 2 issue 3 NSD_ATA2536_NM007-03
*/
typedef enum
	{
	kState_Undefined = 0,
	kState_Initialize = 1,	
	kState_StandbyON = 2,
	kState_ON = 3,
	kState_Waiting = 4,
	kState_Interrupted = 5,
	kState_Finished = 6,
	kState_Maintenance = 7,
	kState_Error = 8,
	kState_StandbyOFF = 9,
	kState_StoppedProcess = 10
	} GAINState;

/**	GAIN implemented functionalities
	Refer to NSD part 2 issue 3 NSD_ATA2536_NM007-03
*/
typedef enum
	{
	kGAIN_RemoteOperations = 1<<7,	
	kGAIN_CentralizedPowerControl = 1<<6,	
	kGAIN_DecentralizedPowerControl = 1<<5,
	kGAIN_Diagnostics = 1<<4,
	kGAIN_FallbackPowerMode = 1<<3,
	kGAIN_StorageFunction = 1<<2,
	kGAIN_DataTranfer = 1<<1
	} GAINImplementedFunctionalities;

/**	MGCU implemented functionalities
	Refer to NSD part 2 issue 3 NSD_ATA2536_NM007-03
*/
typedef enum
	{
	kMGCU_RemoteOperations = 1<<7,	
	kMGCU_CentralizedPowerControl = 1<<6,	
	kMGCU_Diagnostics = 1<<4,
	kMGCU_DataTranfer = 1<<1
	} MGCUImplementedFunctionalities;

typedef enum
	{
	kSTA_MGCU = 0,	
	kPBM,
	kSES_MGCU,
	kFLIGHT_INFO,
	kDIAG_CMD,
	kDT_REQ,
	kDT_DUL,
	kNumberOfMGCUMessages
	} MGCUMessage;

typedef enum
	{
	kSTA_GAIN = 0,	
	kPBR,
	kDIAG_FAILURE,
	kDIAG_MALFUNCTION,
	kDIAG_WARNING,
	kSES_GAIN,
	kGAIN_PT,
	kPHSM,
	kDT_RES,
	kDT_DDL,
	kNumberOfGAINMessages
	} GAINMessage;

extern	tick_t	GAINMessageInhibitTimes[kNumberOfGAINMessages];

#ifndef AVR
#pragma pack(2)
#endif

typedef struct
	{
	bool	isPresent;
	bool	centralizedPowerAvailable;
	bool	desinfectionMode;
	} MGCUStateBits;

typedef struct
	{
	tick_t			lastLifepulse;
	MGCUStateBits	bits;
	int8			timeoutCounter;
	} MGCUOperationalState;

/**	GAIN mode codes
	Refer to NSD part 2 issue 3 NSD_ATA2536_NM007-02
*/
typedef enum
	{
	kMGCUPresentChanged = 1,	
	kCentralizedPowerAvailableChanged,
	kDisinfectionModeChanged,
	kPinCodingError
	} NetworkStateChangeBits;

typedef enum
	{
	kStartSelfTest = 1,	
	kAbortSelfTest = 2
	} SelftestRequestCommands;

/**	Block 0: Last known good values pin coding
	Total: 32 Byte
*/
typedef struct
	{
	uint32	pinCode;
	uint16	partNumber;
	uint8	reserved[10];
	} GAINDataBlock00;

/**	Block 1: GAIN Standard
	Total: 7 Byte
*/
typedef struct
	{
	uint8	deviceStandard;
	char	standardVersion[6];
	uint8	allignment;	//	Allign to even memory address
	} GAINDataBlock01;

/**	Block 2: GAIN Functionality
	Total: 1 Byte
*/
typedef struct
	{
	uint8	GAINImplementedFunctionalities;
	uint8	allignment;	//	Allign to even memory address
	} GAINDataBlock02;

/**	Block 3: GAIN Power Thresholds
	Total: 4 Byte
*/
typedef struct
	{
	uint8	MaximumPowerBudget;		//	[VA/50]
	uint8	FallbackPowerThreshold;	//	[VA/50]	
	uint8	MinimumPowerBudget;		//	[VA/50]
	uint8	StandbyPowerThreshold;	//	[VA/5]
	} GAINDataBlock03;

/**	Block 4 GAIN Partial Power Capabilities
	Total: 15 Byte
*/
typedef struct
	{
	uint8	NumberOfPowerBudgetSteps;
	uint8	PowerBudgetSteps[14];	//	[VA*50]
	uint8	allignment;	//	Allign to even memory address
	} GAINDataBlock04;


typedef struct
	{
	uint16	build;
	uint8	major;
	uint8	minor;
	uint8	bugfix;
	uint8	stage;
	uint8	release;
	} softwareVersion;
	
/**	Block 5 GAIN Data
	Total: 48 Byte
*/
typedef struct
	{
	uint8	FallbackType;
	uint8	FeederID;
	uint8	DeviceID;
	uint8	DeviceClass;
	uint8	DeviceGroup;
	char	PartNumber[15];
	char	PartNumberAmendment;
	char	SerialNumber[12];
	char	HardwareVersion[7];
	char	SoftwareVersion[7];
	uint8	NumberOfClusters;
	} GAINDataBlock05;

/**	Block 6 Maintenance Data
	Total: 10 Byte
*/
typedef struct
	{
	uint8	LastMaintenanceCheckYear;
	uint8	LastMaintenanceCheckMonth;
	uint8	LastMaintenanceCheckDay;
	uint8	NextMaintenanceCheckYear;
	uint8	NextMaintenanceCheckMonth;
	uint8	NextMaintenanceCheckDay;
	uint16	OperationCycles;
	uint16	PowerOnTime;
	} GAINDataBlock06;

/**	Block 8 Flight Data
	Total: 30 Byte
*/
typedef struct
	{
	char	CityPairFrom[4];
	char	CityPairTo[4];
	char	FlightNumber[10];
	char	AircraftID[12];
	} GAINDataBlock08;

/**	Block 9 GAIN Timers
	Total: 28 Byte
*/
typedef struct
	{
	uint16	GlobalInhibitTimer;
	uint16	InhibitTimerDT;
	uint16	SequenceTimeoutTimer;
	uint16	TimerPHSM_GAIN;
	uint16	TimerPHSM_MGCU;
	uint16	reserved1;
	uint16	reserved2;
	uint16	TimerSTA_GAIN;
	uint16	TimeoutTimerSTA_GAIN;
	uint16	TimerSTA_MGCU;
	uint16	TimeoutTimerSTA_MGCU;
	uint16	TimerSES_MGCU;
	uint16	TimerSES_GAIN;
	uint16	TimeoutTimerSES;
	} GAINDataBlock09;

/**	Block 12 Counter Management
	Total: 6 Byte
*/
typedef struct
	{
	uint8	MGCUCounterDeltaInc;
	uint8	MGCUCounterDeltaDec;
	uint8	MGCUCounterThreshold;
	uint8	GAINCounterDeltaInc;
	uint8	GAINCounterDeltaDec;
	uint8	GAINCounterThreshold;
	} GAINDataBlock12;

/**	Configuration data Block Mapping to NVM
	Total: 188 Byte, including 8 byte header size + 180 byte payload
*/
typedef struct
	{
	//	8 byte header!
	uint16			magic;
	uint16			size;
	uint32			checksum;

	GAINDataBlock00	block00;
	GAINDataBlock01	block01;
	GAINDataBlock02	block02;
	GAINDataBlock03	block03;
	GAINDataBlock04	block04;
	GAINDataBlock05	block05;
	GAINDataBlock06	block06;
	GAINDataBlock08	block08;
	GAINDataBlock09	block09;
	GAINDataBlock12	block12;
	} GAINConfigurationData;

#ifndef AVR
#pragma pack()
#endif

extern uint16 GAINConfigurationDataBlockOffsetTable[];
extern uint16 GAINConfigurationDataBlockSizeTable[];
	
typedef tick_t (*getControllerTicksHandlerType)(void);
/**
	Callback for writing a message on the CAN bus
*/
typedef bool (*CANWriteMessageHandlerType)(CANMessage*);
/**
	Callback function for NVM access
*/
typedef void (*readGAINConfigurationDataHandlerType)(int16 adr, int16 n, void* destPtr);
typedef void (*writeGAINConfigurationDataHandlerType)(int16 adr, int16 n, const void* sourcePtr);
/**
	Callback function for network state changes
*/
typedef void (*networkStateChangedHandlerType)(NetworkStateChangeBits, uint8);
/**
	Callback function for power control state changes
*/
typedef void (*powerControlStateChangedHandlerType)(uint16 powerBudgetMax, uint16 time);

typedef void (*selfTestRequestHandlerType)(uint8 command);
/**
	Callback function for current time access
*/

typedef struct
	{
	getControllerTicksHandlerType			getControllerTicksCallback;
	CANWriteMessageHandlerType				CANWriteMessageCallback;
	readGAINConfigurationDataHandlerType	readGAINConfigurationDataCallback;
	writeGAINConfigurationDataHandlerType	writeGAINConfigurationDataCallback;
	networkStateChangedHandlerType			networkStateChangedCallback;
	powerControlStateChangedHandlerType		powerControlStateChangedCallback;
	selfTestRequestHandlerType				selfTestRequestCallback;
	} coreNSDConfigurationStruct;

/** Data blocks
*/
extern GAINConfigurationData	configurationData;

/** Initialize NSD stack
Coming soon:
\in nFilters number of possible receive filters
\out nFilters number of requested receive filters
\out filterArray[0]..filterArray[nFilters - 1]	array of CAN identifier filters
*/
bool	coreNSD_initialize(coreNSDConfigurationStruct*);

void	coreNSD_handlePeriodicTask(MGCUStateBits* mgcu);
void	coreNSD_sendNSDMessage(uint8 idx);

bool	coreNSD_refreshGAINConfigurationDataBlock(int16 block);
bool	coreNSD_readGAINConfigurationDataBlock(int16 block, void* src);
bool	coreNSD_copyGAINConfigurationDataBlock(int16 block, void* dest);
void*	coreNSD_getGAINConfigurationDataBlockAdress(int16 block);
bool	coreNSD_writeGAINConfigurationDataBlock(int16 block, const void* src);
bool	coreNSD_flushGAINConfigurationDataBlock(int16 block);

bool	coreNSD_readGAINConfigurationData(int16 offset, int16 size, void* dest);
bool	coreNSD_writeGAINConfigurationData(int16 offset, int16 size, const void* src);

/**	Helper functions
*/
tick_t	coreNSD_ticks(void);
uint16	coreNSD_convertToBigEndian16(uint16 x);
uint16	coreNSD_convertToSystemEndian16(uint16 x);
uint32	coreNSD_calculateCRC32(void *buffer, uint16 size);


/** Initialize NSD stack
*/
void	coreNSD_setGAINMessageIdentifier(CANMessage* message, GAINMessage msg);
void	coreNSD_setMGCUMessageIdentifier(CANMessage* message, MGCUMessage msg);

void	coreNSD_setCANMessage(CANMessage* message, tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data);
bool	coreNSD_checkAndSetCANMessage(uint8 deviceID, CANMessage* message, tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data);
/**	Valid CAN identifier?
*/
bool	coreNSD_isValidIdentifier(uint32 identifier, uint8 deviceID);

/**	Read accessors for STA_GAIN
*/
GAINMode	coreNSD_getGAINMode(const byte* message);
GAINState	coreNSD_getGAINState(const byte* message);
uint16		coreNSD_getGAINPB_Remaining_Time(const byte* message);
uint16		coreNSD_getGAINFallback_Power_Budget(const byte* message);
uint16		coreNSD_getGAINUsed_Power_Budget(const byte* message);
uint8		coreNSD_getGAINImpl_Functionalities(const byte* message);
bool		coreNSD_getGAINGAIN_Is_Primary(const byte* message);
uint8		coreNSD_getGAINFeeder_ID(const byte* message);
bool		coreNSD_getGAINMGCU_Is_Present(const byte* message);

/**	Write accessors for STA_GAIN
*/
void	coreNSD_setGAINMode(byte* message, GAINMode mode);
void	coreNSD_setGAINState(byte* message, GAINState state);
void	coreNSD_setGAINUsed_Power_Budget(byte* message, uint16 v);
void	coreNSD_setGAINFallback_Power_Budget(byte* message, uint16 v);
void	coreNSD_setGAINPB_Remaining_Time(byte* message, uint16 v);
void	coreNSD_setGAINImpl_Functionalities(byte* message, uint8 v);
void	coreNSD_setGAINGAIN_Is_Primary(byte* message, bool v);
void	coreNSD_setGAINFeeder_ID(byte* message, uint8 v);
void	coreNSD_setGAINMGCU_Is_Present(byte* message, bool v);

/**	Read accessors for GAIN_PT
*/
uint16 coreNSD_getGAINPTStandbyPowerThreshold(const byte* message);
uint16 coreNSD_getGAINPTMaximumPowerThreshold(const byte* message);
uint16 coreNSD_getGAINPTMinimumPowerThreshold(const byte* message);
uint16 coreNSD_getGAINPTFallbackPowerThreshold(const byte* message);

/**	Write accessors for GAIN_PT
*/
void	coreNSD_setGAINPTStandbyPowerThreshold(byte* message, uint16 v);
void	coreNSD_setGAINPTMaximumPowerThreshold(byte* message, uint16 v);
void	coreNSD_setGAINPTMinimumPowerThreshold(byte* message, uint16 v);
void	coreNSD_setGAINPTFallbackPowerThreshold(byte* message, uint16 v);

/**	Read accessors for PBR
*/
bool	getPBRPowerBudgetType(const byte* message);
uint16	coreNSD_getPBRNominalPowerBudget(const byte* message);
uint16	coreNSD_getPBRMinimalPowerBudget(const byte* message);
uint16	coreNSD_getPBRNeededTimeWithNominalPB(const byte* message);
uint16	coreNSD_getPBRNeededTimeWithMinimalPB(const byte* message);

/**	Write accessors for PBR
*/
void	coreNSD_setPBRPowerBudgetType(byte* message, bool v);
void	coreNSD_setPBRNominalPowerBudget(byte* message, uint16 v);
void	coreNSD_setPBRNeededTimeWithNominalPB(byte* message, uint16 v);
void	coreNSD_setPBRMinimalPowerBudget(byte* message, uint16 v);
void	coreNSD_setPBRNeededTimeWithMinimalPB(byte* message, uint16 v);

/**	Read accessors for STA_MGCU
*/
MGCUState	coreNSD_getMGCUState(const byte* message);
bool		coreNSD_getMGCUDisinfection_Mode(const byte* message);
uint8		coreNSD_getMGCUImplemented_Functionalities(const byte* message);
uint8		coreNSD_getMGCUPower_Info_Feeder(const byte* message);
bool		coreNSD_getMGCUCentral_Power_Valid_On_Feeder(const byte* message);

/**	Write accessors for STA_MGCU
*/
void	coreNSD_setMGCUState(byte* message, MGCUState v);
void	coreNSD_setMGCUDisinfection_Mode(byte* message, bool v);
void	coreNSD_setMGCUImplemented_Functionalities(byte* message, uint8 v);
void	coreNSD_setMGCUPower_Info_Feeder(byte* message, uint8 v);
void	coreNSD_setMGCUCentral_Power_Valid_On_Feeder(byte* message, bool v);

/**	Read accessors for PBM
*/
uint16	coreNSD_getPBMPowerBudgetMax(const byte* message);
uint16	coreNSD_getPBMTime(const byte* message);

/**	Write accessors for PBM
*/
void	coreNSD_setPBMPowerBudgetMax(byte* message, uint16 v);
void	coreNSD_setPBMTime(byte* message, uint16 v);

/**	Write accessors to payload data
*/
void	coreNSD_setCANMessagePayloadData(byte* message, uint8 mask, uint8 byteOffset, uint8 bitOffset, uint8 value);
void	coreNSD_setCANMessagePayloadDataBit(byte* message, uint8 byteOffset, uint8 bitOffset, bool value);

/**	Read accessors to payload data
*/
uint8	coreNSD_getCANMessagePayloadData(const byte* message, uint8 mask, uint8 byteOffset, uint8 bitOffset);
bool	coreNSD_getCANMessagePayloadDataBit(const byte* message, uint8 byteOffset, uint8 bitOffset);

/**	NSD read accessors to CAN identifier
*/
uint8	coreNSD_getLCC(uint32 identifier);
uint8	coreNSD_getSourceFID(uint32 identifier);
uint8	coreNSD_getRSD(uint32 identifier);
uint8	coreNSD_getSMT(uint32 identifier);
#define	coreNSD_getSMT(x)	coreNSD_getRSD(x)	
uint8	coreNSD_getLCL(uint32 identifier);
uint8	coreNSD_getPVT(uint32 identifier);
uint8	coreNSD_getMessageCode(uint32 identifier);
#define	coreNSD_getServerFID(x)	coreNSD_getMessageCode(x)	
uint8	coreNSD_getDeviceID(uint32 identifier);
#define	coreNSD_getSID(x)	coreNSD_getDeviceID(x)	
uint8	coreNSD_getRCI(uint32 identifier);

/**	NSD write accessors to CAN identifier
*/
void	coreNSD_setLCC(uint32* identifier, uint8 value);
void	coreNSD_setSourceFID(uint32* identifier, uint8 value);
void	coreNSD_setRSD(uint32* identifier, uint8 value);
void	coreNSD_setLCL(uint32* identifier, uint8 value);
void	coreNSD_setPVT(uint32* identifier, uint8 value);
void	coreNSD_setMessageCode(uint32* identifier, uint8 value);
void	coreNSD_setDeviceID(uint32* identifier, uint8 value);
void	coreNSD_setRCI(uint32* identifier, uint8 value);
/*@}*/

#ifdef __cplusplus
	}
#endif

#endif

#endif	//	NSD_SUPPORT
