/* 
* FlapperValveController.cpp
*
* Created: 11/13/2020 5:34:59 PM
* Author: GMateusDP
*/


#include "FlapperValveController.h"
#include "math.h"
#include "Timer_Class.h"
#include "MemoryFlash_Class.h"


FlapperValveController	*ptrFlapperValveController;

static SingleFlapperValve_Class fvStatic(0);

static void TimeoutFlapperValveController(const struct timer_task *const timer_task)
{
	ptrFlapperValveController->timeoutFlapperValveController=true;
	//ptrFlapperValveController->timeoutKeepControlling=true;
		ptrFlapperValveController->unLockTimeout=true;
	
}
static void TimeoutFlapperValveFullyClossed(const struct timer_task *const timer_task)
{
	//	ptrFlapperValveController->timeoutFlapperValveController=true;
	//ptrFlapperValveController->timeoutKeepControlling=true;
	ptrFlapperValveController->fullClosedTimeout=true;
	
}

static void ResetValveTimeout(const struct timer_task *const timer_task)
{

	ptrFlapperValveController->resetTimeout=true;
}


// default constructor
FlapperValveController::FlapperValveController()
{
	fv=&fvStatic;
	ptrFlapperValveController=this;
	doPeriodicTask=false;
	isControlling=false;
	gotSetpoint=false;
	controllerEnabled=false;
	closing=false;
	dataStruct.controllerStatus.minimunAir=FLAPPER_VALVE_MINIMUM_AIR;
	NBC_Mode=false;
	NBC_Activated=false;
	standAloneActivated=false;
	standAloneMode=false;
	resetTimeout=false;
	standAloneReturning=false;
	
	
} //FlapperValveController

// default destructor
FlapperValveController::~FlapperValveController()
{
} //~FlapperValveController

bool FlapperValveController::InitController(){
	fv=&fvStatic;
	if (fv->Init())
	{
		isOK=Selftest();
	}
	return	isOK;	
}
void FlapperValveController::GetExitPointer(uint8_t ptr){
	ptrTask=&ptr;
}
bool FlapperValveController::IsEnabled(){
	return	fv->controlOutputs.niAlcFvMotorEnable==true;
}


bool	FlapperValveController::CalculateDirection(uint8_t sp){
	lastDirection=direction;
	direction=currentPosition>sp;
	return direction;
	
}

bool	FlapperValveController::CalculateDirection(){

	//floatSetpoint=(float)setpoint;
	return CalculateDirection(setpoint);
	
}
bool	FlapperValveController::StatusHadChanged(){
	return	fv->fv1StatusChanged;
}

uint8_t FlapperValveController::StartMovingOnDirection(){

	lastPosition=currentPosition;
//	floatLastPosition=(float)lastPosition;
	if (fv->controlOutputs.niAlcFvMotorEnable)
	{
		fv->SetEnable(false);
		delay_ms(10);
	} 
	else
	{
	}
	fv->SetDirection(CalculateDirection());
	StartMotor();
	return 0;
}

bool FlapperValveController::ControlMovement_AND_NBC(void){

	if (IsNBCMode())
	{
		CommandNBC_Mode();
	} 
	else
	{
		if (NBC_Activated)
		{
			StartControlling(dataStruct.controllerStatus.minimunAir);
			NBC_Activated=false;
		} 
		else
		{
			ControlMovement();
		}
		
	}
	return !NBC_Mode;
}

bool FlapperValveController::IsInvalidPosition(){
	bool result=false;
	memory.ReadParameters(parameters);
	UpdateFlapperValveData();
	if ((!standAloneReturning)&(!IsNBCMode()))
	{
		if (currentPosition>currentMaximum)
		{
			result=true;
		}
		else
		{
			result=false;
		}
	}

	return result;
}

bool FlapperValveController::Control_NBC_StandAlone_Reset(void){
	
	if ((IsInvalidPosition()&(!flapperValveIsMoving)))
	{
		ResetValvePosition();
	} 
	else
	{
		Control_NBC_StandAlone();
	}
	return resetTimeout;
}

bool FlapperValveController::Control_NBC_StandAlone(void){

	if (IsStandAloneMode())
	{	

			standAloneMode=true;
			if (standAloneActivated)
			{
				ControlMovement_AND_NBC();
			}
			else
			{
				
				if ((currentPosition>0xff-parameters.flapperValveStandAloneMinimumPosition))
				{
					StartControlling(0xff-parameters.flapperValveStandAloneMinimumPosition);
					standAloneReturning=true;
									
				}
				else
				{
					
				}
				currentMaximum=0xff-parameters.flapperValveStandAloneMinimumPosition;
				standAloneActivated=true;
			}

		
		
	} 
	else
	{
		standAloneMode=false;
		if (standAloneActivated)
		{
		//	StartControlling(parameters.flapperValveMinimumPosition);
			standAloneActivated=false;
			standAloneReturning=false;
			
			
			
		} 
		else
		{
		
		}
			ControlMovement_AND_NBC();
		
	}
	return standAloneMode;
}

