/* 
* ConfigState_Class.cpp
*
* Created: 7/3/2020 3:32:41 PM
* Author: GMateusDP
*/


#include "ConfigState_Class.h"
#include "MemoryFlash_Class.h"
#include "States_Class.h"
#include "CDC_Class.h"
#include "time.h"
#ifdef __DEBUG__
#ifdef AVR
const char gainStateDescription[][18] PROGMEM =
#else
const char* gainStateDescription[] =
#endif
{
	"Reset",
	"Initialize",
	"Resume",
	"StandbyOFF",
	"PrepareStandbyON",
	"StandbyON",
	"StandbyReady",
	"Delayed",
	"Interrupted",
	"ON",
	"StoppedProcess",
	"Cleanup",
	"Finished",
	"KeepHot",
	"SelfProtect",
	"Error",
	"Maintenance",
	"HMIUpdate",
	"FirmwareUpdate",
};
#endif
const	char*	printableNamesHVACState[]={
		
		"Current State",
		"Cycle Duration",
		"Remaining Time",
		"Target Power",
		"Now",
		"Magic"
		
	
};

const  HVACState defaultHVACState=
{
		.magic	= 0x61626364,
// 	tick_t		now;
// 	calendar_date_time	dateTime;
// 
// 	int			build;
// 	HVACStatus	hvacStatus;
// 
// 
// 
// 	uint32_t	currentState;
// 	uint32_t	callingState;			//State where system came from
// 	uint32_t	internalCateringState;
// 	uint32_t	stateFlags;
// 	tick_t	stateEntryTime;
// 
// 	//float	targetUDC;
// 
// 	//float	targetPower;
// 
// 	tick_t	inStateTime;
// 	tick_t	leaveInternalOperationStateTime;
// 
// 	tick_t	cycleDuration;
// 	tick_t	remainingTime;
// 	tick_t	cycleStepRemainingTime;
// 	tick_t	waitingExceededTime;
// 	tick_t	fansOnAfterCateringCycleTime;
// 	tick_t	lastPEModuleCheck;
// 	tick_t	resetSMPSInternalTemperatureOutOfRangeTime;
// 	tick_t	magnetronFanFailureDetectionTime;
// 	//tick_t	resetEmptyCavityTime;
// 	tick_t	resetInsufficientSMPSCoolingTime;
// 
// 
// 	bool	fanState:1;
// 
// 	
// 	bool	smpsTemperatureOutOfRangeEventSent:1;
// 
// 	bool	exhaustTemperatureOutOfRangeEventSent:1;
// 
// 	bool	magnetronTemperatureOutOfRangeEventSent:1;
// 
// 
// 
// 	bool	reedRelayOpen:1;
// 	bool	latchClosedConfirmPending:1;
// 
// 
// 	bool	alarmState:1;


} ;


// default constructor
ConfigState_Class::ConfigState_Class()
{
	defaultState.currentState = kHVACStateReset;
	defaultState.callingState = kHVACStateReset;
	defaultState.internalCateringState = 0;
	//defaultState.targetUDC = 0.0;
	defaultState.cycleDuration = 0;
	defaultState.inStateTime = 0;
	defaultState.remainingTime = 0;
	defaultState.waitingExceededTime = 0;
	defaultState.magic=0xCAFEBABE;
} //ConfigState_Class

// default destructor
ConfigState_Class::~ConfigState_Class()
{
} //~ConfigState_Class

void ConfigState_Class::SetFactoryDefaults(uint32_t subPartNumber, bool config, bool cycles)
{
	
	if (config)
	{
		memcpy(&configuration, &factoryDefaultsConfiguration, sizeof(configuration));
		configuration.commons.subPartNumber = subPartNumber;
	//	memory.writeConfigurationDataArea(configuration);
	}
	
	
}


uint32_t ConfigState_Class::SetInitialState(){
		uint32_t	r;
		r=memory.ReadApplicationState(lastHVACState);
// 		if (memory.IsStateDataValid(lastHVACState))
// 		{
// 				hvac.SetCurrentState(lastHVACState);
// 		} 
// 		else
// 		{	
// 			//r=memory.ReadFastApplicationState(lastHVACState);
// //			if (memory.IsFastStateDataValid(lastHVACState))
// // 			{
// // 				hvac.SetCurrentState(lastHVACState);
// // 				memory.SaveApplicationState(lastHVACState);
// // 			} 
// // 			else
// // 			{
// // 				hvac.SetDefaultState();
// // 				memory.WriteValidApplicationState(hvac.defaultState);
// // 			//	memory.WriteFastCurrentState(hvac.defaultState);
// // 			}
// // 					
// 		
// 		}

		return r;
	};
	
uint32_t	ConfigState_Class::UpdateStatusArincLabel(void){
	uint32_t s=0;
	AcknoledgeStatus as=hvac.hvacState->arincStatus;
	s=((uint32_t)as.statusArinc)|(((uint32_t)as.AC_CMD)<<2)|
	(((uint32_t)as.PLT_AUTO_FAN)<<3)|(((uint32_t)as.CPG_AUTO_FAN)<<4)
	//|(((uint32_t)0x1d)<<9)
	|(((uint32_t)as.statusFV)<<5)|(((uint32_t)as.DEFOG)<<7);
	lastAcknowledgeStatus=acknowledgeStatus;
	acknowledgeStatus=s;
	return s;
}	

uint32_t ConfigState_Class::GetAcknowledgeStatus(void){
	
	return acknowledgeStatus;
}
bool  ConfigState_Class::IsStatusArinclabelChanged(void){
	UpdateStatusArincLabel();
	return lastAcknowledgeStatus!=acknowledgeStatus;
}

	
uint32_t	ConfigState_Class::GetInitialStatus(HVACStatus& st){
	HVACStatus	is,hs;
	uint32_t	s;
	interfaces.GetStatus(is);
	hvac.GetStatus(hs);
	is=st;
	if (is.rawStatus==hs.rawStatus)
	{
		st=hs;
	} 
	else
	{
		st=is;
	}
	s=uint32_t(st.rawStatus);
	return	s;
}
void	ConfigState_Class::PrintState(void){
	HVACState	hs;
	bool	done=false;
	hvac.GetCurrentState(hs);
	uint32_t	nowTime=(uint32_t)hs.now;
	char *act=ctime((time_t *)&nowTime);
	k=sizeof(printableNamesHVACState)/(sizeof(char*));
	for (uint8_t ii = 0; ii < k; ii++)
	{
	
		usb<<printableNamesHVACState[ii]<<" : ";
	
		switch (ii)
		{
			case 0:
				usb<<(uint32_t)hs.currentState;
			/* Your code here */
			break;
			case 1:
				usb<<(uint32_t)hs.cycleDuration;
			/* Your code here */
			break;
			case 2:
				usb<<(uint32_t)hs.remainingTime;
			/* Your code here */
			break;
			case 3:
				//usb<<(uint32_t)hs.targetPower;
			/* Your code here */
			break;
			case 4:
				
				usb.print(act);
			/* Your code here */
			break;
			case 5:
				usb<<"0x";
				usb.print(hs.magic,HEX);
			
				/* Your code here */
			break;
			default:
				i=sizeof(printableNamesHVACState);
				
			/* Your code here */
			break;
		}
		usb<<NEWLINE;
			
	}
	
}
void	ConfigState_Class::SetDefaultState(void){
	hvac.SetCurrentState(defaultState);

}



