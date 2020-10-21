/*--------------------------------------------------------------------------

GAINDefinitions.h

Interface

Common GAIN enum and type definitions

Author: Steffen Simon
--------------------------------------------------------------------------*/
#ifndef GAIN_DEFINITIONS_H
#define GAIN_DEFINITIONS_H

#ifdef __DEBUG__
#ifdef AVR
#include <avr/pgmspace.h>
#endif
#endif
#include "coreTypes.h"

#ifdef __DEBUG__
#ifdef AVR
extern const char gainStateDescription[][18] PROGMEM;
#else
extern const char* gainStateDescription[];
#endif
#endif
/*@{*/
/** Constants for GAIN internal states
*/
enum HVACStatesEnum
	{
	kHVACStateStart=300,
	kHVACStateReset,
	kHVACStateInitialize,
	kHVACStateResume,
	kHVACStateStandbyVENT,
	kHVACStateStandbyAUTO,
	kHVACStateStandbyOFF,
	kHVACStatePrepareStandbyOFF,
	kHVACStatePrepareStandbyON,
	kHVACStateStandbyON,
	kHVACStateStandbyReady,
	kGAINStateDelayed,
	kHVACStateInterrupted,
	kHVACStateON,
	kHVACStateStoppedProcess,
	kGAINStateCleanup,
	kHVACStateFinished,
	kGAINStateKeepHot,
	kHVACStateSelfProtect,
	kHVACStateError,
	kHVACStateMaintenance,		//!< This state allows Maintenance tool to update configuration values or to set them to default.
	kGAINStateHMIUpdate,
	kGAINStateFirmwareUpdate,
	kHVACStateLeaving,
	kGAINStateCount
	};
/*@}*/

/*@{*/
/** Constants for GAIN resume
*/
enum
	{
	kGAINResume200msecs = 1,
	kGAINResumePowerResumeFunction,
	kGAINResumeStopped
	};
/*@}*/

/*@{*/
/** Constants for GAIN interruption source/state flags
*/
enum
	{

	kDoorOpen = (1 << 0),
	kDoorLatchOpen = (1 << 1),
	kLatchClosedConfirmPending = (1 << 2),
	kEmptyCavity = (1 << 3),
    kEmptyCavityWarning = (1 << 4),
	kNoNetwork = (1 << 5),				//!< No network
	kDisinfectionOngoing = (1 << 6),	//!< Disinfection ongoing
	kDevelopmentToolConnected = (1 << 7),//!< Development tool connected
	kServiceToolConnected = (1 << 8),	//!< Service tool connected
	};
/*@}*/

/*@{*/
/** Constants for initial connect message
*/
enum
	{
	kConnectFlagsAlarmOn = (1 << 0),
	};

// PSC/APSC output pin definitions
#if PARTNUMBER == 916
	#define	kControlPanelPowerSupplyPin			kPortPin3
	#define	kPinCodingBoardPowerSupplyPin			kPortPin4
	#define	kPEMpowerSupplyPin			kPortPin5
	#define	kDoorLatchPowerSupplyPin			kPortPin6
	#define	kReserveSwitchPowerSupplyPin			kPortPin7
	#define	kRFIDPowerSupplyPin			kPortPin8
	#define	kMainRelayPin			kPortPin9
	#define	kFan1Pin			kPortPin10
	#define	kFan2Pin			kPortPin11
	#define	kFan3Pin			kPortPin12
	#define	kCavityLEDPin			kPortPin13
	#define	kI2CSparePowerSupplyPin			kPortPin14
	#define	kOnboardLEDPin					kPortPin15
	#define	kDoorSwitchPin					kPortPin26
#elif PARTNUMBER == 932
	#define	kControlPanelPowerSupplyPin			kPortPin3
	#define	kPinCodingBoardPowerSupplyPin			kPortPin4
	#define	kPEMpowerSupplyPin			kPortPin5
	#define	kDoorLatchPowerSupplyPin			kPortPin6
	#define	kReserveSwitchPowerSupplyPin			kPortPin7
	#define	kRFIDPowerSupplyPin			kPortPin8
	#define	kMainRelayPin			kPortPin9
	#define	kFan1Pin			kPortPin10
	#define	kFan2Pin			kPortPin11
	#define	kFan3Pin			kPortPin12
	#define	kCavityLEDPin			kPortPin13
	#define	kI2CSparePowerSupplyPin			kPortPin14
	#define	kOnboardLEDPin					kPortPin15
	#define	kDoorSwitchPin					kPortPin26
#elif PARTNUMBER == 917
	#define	kControlPanelPowerSupplyPin			kPortPin3
#elif PARTNUMBER == 920
	#define	WaterInletInputPin					kPortA, kPortPin0
	#define	JugLevel1InputPin					kPortA, kPortPin2
	#define	JugLevel2InputPin					kPortE, kPortPin4
	#define	BrewTrayInputPin					kPortA, kPortPin4

	#define	DiverterValveOutputPin				kPortA,	kPortPin3
	#define	HeaterOutputPin						kPortA,	kPortPin5
	#define	SafetyRelayOutputPin				kPortA, kPortPin7

	#define	k24VPowerSupplyPin					kPortB, kPortPin0

	#define	MicropumpTimerLongOutputPin			kPortC,	kPortPin1
	#define	ThreeWayValveTimerLongOutputPin		kPortC,	kPortPin2
	#define	DiverterValveTimerLongOutputPin		kPortC,	kPortPin3

	#define	MicropumpOutputPin					kPortE,	kPortPin2
	#define	ThreeWayValveOutputPin				kPortE,	kPortPin3

	#define	BrewHandleInputPin					kPortE, kPortPin6
	#define	JugPositionInputPin					kPortE, kPortPin7

	#define	kControlPanelPowerSupplyPin			kPortG, kPortPin3
	#define	AdditionalValveOutputPin			kPortG, kPortPin4

	
	#define	kOutletWaterTemperatureADInputPin	kPortPin0
	#define	kAmbientTemperatureADInputPin		kPortPin1
	#define	kInletWaterTemperatureADInputPin	kPortPin2
	#define	kHeaterTemperatureADInputPin		kPortPin4
	#define	kSupplyVoltageADInputPin			kPortPin5
	#define	kDrainTemperatureADInputPin			kPortPin6
	#define	kWaterPressureADInputPin			kPortPin7
