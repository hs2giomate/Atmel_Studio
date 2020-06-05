/*--------------------------------------------------------------------------

GAINClass.h

Interface
Ipeco P/N 922

Author: Steffen Simon

$Date: 2018-03-28 15:23:30 +0200 (Mi, 28. Mrz 2018) $
$Revision: 7884 $

--------------------------------------------------------------------------*/
#ifndef GAIN_CLASS_H
#define GAIN_CLASS_H

#include "eDeviceEvent.h"
#include "eDevice.h"
#include "ConfigurationData.h"
#include "ControlPanel.h"
#include "MaintenanceTool.h"
#include "GAINDefinitions.h"

#ifdef PINCODING_SUPPORT
#include "PinCoding.h"
#endif

/**
 * \defgroup 922 922 Steam oven/Convection oven
*/
/*@{*/
/**	Constants for kGAINEventClass event types
*/
enum
	{
	kGAINDoorOpened = 0x280,
	kGAINDoorClosed,
/*
	kGAINCabinetTemperatureOutOfRange,
	kGAINCabinetTemperatureInRange,
	kGAINCabinetTemperatureChanged,
	kGAINSteamerTemperatureOutOfRange,
	kGAINSteamerTemperatureInRange,
*/
	kGAINWaterLowPressure,
	kGAINWaterOverPressure,
	kGAINWaterPressureInRange,
	//	Neu:
	kGAINCabinetTemperatureEvent,
	kGAINSteamerTemperatureEvent,
	kGAINWaterPressureEvent,
	kGAINFanSpeedEvent,
	kGAINFanStateEvent,
	};

typedef enum
	{
	kStateStandby,
	kStateOperationDelay,
	kStatePreheat,
	kStateSteamStandby,
	kStateSteamON,
	kStatePrepareRefill,
	kStateRefill,
	kStateRelaxSteamer,
	kStatePrepareConvection,
	kStateConvectionStandby,
	kStateConvectionON,
	kStateRelax,
	} ovenOperatingState;

typedef struct
	{
	uint32	magic;
	tick_t	now;
	int		build;
#ifdef NSD_SUPPORT
	uint32	pincoding;
	uint16	powerBudgetRequested;
	uint16	powerBudgetGranted;
	int16	powerTimeRequested;
	int16	powerTimeGranted;
#endif
	uint32	selfProtectSourceMask;
	uint16	error;
	uint16	currentState;
	uint16	callingState;
	uint16	internalCateringState;
	uint32	stateFlags;
	tick_t	stateEntryTime;

	tick_t	cycleDuration;
	tick_t	totalHeatingTime;
	tick_t	remainingTime;
	tick_t	steamCycleDuration;
	tick_t	leaveStateTime;
	tick_t	valveOpenedTime;

	int16	cabinetTemperature;
	int16	steamerTemperature;
	int16	targetTemperature;

	bool	inCycle:1;
	bool	savedDoorState:1;
	bool	cavityTemperatureOutOfRange:1;
	bool	steamerTemperatureOutOfRange:1;
	bool	steamMode:1;
	bool	steamerOn:1;
	bool	insufficientWaterPressure:1;
	bool	overPressure:1;
	bool	heaterStateChanged:1;
	bool	forcedConvectionMode:1;
	bool	disinfectionOngoing:1;
	bool	maintenanceToolConnected:1;
	
	bool	ambientTemperatureInRangeEventPending:1;
	bool	controllerTemperatureInRangeEventPending:1;
	bool	cabinetTemperatureInRangeEventPending:1;
	bool	steamerTemperatureInRangeEventPending:1;
	bool	waterPressureInRangeEventPending:1;
	
	bool	fanSpeedInRangeEventPending:1;
	bool	fanStateValidEventPending:1;
#ifdef NSD_SUPPORT
	bool	centralizedPowerControl:1;
#endif
	bool	nextGenerationHMIAvailable:1;
	} GAINWorkingState;

/** \brief 922 flow control
*/
class GAINClass
	{
	public:
		GAINClass(void);

		void	run(void);

		void	setFactoryDefaults(uint16, bool config, bool cycles);
		void	setPersistentConfigurationDefaults(void);

		bool	syncConfigurationWithPanel(void);
		void	resyncWithCycleDescription(uint8 cycleID);
		bool	resyncWithPanel(uint8 state = kGAINStateReset);

		static ConfigurationData			configuration;
		static CycleDictionary				cycleDictionary;
		static PersistentConfigurationData	persistentConfiguration;	

	private:
		void	handleGAINEvent(event& theEvent);

		void	handleStateInitialize(uint16 flags);
		void	handleStateResume(uint16 flags);
		void	handleStateStandbyOFF(uint16 flags);
		void	handleStatePrepareStandbyON(uint16 flags);
		void	handleStateStandbyON(uint16 flags);
		void	handleStateStandbyReady(uint16 flags);
		void	handleStateDelayed(uint16 flags);
	#ifdef NSD_SUPPORT
		void	handleStateInterrupted(uint16 flags);
	#endif
		void	handleStateON(uint16 flags);
		void	handleStateFinished(uint16 flags);
		void	handleStateStoppedProcess(uint16 callingsState);
		void	handleStateSelfProtect(uint16 error);
		void	handleStateError(uint16 error);
		void	handleStateMaintenance(uint16 flags);

		void	handleStateHMIUpdate(uint16 flags);
		void	handleStateFirmwareUpdate(uint16 flags);

		bool	handleInStateEvent(event& e, tick_t t, bool& done);

		void	prepareStateChangeEvent(event& e, uint16 newState, uint16 data = 0);

		uint16	handlePowerOnSelftest(void);

	#if defined(PINCODING_SUPPORT) && defined(NO_PINCODING)
		void	initializePinCoding(bool write);
		void	updatePinCoding(bool reset);
	#endif

		bool	abortCateringCycle(void);

		void	setFanState(bool on);
		void	setValveState(bool on);
		void	setHeaterState(bool on, bool vaporizer);

		void	setSafetyRelayState(bool on);

		void	saveCurrentState(void);
			
		bool	stateShallResume(uint8 state) const;
		bool	stateIsCateringCycleState(uint8 state) const;

		friend void checkCommunication(void);
		friend void checkEvent(void);

		bool	GAINIsReady(void);
		uint16	GAINGetWaitingFlags(void);
		uint16	GAINGetHMIFlags(void);
		bool	getDoorState(void) const;
		bool	getSteamSensorState(void) const;
		int16	getAmbientTemperature(bool initial = false);
		int16	getCabinetTemperature(bool initial = false);
		int16	getSteamerTemperature(bool initial = false);
		float	getWaterPressure(bool initial = false);
		
		uint16	maximumPowerBudget;
		uint16	minimumPowerBudget;
		tick_t	nextFanStateUpdateTime;

        GAINWorkingState	state;
        ControlPanel		panel;
        MaintenanceTool		maintenance;
	#if defined(PINCODING_SUPPORT)
        PinCoding			coding;
	#endif
#ifdef OFFLINE
	float	temperature;
	float	steamerTemperature;
	tick_t	heaterOnTime;
	tick_t	steamerOnTime;
	uint16	heaterState;
#endif
		
		tick_t	steamCycleStartTime;
		tick_t	powerInterruptDuration;
		uint8	refillCounter;
		bool	resumeLastStateValid;
	};


extern GAINClass	GAIN;
/*@}*/

#endif

