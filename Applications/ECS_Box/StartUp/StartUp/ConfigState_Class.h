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
#include "FlapperValveDataStruct.h"

struct StatusBits
{
	bool	ventilationFan1:1;
	bool	ventilationFan2:1;
	bool	scanvengerFan1:1;
	bool	flapValve1:1;
	bool	flapValve2:1;
	bool	heater1:1;
	bool	heater2:1;
	bool	compressor:1;
	bool	condesator:1;
};

enum StatusHVACARINC
{
	OFF=0,
	VENT,
	AUTO,
	BACKUP,
};




typedef struct
{
	StatusBits	statusBits;
	uint32_t	rawStatus;
}HVACStatus;
struct AcknoledgeStatus
{
	StatusHVACARINC	statusArinc;
	bool	AC_CMD:1;
	bool	PLT_AUTO_FAN:1;
	bool	CPG_AUTO_FAN:1;
	StatusFlapperValve statusFV;
	bool	DEFOG:1;
		
};

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

	int			build;
	HVACStatus	hvacStatus;
	AcknoledgeStatus	arincStatus;


	uint32_t	lastState;
	uint32_t	currentState;
	uint32_t	callingState;			//State where system came from
	uint32_t	internalCateringState;
	uint32_t	stateFlags;
	tick_t	stateEntryTime;

	//float	targetUDC;

	//float	targetPower;

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


	bool	fanState:1;

	
	bool	smpsTemperatureOutOfRangeEventSent:1;

	bool	exhaustTemperatureOutOfRangeEventSent:1;

	bool	magnetronTemperatureOutOfRangeEventSent:1;



	bool	reedRelayOpen:1;
	bool	latchClosedConfirmPending:1;


	bool	alarmState:1;


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
	HVACState	lastHVACState,currentHVACState;
	uint8_t	i,j,k;
	uint32_t acknowledgeStatus,lastAcknowledgeStatus;
	

//functions
public:
	ConfigState_Class();
	~ConfigState_Class();
//	virtual	~ConfigState_Class();
		uint32_t		SetInitialState(void);
		virtual void	SetCurrentState(HVACState& ) = 0;
		void			SetFactoryDefaults(uint32_t subPartNumber, bool config, bool cycles);
		uint32_t		GetInitialStatus(HVACStatus& st);
		void			PrintState(void);
		void			SetDefaultState(void);
		uint32_t		ConvertStatusArincLabel(void);
		uint32_t	 GetAcknowledgeStatus(void);
		bool  IsStatusArinclabelChanged(void);
protected:
	
private:
	ConfigState_Class( const ConfigState_Class &c );
	ConfigState_Class& operator=( const ConfigState_Class &c );
	

}; //ConfigState_Class

extern const HVACState  defaultHVACState;

#endif //__CONFIGSTATE_CLASS_H__
