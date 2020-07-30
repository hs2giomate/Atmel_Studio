/* 
* ConfigState_Class.h
*
* Created: 7/3/2020 3:32:41 PM
* Author: GMateusDP
*/


#ifndef __CONFIGSTATE_CLASS_H__
#define __CONFIGSTATE_CLASS_H__
#include "coreTypes.h"
#include "ConfigurationData.h"
#include "GAINDefinitions.h"
#include "hpl_calendar.h"

typedef struct
{
	struct{
		uint32_t	ventilationFan1:1;
		uint32_t	ventilationFan2:1;
		uint32_t	scanvengerFan1:1;
		uint32_t	flapValve1:1;
		uint32_t	flapValve2:1;
		uint32_t	heater1:1;
		uint32_t	heater2:1;
		uint32_t	compressor:1;
		uint32_t	condesator:1;
	
	}statusBits;
	uint32_t	rawStatus;
}HVACStatus;

typedef struct
	{
	uint32_t	power;
	uint32_t	percentage;
	} PowerLevel;
	
typedef enum
	{
	kStateOnStandby,			//	0
	kStateOnInitialSetUDCDelay,	//	1
	kStateOnInitialSetUDC,		//	2
	kStateOnSetUDCDelay,		//	3
	kStateOnSetUDC,				//	4
	kStateOnControlPDC,         //	5
	kStateOnDone,           	//	6
	} deviceOnOperatingState;

/*
	Constants for kGAINEventClass event types
*/
enum
	{
	kGAINDoorLatchOpened = 0x280,
	kGAINDoorLatchClosed,
	kGAINDoorOpened,
	kGAINDoorClosed,
	kGAINExhaustTemperatureOutOfRange,
	kGAINExhaustTemperatureInRange,
	kGAINMagnetronTemperatureOutOfRange,
	kGAINMagnetronTemperatureInRange,
	kGAINMagnetronFanFault,
	kGAINSMPSTemperatureOutOfRange,
	kGAINSMPSTemperatureInRange,
	};

#pragma pack (4)		
struct HVACState
	{
	uint32_t	magic;
	tick_t		now;
	calendar_date_time	dateTime;
	uint32_t	seconds;
	int			build;
	HVACStatus	status;

	uint32_t	selfProtectSourceMask;

	uint32_t	currentState;
	uint32_t	callingState;			//State where system came from
	uint32_t	internalCateringState;
	uint32_t	stateFlags;
	tick_t	stateEntryTime;

	float	targetUDC;
	float	targetUDCMax;
	float	targetPower;

	tick_t	inStateTime;
	tick_t	leaveInternalOperationStateTime;

	tick_t	cycleDuration;
	tick_t	remainingTime;
	tick_t	cycleStepRemainingTime;
	tick_t	waitingExceededTime;
	tick_t	fansOnAfterCateringCycleTime;
	tick_t	lastPEModuleCheck;
	tick_t	resetSMPSInternalTemperatureOutOfRangeTime;
	tick_t	magnetronFanFailureDetectionTime;
    //tick_t	resetEmptyCavityTime;
	tick_t	resetInsufficientSMPSCoolingTime;

	int32_t	errorPEM;
	uint32_t	errorCode;

	bool	fanState:1;

	bool	smpsTemperatureInRangeEventSent:1;
	bool	smpsTemperatureOutOfRangeEventSent:1;
	bool	exhaustTemperatureInRangeEventSent:1;
	bool	exhaustTemperatureOutOfRangeEventSent:1;
	bool	magnetronTemperatureInRangeEventSent:1;
	bool	magnetronTemperatureOutOfRangeEventSent:1;

	bool	maintenanceToolConnected:1;
	bool	doorOpenedAfterCateringCycle:1;
	bool	savedDoorLatchState:1;
	bool	reedRelayOpen:1;
	bool	latchClosedConfirmPending:1;
	bool	safetyRelayFaultDetected:1;

	bool	nextGenerationHMIAvailable:1;
	bool	alarmState:1;
    bool    selftestNeedsToBePerformed;    //!< Tells at bootup  in resume state whether a selftest was requested (over NSD)

	} ;

typedef struct  HVACState HVACState;
	

#pragma pack ()		

typedef		void	(*PTR_HVAC_STATE)(HVACState *);

class ConfigState_Class
{
//variables
public:
	HVACState defaultState;
protected:
private:
	ConfigurationData	configuration,defaultsConfiguration;
	HVACState	lastState,currentState;
	uint8_t	i,j,k;
	

//functions
public:
	ConfigState_Class();
	~ConfigState_Class();
		uint32_t		SetInitialState(void);
		virtual void	SetCurrentState(HVACState& ) = 0;
		void			SetFactoryDefaults(uint32_t subPartNumber, bool config, bool cycles);
		uint32_t		GetInitialStatus(HVACStatus& st);
		void			PrintState(void);
		void			SetDefaultState(void);
protected:
	
private:
	ConfigState_Class( const ConfigState_Class &c );
	ConfigState_Class& operator=( const ConfigState_Class &c );
	

}; //ConfigState_Class

#endif //__CONFIGSTATE_CLASS_H__
