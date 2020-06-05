/*--------------------------------------------------------------------------

ControlPanel.h

Interface
Ansteuerung Controlpanel Protokollebene

Autor: Steffen Simon

$Date: 2018-06-26 09:17:31 +0200 (Di, 26. Jun 2018) $
$Revision: 7940 $

--------------------------------------------------------------------------*/
#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "GAINClass.h"

/** \brief Control panel related functions
 */
enum
	{
	kHMIEventClass = 0x1000
	};

/*@{*/
/** Constants for GAIN panel requests
*/
enum
	{
	kHMIRequestConnect,
	kHMIRequestStateChanged,
	kHMIRequestCateringStateChanged,
	kHMIRequestNextMaintenanceTimeChanged,
	kHMIRequestCateringProgress,
#if (PARTNUMBER == 916)
	kHMIRequestPowerLevel,
#elif (PARTNUMBER == 932)
	kHMIRequestCateringTime,
	kHMIRequestLRUCycles,
	kHMIRequestPowerLevel,
#else
	kHMIRequestTemperature,
#endif
	kHMIRequestModeDescription,
	kHMIRequestCateringMode,
	kHMIRequestSetDelayTime,
	kHMIRequestCount,
	kHMIRequestErrorCode = 0xB0,
	kHMIRequestNetworkState = 0xC0,
	kHMIRequestFirmwareUpdate = 0xCB
	};
/*@}*/

/*
	Constants for Panel event class types
*/	
#if ((PARTNUMBER == 916)||(PARTNUMBER == 932))
enum
	{
	kHMINoEvent = 0,
	kHMISendConfigurationEvent = 0x10,
	kHMISwitchStateEvent,
	kHMIButtonPressedEvent,
	kHMIStartCateringCycleEvent = 0x20,
	kHMIPauseCateringCycleEvent,
	kHMIResumeCateringCycleEvent,
	kHMIAbortCateringCycleEvent,
	kHMIChangeTimeEvent = 0x30,
	kHMIIncreaseTimeEvent,
	kHMIDecreaseTimeEvent,
	kHMIChangeDelayTimeEvent = 0x40,
	kHMIIncreaseDelayTimeEvent,
	kHMIDecreaseDelayTimeEvent,
	kHMIChangePowerEvent = 0x50,
	kHMIIncreasePowerEvent,
	kHMIDecreasePowerEvent,
	kHMIChangeTemperatureEvent = 0x60,
	kHMIIncreaseTemperatureEvent,
	kHMIDecreaseTemperatureEvent,
	kHMIChangePresetEvent = 0x70,
	kHMIChangeModeEvent,
	kHMIChangeAlarmStateEvent = 0x80,
	kHMINoFirmwareEvent = 0xFF
	};
#else
enum
	{
	kHMINoEvent = 0,
	kHMISendConfigurationEvent = 0x20,
	kHMISwitchStateEvent,
	kHMIButtonPressedEvent = 0x40,
#if (PARTNUMBER == 917)
	kHMISetHeatingProgramEvent,
	kHMIHeatingSummaryRequestEvent,
	kHMIStartCateringCycleEvent,
	kHMIPauseCateringCycleEvent,
	kHMIResumeCateringCycleEvent,
	kHMIAbortCateringCycleEvent,
	kHMISetLayerStateEvent,
	kHMIResetLayerEvent,
	kHMIStartKeepHotEvent,
#else
	kHMIStartCateringCycleEvent,
	kHMIPauseCateringCycleEvent,
	kHMIResumeCateringCycleEvent,
	kHMIAbortCateringCycleEvent,
	kHMIChangeTimeEvent,
#if (PARTNUMBER == 916)
	kHMIChangePowerEvent,
#else
	kHMIChangeTemperatureEvent,
#endif
	kHMIChangePresetEvent,
	kHMIChangeModeEvent,
	kHMIChangeDelayTimeEvent,
#endif
	kHMIEnterMaintenanceEvent = 0xA0,
	kHMIResetFactoryDefaultsEvent,

	kHMIChangeAlarmStateEvent = 0x80,
	kHMINoFirmwareEvent = 0xFF
	};
#endif

