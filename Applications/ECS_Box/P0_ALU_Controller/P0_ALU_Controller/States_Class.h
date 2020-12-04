/* 
* States_Class.h
*
* Created: 6/22/2020 7:24:49 PM
* Author: GMateusDP
*/


#ifndef __STATES_CLASS_H__
#define __STATES_CLASS_H__
#include "ConfigurationData.h"
#include "Maintenance_Tool.h"
#include "ConfigState_Class.h"
#include "EventHandler_Class.h"
#include "Interfaces_Class.h"
#include "HVAC_Commando_Class.h"
#include "HVAC_Controller.h"
#define		CHECK_EVENT_STATE_TIMEOUT 8



/** \brief GAIN flow control
 
GAINClass behandelt die Ablaufsteuerung eines GAINs
*/


class States_Class:  public virtual	ConfigState_Class, public	Interfaces_Class, public virtual HVAC_Commando_Class, public  HVAC_Controller
{
//variables
public:
	//	Hardware-Beschreibung: siehe ConfigurationData
	
	CycleDictionary				cycleDictionary;
	LRUCycles					lruCycles;
	PersistentConfigurationData	persistentConfiguration;
	volatile	bool isOK,timeElapsed;
	HVACState	*hvacState;
protected:
	
	friend	class	ALU_Class;

private:
		uint32_t	maximumPowerBudget;
		uint32_t	minimumPowerBudget;
		HVACState	lastHVACState;
		tick_t	powerInterruptDuration;
		bool	resumeLastStateValid,gotAluEvent,gotHVACEvent, gotAnyEvent;
		ConfigurationData			configuration;
		uint32_t	CRC32,readResult,writeResult;
	

//functions
public:
	States_Class();
	~States_Class();
	bool	Init(void);
	void	Start(uint8_t operationMode=0);
	uint8_t	Stop(uint8_t layer=0);

	void	setPersistentConfigurationDefaults(void);
	void	setPersistentConfiguration(uint8 cycleID);
	void	updateLRUCycles(CycleDescription&, int16 cycleID);

	bool	syncConfigurationWithPanel(bool syncWithInfo = true);
	void	resyncWithCycleDescription(uint8 cycleID);
	bool	resyncWithPanel(uint8 state = kHVACStateReset);
	void	GetCurrentState(HVACState& hs);
	virtual		void	SetCurrentState(HVACState&);
	uint32_t	GetStatus(HVACStatus& s);
	void ControllerResume(uint8_t	operationMode);
	

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
	void	ExecutePeriodicTask(void);
	bool	readCycleDescription(CycleDescription&, uint8 id);

	void	handleHVACEvent(event& theEvent);

	void	handleStateReset(uint32 flags);
	void	StateInitialize(uint32 flags);
	void	StateResume(uint32 flags);
	void	StateStandbyOFF(uint32 flags);
	void	StatePrepareStandbyON(uint32 flags);
	void	handleStateStandbyON(uint32 flags);
	void	handleStateStandbyReady(uint32 flags);

	void	handleStateInterrupted(uint32 flags);

	void	handleStateON(uint32 flags);
	void	handleStateFinished(uint32 flags);
	void	StateStoppedProcess(uint32 flags);
	void	handleStateSelfProtect(uint32_t error);
	void	handleStateError(uint32_t error);
	void	handleStateMaintenance(uint32 flags);

	void	handleStateHMIUpdate(uint32 flags);
	void	prepareNewEvent(event& e, uint16 newState, uint16 data = 0);
	void	StateLeaving(uint32 flags);
	void	StateAUTO(uint32 flags);
	void	StateVENT(uint32 flags);




	bool	InStateEvent(event& e, tick_t t, bool& done);
	bool	InStateEvent(event& e, tick_t t);

	void	prepareStateChangeEvent(uint16 newState, uint16 data = 0);

	uint32_t	handlePowerOnSelftest(void);
	static void CheckEvents(void);
	void	StatePrepareStandbyOFF(uint32 flags);

	


	


	
	



}; //States_Class

	

extern	States_Class	hvac;

#endif //__STATES_CLASS_H__
