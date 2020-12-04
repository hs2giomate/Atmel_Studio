/* 
* FlapperValveController.h
*
* Created: 11/13/2020 5:35:00 PM
* Author: GMateusDP
*/


#ifndef __FLAPPERVALVECONTROLLER_H__
#define __FLAPPERVALVECONTROLLER_H__
#include "SingleFlapperValve_Class.h"
#include "Parameters.h"
#define  FLAPPER_VALVE_TIMEOUT	10000
#define  FLAPPER_VALVE_UNLOCK_TIMEOUT	500
#define  FLAPPER_VALVE_CLOSING_TIMEOUT	200
#define FLAPPER_VALVE_CONTROL_LIMIT	4



class FlapperValveController: public  SingleFlapperValve_Class
{
//variables
public:
	volatile bool timeoutFlapperValveController, flapperValveIsMoving, doPeriodicTask,controllerEnabled,timeoutKeepControlling,isOK;
	SingleFlapperValve_Class* fv;
	FVDataStruct	dataStruct;
	volatile bool	unLockTimeout,fullClosedTimeout,gotSetpoint,resetTimeout;
protected:
private:
	
	uint8_t currentPosition, lastPosition,setpoint,tolerance;
	
	float floatCurrentPosition,floatLastPosition,floatSetpoint;
	bool	paused, isControlling,keepControlling,direction,lastDirection,reachLimit1,reachLimit2;
	bool closing,NBC_Mode,NBC_Activated,standAloneMode,standAloneActivated,standAloneReturning,NBC_Remote;
	volatile uint8_t *ptrTask;
	uint8_t	maximumPosition, standAlonePosition,currentMaximum;
	UserParameters	parameters;
	

//functions
public:
	FlapperValveController();
	~FlapperValveController();
	bool InitController();
	uint8_t	StopMotor();
	uint8_t	StartMotor();
	uint8_t	StartControlling(uint8_t sp);
	uint8_t	 GetCurrentPosition();
	uint8_t ControlMovement();
	void	GetExitPointer(uint8_t ptr);
	bool CalculateDirection();
	bool	StatusHadChanged();
	bool IsEnabled();
	
	uint8_t StopControlling(void);
	uint8_t	StartControlling(void);
	uint8_t UpdateFlapperValvePosition(void);
	void	UpdateFlapperValveData(void);
	bool	ControlMovement_AND_NBC();
	uint8_t VerifySetpoint(uint8_t sp);
	bool	Control_NBC_StandAlone(void);
	bool	Control_NBC_StandAlone_Reset(void);
	bool IsInvalidPosition(void);
	bool SetRemoteNBCMode(bool st);
	bool	IsNBCMode(void);
	
	
protected:
private:
	FlapperValveController( const FlapperValveController &c );
	FlapperValveController& operator=( const FlapperValveController &c );
	bool	CalculateDirection(uint8_t sp);
	uint8_t StartMovingOnDirection();
	void	StopValveAtLimit(void);
	uint8_t	CorrectTolerance(uint8_t sp);
	void StopValveinBetween(void);
	uint8_t CommandNBC_Mode(void);
	
	uint8_t	KeepControlling(uint8_t restart=0);
	bool	IsStandAloneMode(void);
	bool Selftest(void);
	
	
	void ResetValvePosition(void);	
	

}; //FlapperValveController

extern FlapperValveController fvc;



#endif //__FLAPPERVALVECONTROLLER_H__