bool	FlapperValveController::IsStandAloneMode(){
	bool result=false;
	if ((dataStruct.controlOutputs.iAlcFvStandAloneOut)&(!dataStruct.inputStatus.cabin[1]))
	{
		result=true;
	} 
	else
	{
		result=false;
	}
	return result;
}

uint8_t FlapperValveController::ControlMovement(){
	if (isControlling)
	{	
		if (((dataStruct.inputStatus.limit[0]|dataStruct.inputStatus.limit[1])&(closing))&(unLockTimeout))
		{
				StopValveAtLimit();

		} 
		else
		{
			
		//	currentPosition=fv->ReadActualPosition();
			if ((((setpoint<6)&(currentPosition<6))|((setpoint>250)&(currentPosition>245)))&((!closing)&(flapperValveIsMoving)))
			{
				hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValveFullyClossed),FLAPPER_VALVE_CLOSING_TIMEOUT);
				fullClosedTimeout=false;
				gotSetpoint=false;
				closing=true;
				unLockTimeout=true;
			}
			//floatCurrentPosition=(float)currentPosition;
			if ((abs(currentPosition-setpoint)>tolerance)|(closing))
			{
				if (flapperValveIsMoving)
				{
					if (closing)
					{
						if (keepControlling)
						{
							if (fullClosedTimeout)
							{
								StopValveAtLimit();
							}
							else
							{
							//	KeepControlling();
							}
							
						}
					} 
					else
					{
						
						if (lastDirection!=CalculateDirection())
						{
							StopMotor();
							hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValveController));
							keepControlling=true;
						}
						else
						{
							if (timeoutFlapperValveController)
							{
								StopMotor();
								
							}
							else
							{
								if (keepControlling)
								{
									if (timeoutKeepControlling)
									{
										StopMotor();
									}
									else
									{
										KeepControlling();
									}
									
								}
								
								
							}
						}
					}
					
				}
				else
				{
					if (keepControlling)
					{
						if (timeoutKeepControlling)
						{
							StopValveinBetween();
						}
						else
						{
							KeepControlling();
						}
						
					}
					
				}
				
				if ((abs(currentPosition-setpoint)>tolerance))
				{
					gotSetpoint=false;
				} 
				else
				{
				}
				
				
			}
			else
			{
				if (closing)
				{
					if (fullClosedTimeout)
					{
						StopValveAtLimit();
						
					} 
					else
					{
					}
				} 
				else
				{
					StopValveinBetween();
					gotSetpoint=true;
					//isControlling=false;
				}
				
			}
		}
		
	} 
	else
	{
	}
	return 0;
	
}
void FlapperValveController::ResetValvePosition(void){
	uint8_t localSetpoint,localPosition;
	bool pinLevel=gpio_get_pin_level(LED0);
	if (IsInvalidPosition())
	{
		
		localSetpoint=currentMaximum;
		
		fv->SetEnable(false);
		fv->SetInvalidPosition(false);
		fv->ClearMoveFault(false);
		delay_ms(100);
		if (fv->ReadActualPosition()>localSetpoint)
		{
			fv->SetDirection(true);
		}
		else
		{
			fv->SetDirection(false);
		}
		fv->SetInvalidPosition(true);
		fv->ClearMoveFault(true);
		fv->WriteSetpoint(localSetpoint);
		
		currentPosition=fv->ReadActualPosition();
		
		resetTimeout=false;
		hvacTimer.Start_oneShot_task(FUNC_PTR(ResetValveTimeout),1000);
				fv->SetEnable(true);
		while ((abs(currentPosition-localSetpoint)>6)&(!resetTimeout))
		{			gpio_toggle_pin_level(LED0);
			currentPosition=fv->ReadActualPosition();
		}
		fv->SetEnable(false);
		gpio_set_pin_level(LED0,pinLevel);
	} 
	else
	{
	}
}

uint8_t FlapperValveController::CommandNBC_Mode(void){
	if (NBC_Activated)
	{
		if (NBC_Mode)
		{
			ControlMovement();
		}
		else
		{
			StartControlling(dataStruct.controllerStatus.minimunAir);
			NBC_Activated=false;
		}
	} 
	else
	{
			StartControlling(255);
			NBC_Activated=true;
	}

	return 0;
	
}

bool FlapperValveController::IsNBCMode(){
	NBC_Mode=((dataStruct.inputStatus.cabin[0]&dataStruct.inputStatus.cabin[1])|NBC_Remote);
	currentMaximum=255;
	return NBC_Mode;
}

bool FlapperValveController::SetRemoteNBCMode(bool st){
	if (st!=NBC_Mode)
	{
		NBC_Remote=st;
	} 
	else
	{
		
	}
	return NBC_Remote;
}


uint8_t	FlapperValveController::StopMotor(){
	fv->SetEnable(false);
	flapperValveIsMoving=false;
	
	return 0;
}
void	FlapperValveController::StopValveAtLimit(void){
		if (closing)
		{
			closing=false;
		}
	
		StopMotor();
		hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValveController));
		hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValveFullyClossed));
		timeoutFlapperValveController=false;
		timeoutKeepControlling=false;
		unLockTimeout=false;
		paused=false;
		keepControlling=false;
}