#elif (PARTNUMBER == 921)||(PARTNUMBER == 928)
	#define	kControlPanelPowerSupplyPin			kPortG, kPortPin3

	#define	SafetyRelayOutputPin				kPortA, kPortPin7
	#define	Relay2OutputPin						kPortA, kPortPin6
	#define	Relay1OutputPin						kPortA,	kPortPin5

	#define	CirculationFanOutputPin				kPortB, kPortPin0

	#define	DoorSwitchInputPin					kPortE, kPortPin6
	#define	Fan3TachoSignalInputPin				kPortE, kPortPin5
	#define	AdditionalSenseInputPin				kPortE, kPortPin4
	#define	Fan2NotRunInputPin					kPortG, kPortPin2
	#define	Sense28VInputPin					kPortG, kPortPin0

	#define	IMC1InputPin						kPortE, kPortPin7
	#define	IMC2InputPin						kPortA, kPortPin0
	#define	IMC3InputPin						kPortA, kPortPin2
	#define	IMC4InputPin						kPortA, kPortPin4
#elif PARTNUMBER == 922
	#define	kControlPanelPowerSupplyPin			kPortG, kPortPin3

	#define	SafetyRelayOutputPin				kPortA, kPortPin7
	#define	SteamerOutputPin					kPortA, kPortPin6
	#define	HeaterOutputPin						kPortA,	kPortPin5
	#define	SolenoidValve2OutputPin				kPortA, kPortPin3

	#define	FanStartOutputPin					kPortB, kPortPin7
	#define	CirculationFanOutputPin				kPortB, kPortPin4
	#define	SolenoidValve2TimerLongOutputPin	kPortC, kPortPin3
	#define	SolenoidValve1TimerLongOutputPin	kPortC, kPortPin2
	#define	SolenoidValve1OutputPin				kPortE, kPortPin3
	#define	FanPSCOutputPin						kPortG, kPortPin4

	#define	DoorSwitchInputPin						kPortE, kPortPin6
	#define	FanSpeedTimerLongOutputPin				kPortC, kPortPin1
	#define	FanSpeedOutputPin						kPortE, kPortPin2
	#define	FanControllerOverTemperatureInputPin	kPortA, kPortPin4
	#define	FanMotorOverTemperatureInputPin			kPortE, kPortPin7
	#define	FanNotRunInputPin						kPortG, kPortPin2

	#define	FanTachoSignalInputPin					kPortPin4
#endif



/*
	Constants for Generic GAIN event class types
*/	
enum
	{
	kHVACSwitchStateEvent = 0x200,
	
	kGAINTimerEvent,
	kGAINResetEvent,
	kGAINUpdateFirmwareEvent,
	kGAINMaintenanceConnectionEstablishedEvent,
	kGAINMaintenanceConnectionLostEvent,
	kGAINPanelConnectionEstablishedEvent,
	kGAINPanelConnectionLostEvent,
	kGAINPhaseLossEvent,
	kGAINPhaseALossEvent,
	kGAINPhaseBLossEvent,
	kGAINPhaseCLossEvent,
	kGAINAmbientTemperatureOutOfRange,
	kGAINAmbientTemperatureInRange,
	kGAINAmbientTemperatureSensorLost,
	kGAINControllerTemperatureOutOfRange,
	kGAINControllerTemperatureInRange,

	};


enum
	{
	kGAINNotPaused = 0,
	kGAINPausedByButton = 1,
	kGAINPausedByDoor = 2
	};

/**
	Common error structure definitions for GAINs
*/
#ifndef AVR
#pragma pack(2)
#endif

/** \brief Error log entry
 */
typedef struct
	{
	//! Internal error code.
	//	First 4 MSB encode entry severity, following 12 bit encode error ID
	uint16		errorID;
	// Additional information
	//	First 4 MSB encode annotation class, following 12 bit encode annotation data.
	//	Annotation Class 0 is considered application internal.
	uint16		annotation;

	// Time of occurence: milliseconds since power up
	tick_t		timestamp;
	// Time of occurence: Cumulated Power up time in seconds
	uint32		totalUpTime;
	} errorLogEntry;	

/** \brief Internal error description
 */
typedef struct
	{
	//! Internal error code.
	//	First 4 MSB encode entry severity, following 12 bit encode error ID
	uint16		errorID;
	// Additional information
	//	First 4 MSB encode annotation class, following 12 bit encode annotation data
	//	Annotation Class 0 is considered application internal.
	uint16		annotation;
	} errorDescription;
	
#ifndef AVR
#pragma pack()
#endif

/**
	Common error codes GAIN
*/
enum
	{
	kHVACNoError = 0,
	kGAINGenericError,
	kHVACErrorIllegalStateTransition,
	kGAINErrorPinProgramming,

	kGAINErrorSafetyRelayFailure,
	kGAINErrorPinCodingBoardFailure,
	kGAINErrorHMIFailure,

	kGAINErrorPhaseLost,

	kGAINErrorControllerTemperatureOutOfRange,
	kGAINErrorAmbientTemperatureOutOfRange,
	kGAINErrorAmbientSensorFailure,
	kGAINCommonErrorCount,
	};

#endif

