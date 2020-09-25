/* 
* ConfigState_Class.cpp
*
* Created: 7/3/2020 3:32:41 PM
* Author: GMateusDP
*/


#include "ConfigState_Class.h"
#include "MemoryManagment_Class.h"
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


// default constructor
ConfigState_Class::ConfigState_Class()
{
	defaultState.currentState = kGAINStateReset;
	defaultState.callingState = kGAINStateReset;
	defaultState.internalCateringState = 0;
	defaultState.targetUDC = 0.0;
	defaultState.cycleDuration = 0;
	defaultState.inStateTime = 0;
	defaultState.remainingTime = 0;
	defaultState.waitingExceededTime = 0;
	defaultState.fansOnAfterCateringCycleTime = forever;
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
		memory.writeConfigurationDataArea(configuration);
	}
	
	
}


uint32_t ConfigState_Class::SetInitialState(){
		uint32_t	r;
		r=memory.ReadApplicationState(lastState);
		if (memory.IsStateDataValid(lastState))
		{
				hvac.SetCurrentState(lastState);
		} 
		else
		{	
			r=memory.ReadFastApplicationState(lastState);
			if (memory.IsFastStateDataValid(lastState))
			{
				hvac.SetCurrentState(lastState);
				memory.SaveApplicationState(lastState);
			} 
			else
			{
				hvac.SetDefaultState();
				memory.WriteValidApplicationState(hvac.defaultState);
				memory.WriteFastCurrentState(hvac.defaultState);
			}
					
		
		}

		return r;
	};
	
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
	for (i = 0; i < k; i++)
	{
	
		usb<<printableNamesHVACState[i]<<" : ";
	
		switch (i)
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
				usb<<(uint32_t)hs.targetPower;
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



