/* 
* ConfigState_Class.cpp
*
* Created: 7/3/2020 3:32:41 PM
* Author: GMateusDP
*/


#include "ConfigState_Class.h"
#include "MemoryManagment_Class.h"
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
// default constructor
ConfigState_Class::ConfigState_Class()
{
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
uint32_t ConfigState_Class::SetDefaultState(HVACState& hs){
	uint32_t	r=memory.ReadApplicationState(hs);
	return	r;
	};
	
uint32_t ConfigState_Class::SetInitialState(HVACState& hs){
		uint32_t	r=memory.ReadApplicationState(hs);
		if (memory.IsStateDataValid(hs))
		{
			
		} 
		else
		{
			r=memory.ReadDeafultApplicationState(hs);
			
		}
	SetCurrentState(hs);
		return r;
	};
	
uint32_t	ConfigState_Class::GetInitialStatus(HVACStatus& st){
	HVACStatus	is,hs;
	uint32_t	s;
	communicator.GetStatus(is);
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
	