void FlapperValveController::StopValveinBetween(void){
		StopMotor();
		hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValveController));
		timeoutFlapperValveController=false;
		paused=false;
		timeoutKeepControlling=false;
		keepControlling=true;
}



uint8_t	FlapperValveController::CorrectTolerance(uint8_t sp){
	float floatTolerance=FLAPPER_VALVE_CONTROL_LIMIT*((abs(127-sp)-128))/256;
	tolerance=(uint8_t)floatTolerance;
	return tolerance;
}
uint8_t	FlapperValveController::StartMotor(){
	if (fv->controlOutputs.niAlcFvMotorEnable)
	{
		fv->SetEnable(false);
		delay_ms(10);
	}
	
		
	fv->SetEnable(true);
	flapperValveIsMoving=true;
	return 0;
}
uint8_t FlapperValveController::StopControlling(void){
	StopMotor();
	timeoutKeepControlling=false;
	timeoutFlapperValveController=false;
	isControlling=false;
	keepControlling=false;
	controllerEnabled=false;
	return 0;
}

uint8_t	FlapperValveController::StartControlling(uint8_t sp){
		setpoint=VerifySetpoint(sp);
		CorrectTolerance(setpoint);
		floatSetpoint=(float)setpoint;
		timeoutFlapperValveController=false;
		timeoutKeepControlling=false;
		keepControlling=false;
		controllerEnabled=true;
		unLockTimeout=false;
		hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValveController),FLAPPER_VALVE_TIMEOUT);
		isControlling=true;
		StartMovingOnDirection();
	//	while(!gotSetpoint&(!doPeriodicTask)){
			ControlMovement();
	//	}	
		if (doPeriodicTask)
		{
			paused=true;
		}
		return 0;
	
}
uint8_t FlapperValveController::VerifySetpoint(uint8_t sp){
	
	if (IsNBCMode())
	{
		setpoint=sp;
	} 
	else
	{
		if (standAloneActivated)
		{
			standAlonePosition=0xff-parameters.flapperValveStandAloneMinimumPosition;
			if (sp>(standAlonePosition))
			{
				setpoint=standAlonePosition;
				currentMaximum=standAlonePosition;
			}
			else
			{
				setpoint=sp;
			}
		}
		else
		{
			maximumPosition=0xff-parameters.flapperValveMinimumPosition;
			if (sp>maximumPosition)
			{
				setpoint=maximumPosition;
				currentMaximum=maximumPosition;
			}
			else
			{
				setpoint=sp;
			}
		}
	}
	
	return setpoint;
	
}
uint8_t FlapperValveController::UpdateFlapperValvePosition(void){
	currentPosition=fv->ReadActualPosition();
	//floatCurrentPosition=(float)currentPosition;
	return currentPosition;
}
	
uint8_t	FlapperValveController::StartControlling(void){
	return	StartControlling(currentPosition);
	
}
uint8_t	FlapperValveController::KeepControlling(uint8_t restart){

	//setpoint=lastPosition;

		timeoutKeepControlling=false;
	//	hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValveController),FLAPPER_VALVE_TIMEOUT/2);
		isControlling=true;
		StartMovingOnDirection();
		keepControlling=false;

	return 0;
	
}

void	FlapperValveController::UpdateFlapperValveData(void){
	currentPosition=fv->ReadActualPosition();
	dataStruct.actualPosition=currentPosition;
	dataStruct.setPointPosition=setpoint;
	dataStruct.inputStatus=fv->ReadStatusInputs();
	dataStruct.controlOutputs=fv->ReadControlStatus();
	
	dataStruct.controllerStatus.isMoving=flapperValveIsMoving;
	dataStruct.controllerStatus.isControlling=isControlling;
	dataStruct.controllerStatus.NBC_Mode=NBC_Activated;
	if (standAloneActivated)
	{
		currentMaximum=0xff-parameters.flapperValveStandAloneMinimumPosition;
	} 
	else
	{
		currentMaximum=0xff-parameters.flapperValveMinimumPosition;
	}
}

uint8_t	 FlapperValveController::GetCurrentPosition(){
	return fv->ReadActualPosition();
}
bool	FlapperValveController::Selftest(void){
		uint8_t localSetpoint=180;
		parameters=defaultParameters;
		uint8_t localPosition;
	UpdateFlapperValveData();
	fv->SetEnable(false);
	fv->SetInvalidPosition(false);
	fv->ClearMoveFault(false);
	delay_ms(10);
	fv->ClearMoveFault(true);
	fv->WriteSetpoint(localSetpoint);
	fv->SetDirection(false);
	if (fv->ReadActualPosition()>localSetpoint)
	{
		fv->SetDirection(true);
	}
	else
	{
		fv->SetDirection(false);
	}
	hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValveController),10000);
	fv->SetEnable(true);
	localPosition=fv->ReadActualPosition();	timeoutFlapperValveController=false;
	while ((abs(localPosition-localSetpoint)>6)&(!timeoutFlapperValveController))
	{
		localPosition=fv->ReadActualPosition();
	}
	hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValveController));
	fv->SetEnable(false);
	return !timeoutFlapperValveController;
}

FlapperValveController fvc;