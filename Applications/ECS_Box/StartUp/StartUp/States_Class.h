/* 
* States_Class.h
*
* Created: 6/22/2020 7:24:49 PM
* Author: GMateusDP
*/


#ifndef __STATES_CLASS_H__
#define __STATES_CLASS_H__
#include "ConfigurationData.h"
#include "MaintenanceTool.h"
#include "ConfigState_Class.h"
#include "EventHandler_Class.h"
#include "Interfaces_Class.h"




/** \brief GAIN flow control
 
GAINClass behandelt die Ablaufsteuerung eines GAINs
*/


class States_Class:  public	ConfigState_Class, public	Interfaces_Class
{
//variables
public:
	//	Hardware-Beschreibung: siehe ConfigurationData
	
	CycleDictionary				cycleDictionary;
	LRUCycles					lruCycles;
	PersistentConfigurationData	persistentConfiguration;
protected:
	
	friend	class	ALU_Class;

private:
		uint32_t	maximumPowerBudget;
		uint32_t	minimumPowerBudget;
		HVACState	state, lastState;
		tick_t	powerInterruptDuration;
		bool	resumeLastStateValid;
		ConfigurationData			configuration;
		uint32_t	CRC32,readResult,writeResult;
	

//functions
public:
	States_Class();
	~States_Class();
	void	Init(void);
	void	Start(uint8_t operationMode=0);
	uint8_t	Stop(uint8_t layer=0);

	void	setPersistentConfigurationDefaults(void);
	void	setPersistentConfiguration(uint8 cycleID);
	void	updateLRUCycles(CycleDescription&, int16 cycleID);

	bool	syncConfigurationWithPanel(bool syncWithInfo = true);
	void	resyncWithCycleDescription(uint8 cycleID);
	bool	resyncWithPanel(uint8 state = kGAINStateReset);
	void	GetCurrentState(HVACState& hs);
	virtual		void	SetCurrentState(HVACState&);
	uint32_t	GetStatus(HVACStatus& s);


protected:
	void	GetCurrentConfiguration(ConfigurationData *cd);
	void	SetConfigurationData(ConfigurationData& cd);
	void	SetCRCConfigData(void);
	friend class PBIT_Class;
	friend class CBIT_Class;
	friend class ALU_Class;
	void	saveCurrentState(void);
private:
	States_Class( const States_Class &c );
	States_Class& operator=( const States_Class &c );
	bool	readCycleDescription(CycleDescription&, uint8 id);

	void	handleHVACEvent(event& theEvent);

	void	handleStateReset(uint32 flags);
	void	handleStateInitialize(uint32 flags);
	void	handleStateResume(uint32 flags);
	void	handleStateStandbyOFF(uint32 flags);
	void	handleStatePrepareStandbyON(uint32 flags);
	void	handleStateStandbyON(uint32 flags);
	void	handleStateStandbyReady(uint32 flags);

	void	handleStateInterrupted(uint32 flags);

	void	handleStateON(uint32 flags);
	void	handleStateFinished(uint32 flags);
	void	handleStateStoppedProcess(uint32 flags);
	void	handleStateSelfProtect(uint32_t error);
	void	handleStateError(uint32_t error);
	void	handleStateMaintenance(uint32 flags);

	void	handleStateHMIUpdate(uint32 flags);




	bool	handleInStateEvent(event& e, tick_t t, bool& done);

	void	prepareStateChangeEvent(event& e, uint16 newState, uint16 data = 0);

	uint32_t	handlePowerOnSelftest(void);

	


	


	
	



}; //States_Class

	

extern	States_Class	hvac;

#endif //__STATES_CLASS_H__
