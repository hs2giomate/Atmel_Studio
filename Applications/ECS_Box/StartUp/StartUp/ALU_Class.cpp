/* 
* ALU_Class.cpp
*
* Created: 6/19/2020 4:21:20 PM
* Author: GMateusDP
*/


#include "ALU_Class.h"
#include "AMMC_Class.h"
#include "MemoryManagment_Class.h"
#include "CDC_Class.h"
#include "DateTime_Class.h"
#include "N25Q256_Class.h"


// default constructor
ALU_Class::ALU_Class()
{
	ptrPbit=&pBIT;
} //ALU_Class

// default destructor
ALU_Class::~ALU_Class()
{
} //~ALU_Class

uint32_t	ALU_Class::Init(void){
	uint32_t	s;
	StartLivePulse();
	cBit.statusBits.hvacOK=hvac.Init();
	hvac.SetCRCConfigData();
	SetInitialConfiguration(configuration);
	memory.WriteDefaultState();
	hvac.SetInitialState();
	uhr.Init();
	hvac.saveCurrentState();
	s=arinc.Init();
	if (s!=0x01)
	{	
		NotifyError(kARINCINnterfaceError,s);
		return s;
	} 
	else
	{
		s=pBIT.CheckCurrentStatus(status);
		if (s>0)
		{
			NotifyError(kpBITError,s);
			return s;
		} 
		else
		{
			
		}
	}
	  
	return s;
}

uint8_t	ALU_Class::GetSelectedAMMC(void){
	activeLine=arinc.GetActiveLine();
		switch (activeLine)
		{
			case 1:
			ammc=&ammc1;
			/* Your code here */
			break;
			
			case 2:
			ammc=&ammc2;
			/* Your code here */
			break;
			default:
			ammc=&ammc1;
			/* Your code here */
			break;
		}
	return activeLine;	
}

uint8_t	ALU_Class::Run(void){
	uint8_t line= GetSelectedAMMC();
	#ifdef __DEBUG__
		usb<<"Selected line:"<<line<<NEWLINE;
	#endif
	if (ptrPbit->GetStatus()>0)
	{
			#ifdef __DEBUG__
			usb<<"Selected line:"<<line<<NEWLINE;
			#endif
	} 
	else
	{
	}
	return line;
}
int32_t	ALU_Class::EnableWatchDog(uint32_t clk_rate ,uint32_t timeout_period ){

	wdt_set_timeout_period(&WATCHDOG, clk_rate, timeout_period);
	return wdt_enable(&WATCHDOG);

	
}
int32_t	ALU_Class::FeedWatchDog(void){
	return wdt_feed(&WATCHDOG);
}
uint8_t	ALU_Class::StartLivePulse(void){
	usb<<"**** Life Pulse Activated****"<<NEWLINE;
	//pwm_set_parameters(&PWM_0, 500, 1000);
	uint32_t p=pwm_enable(&LIVE_PULSE);
	return p;
}
bool	ALU_Class::ValidateCyclus(void){
	tick_t	nextUptimeUpdate;
	
	if (hvacTimer.get_ticks() > nextUptimeUpdate)
	{
		persistent.updateUptime(false);
		nextUptimeUpdate = hvacTimer.get_ticks()+ 1000;
	}
	
	return (alu.FeedWatchDog()>0);
}
uint32_t ALU_Class::SetInitialConfiguration(ConfigurationData& cd){
	uint32_t	w,crc,refCRC,e;
	w=0;
	uint32_t	r=memory.ReadLastConfigurationData(cd);
	if (memory.IsConfigurationDataUpdated())
	{
		r=memory.ReadUpdatedConfigurationData(configuration);
		
	}
	else
	{
		crc=memory.CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&cd),sizeof(configuration));
		refCRC=memory.ReadCRCConfigurationData();
		if (crc==refCRC)
		{
			configuration=cd;
		
		} 
		else
		{
		
			r=memory.ReadDefaultConfigurationData(configuration);
			crc=memory.CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&configuration),sizeof(configuration));
			if (crc==hvac.CRC32)
			{
				w=memory.WriteCRCConfigurationData(crc);
				
			}
			else
			{
							
				configuration=factoryDefaultsConfiguration;
				e=qspiFlash.Erase(0);
				w=memory.WriteDefaultConfiguration(configuration);
				w=memory.WriteCRCConfigurationData(hvac.CRC32);
				
			}
			
		}
		
		
	}
	hvac.SetConfigurationData(configuration);
	return r;
};

void ALU_Class::NotifyError(Internal_Fault_List ifl,const eventData& data)
{
	listener.SendErrorSelf(uint16_t(ifl),data);
}
void ALU_Class::NotifyError(Fault_List fl,const eventData& data)
{
	listener.SendErrorSelf(uint16_t(fl),data);
}
ALU_Class	alu;