/** Constants for panel button events
*/
typedef enum
	{
	kEventNoButtonPressed = 0,
	kEventPowerButtonPressed,
#if (PARTNUMBER == 920)
	kEventBrewButtonPressed,
	kEventHotWaterButtonPressed,
#else
	kEventTimeDownButtonPressed,
	kEventTimeUpButtonPressed,
#endif
#if (PARTNUMBER == 916)
	kEventPowerUpButtonPressed,
	kEventPowerDownButtonPressed,
#elif (PARTNUMBER == 932)
	kEventPowerUpButtonPressed,
	kEventPowerDownButtonPressed,
#else
	kEventTemperatureUpButtonPressed,
	kEventTemperatureDownButtonPressed,
#endif
	kEventConfirmButtonPressed,
	kEventCancelButtonPressed,
	kEventEnterMaintenanceButtonsPressed
	} GAINButtonEvent;

typedef struct
	{
	uint8	command;
	uint8	target;
	uint8	size;
	uint8	crc;
	} panelRequestHeader;

class ControlPanel
	{
	public:
		ControlPanel(void);
				
		void	setPowerSupplyState(bool on);
		bool	getPowerSupplyState(void) const;
/*
	Coming soon!
*/
		bool	connect(void) const;
		bool	isConnected(void) const;
		bool	stateIsSynched(void) const;

	#if CP_VERSION > 0
		bool	syncWithGAINInfo(uint16 partNumber, uint16 subPartNumber, uint32 flags);
		bool	syncWithState(uint8 state, uint32 flags = 0);
	#else
		bool	syncWithGAINInfo(uint16 partNumber, uint16 subPartNumber);
		bool	syncWithState(uint8 state, uint16 flags = 0);
	#endif
		bool	syncWithCateringState(uint8 state, uint8 flags = 0);
		bool	syncWithNextMaintenanceTime(int16 time, uint8 flags = 0);
	
	#if CP_VERSION > 0
		bool	syncWithCycleDescription(uint8 id, const CycleDescription&, uint32 flags = 0);
		bool	syncWithCycleDescriptionID(uint8 id, uint32 flags = 0);
		bool	syncWithLRUCycles(LRUCycles&, uint32 flags = 0);
	#elif (PARTNUMBER == 916)||(PARTNUMBER == 920)||(PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
		bool	syncWithCycleDescription(uint8 id, const CycleDescription&);
		bool	syncWithCycleDescriptionID(uint8 id, uint8 flags = 0);
	#endif
	#if (PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
		bool	syncWithTemperature(int16 target, int16 current);
	#endif
	#if (PARTNUMBER == 916)||(PARTNUMBER == 932)||(PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
		bool	syncWithDelayTime(int16 delay, uint8 flags = 0);
	#endif
	#if (PARTNUMBER == 920)
		bool	syncWithProgress(uint8 progress, uint8 flags = 0);
	#endif
	#if CP_VERSION > 0
		bool	syncWithRemainingTime(int16 remaining, uint32 flags = 0);
		bool	syncWithRemainingTime(tick_t remaining, uint32 flags = 0);
		bool	syncWithCateringTime(tick_t cateringTime, tick_t maximumTime, uint32 flags = 0);
	#elif (PARTNUMBER == 922)||(PARTNUMBER == 916)
		bool	syncWithRemainingTime(int16 remaining, uint8 flags = 0);
		bool	syncWithRemainingTime(tick_t remaining, uint8 flags = 0);
	#endif
	#if (PARTNUMBER == 916)
		bool	syncWithPowerLevel(uint16 power, uint16 percentage);
	#elif (PARTNUMBER == 932)
		bool	syncWithPowerLevel(uint16 power, uint16 percentage);
	#endif
	#ifdef NSD_SUPPORT
		bool	syncWithNetworkState(void);
	#endif
		bool	syncWithFirmwareUpdate(void);

		bool	handlePeriodicTask(void);
		bool	checkEvents(void);
		bool	stateIsSynched(void);
		bool	update(bool address32);

	private:
		bool	sendRequest(const panelRequestHeader& header, const void* request, int8 delay);
		void	wait(int16);
		
	#if (PARTNUMBER == 920)
		int16	progressSave;
	#endif
	#if (PARTNUMBER == 932)
		int16	remainingSecondsSave;
	#elif (PARTNUMBER == 922)||(PARTNUMBER == 916)
		int16	remainingSecondsSave;
	#endif
		int16	lastMessageSize;
		uint16	savedGAINState;
		uint16	lastReceivedGAINState;
		tick_t	lastMessageSent;
		uint8	retryCounter;
		uint8	disconnectCounter;

		bool	panelHasFirmware:1;
		bool	panelIsConnected:1;
		bool	panelGAINStateIsSynched:1;
	};

inline bool ControlPanel::stateIsSynched(void) const
	{
	return panelGAINStateIsSynched;
	}

inline bool ControlPanel::isConnected(void) const
	{
	return panelIsConnected;
	}

#endif // CONTROL_PANEL_H

