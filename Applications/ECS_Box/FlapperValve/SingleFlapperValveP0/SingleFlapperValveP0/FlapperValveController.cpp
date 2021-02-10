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
#include "Event_Logger_Class.h"


static uint8_t  position_array_static[FLAPPER_VALVE_QUANTITY][FLAPPER_VALVE_POSITIONS_BUFFER];

FlapperValveController	*ptrFlapperValveController[FLAPPER_VALVE_QUANTITY];

static SingleFlapperValve_Class fvStatic[FLAPPER_VALVE_QUANTITY];

static void TimeoutFlapperValve1Controller(const struct timer_task *const timer_task)
{
	ptrFlapperValveController[0]->timeoutFlapperValveController=true;
	//ptrFlapperValveController->timeoutKeepControlling=true;
		ptrFlapperValveController[0]->unLockTimeout=true;
	
}
static void TimeoutFlapperValve2Controller(const struct timer_task *const timer_task)
{
	ptrFlapperValveController[1]->timeoutFlapperValveController=true;
	//ptrFlapperValveController->timeoutKeepControlling=true;
	ptrFlapperValveController[1]->unLockTimeout=true;
	
}
static void TimeoutFlapperValve1FullyClossed(const struct timer_task *const timer_task)
{
	//	ptrFlapperValveController->timeoutFlapperValveController=true;
	//ptrFlapperValveController->timeoutKeepControlling=true;
	ptrFlapperValveController[0]->fullClosedTimeout=true;
	
}
static void TimeoutFlapperValve2FullyClossed(const struct timer_task *const timer_task)
{
	//	ptrFlapperValveController->timeoutFlapperValveController=true;
	//ptrFlapperValveController->timeoutKeepControlling=true;
	ptrFlapperValveController[1]->fullClosedTimeout=true;
	
}

static void ResetValve1Timeout(const struct timer_task *const timer_task)
{

	ptrFlapperValveController[0]->resetTimeout=true;
}
static void ResetValve2Timeout(const struct timer_task *const timer_task)
{

	ptrFlapperValveController[1]->resetTimeout=true;
}


// default constructor
FlapperValveController::FlapperValveController()
{
	singlefv=&fvStatic[0];
//	ptrFlapperValveController=this;
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
	partner=NULL;
	
	
} //FlapperValveController

// default destructor
FlapperValveController::~FlapperValveController()
{
} //~FlapperValveController

bool FlapperValveController::InitController(void){
	ptrFlapperValveController[0]=this;
	singlefv=&fvStatic[0];
		positions[0]=position_array_static[0];
	if (singlefv->Init())
	{
		isOK=Selftest();
	}
	return	isOK;	
}
bool FlapperValveController::InitController(uint8_t valve_id){
	ptrFlapperValveController[valve_id]=this;
		valve_ID=valve_id;
		singlefv=&fvStatic[valve_id];
		positions[valve_id]=position_array_static[valve_id];
		if (singlefv->Init(valve_id))
		{
			isOK=Selftest();
			logger.SaveEventIndexResult("Flapper Valve ",valve_id+1,isOK);
		}
		return	isOK;
}
void FlapperValveController::GetExitPointer(uint8_t ptr){
	ptrTask=&ptr;
}
bool FlapperValveController::IsEnabled(){
	return	singlefv->controlOutputs.niAlcFvMotorEnable==true;
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
	return	singlefv->fv1StatusChanged;
}

uint8_t FlapperValveController::StartMovingOnDirection(){

	lastPosition=currentPosition;
//	floatLastPosition=(float)lastPosition;
	if (singlefv->controlOutputs.niAlcFvMotorEnable)
	{
		singlefv->SetEnable(false);
		delay_ms(10);
	} 
	else
	{
	}
	singlefv->SetDirection(CalculateDirection());
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

void FlapperValveController::ReadGPIOS(uint8_t *buffer){
	singlefv->ReadGPIOs(buffer);
}

bool FlapperValveController::Control_NBC_StandAlone_Reset(void){
	
	if ((IsInvalidPosition()&(!flapperValveIsMoving)))
	{
		ResetValvePosition();
	} 
	else
	{
		if (dataStruct.controlOutputs.iAlcFvFailsToMove)
		{
			singlefv->ClearMoveFault(false);
			delay_us(10);
			singlefv->ClearMoveFault(true);
		} 
		else
		{
			Control_NBC_StandAlone();
		}

		
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
				
				if ((currentPosition>parameters.flapperValveStandAloneMinimumPosition[valve_ID]))
				{
					StartControlling(parameters.flapperValveStandAloneMinimumPosition[valve_ID]);
					standAloneReturning=true;
									
				}
				else
				{
					
				}
				currentMaximum=parameters.flapperValveStandAloneMinimumPosition[valve_ID];
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
	if (partner!=NULL)
	{
		if (partner->isOK)
		{
			result=partner->dataStruct.controlOutputs.iAlcFvStandAloneOut;
		}
		else
		{
			result=true;
		}
	} 
	else
	{
		result=true;
	}
	


	return result;
}



void FlapperValveController::SetPartner(FlapperValveController *part){
	partner=part;
}
bool FlapperValveController::CheckIsClosing(void){
		if ((((setpoint<6)&(currentPosition<6))|((setpoint>250)&(currentPosition>245)))&((!closing)&(is_motor_on)))
		{
			if (valve_ID==0)
			{
				hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValve1FullyClossed),FLAPPER_VALVE_CLOSING_TIMEOUT);
			} 
			else
			{
				hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValve2FullyClossed),FLAPPER_VALVE_CLOSING_TIMEOUT);
			}
		
			fullClosedTimeout=false;
			gotSetpoint=false;
			closing=true;
			unLockTimeout=true;
		}
		return closing;
	
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
			
			CheckIsClosing();
			if ((abs(currentPosition-setpoint)>tolerance)|(closing))
			{
				if (IsFlapperMoving())
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
							RemoveTimeoutTask();
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
							StopValveinBetween(false);
						}
						else
						{
							KeepControlling();
						}
						
					}
					
				}
				
				if ((abs(currentPosition-setpoint)>tolerance))
				{
					
					if (timeoutFlapperValveController)
					{
						StopValveinBetween(false);
						gotSetpoint=false;
					}
					else
					{
						//KeepControlling();
					}
					
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
					if (gotSetpoint)
					{
					} 
					else
					{
						StopValveinBetween();
						gotSetpoint=true;
					}
					
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
		
		singlefv->SetEnable(false);
		singlefv->SetInvalidPosition(false);
		singlefv->ClearMoveFault(false);
		delay_ms(100);
		if (singlefv->ReadActualPosition()>localSetpoint)
		{
			singlefv->SetDirection(true);
		}
		else
		{
			singlefv->SetDirection(false);
		}
		singlefv->SetInvalidPosition(true);
		singlefv->ClearMoveFault(true);
		singlefv->WriteSetpoint(localSetpoint);
		
		currentPosition=singlefv->ReadActualPosition();
		
		resetTimeout=false;
		if (valve_ID==0)
		{
			hvacTimer.Start_oneShot_task(FUNC_PTR(ResetValve1Timeout),1000);
		} 
		else
		{
			hvacTimer.Start_oneShot_task(FUNC_PTR(ResetValve2Timeout),1000);
		}
	
				singlefv->SetEnable(true);
		while ((abs(currentPosition-localSetpoint)>6)&(!resetTimeout))
		{			gpio_toggle_pin_level(LED0);
			currentPosition=singlefv->ReadActualPosition();
		}
		singlefv->SetEnable(false);
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
	singlefv->SetEnable(false);
	flapperValveIsMoving=false;
	is_motor_on=false;
	return 0;
}

bool FlapperValveController::IsFlapperMoving(void){
	bool_result=true;
	if (flapperValveIsMoving)
	{
		for (uint8_t i = 0; i < FLAPPER_VALVE_POSITIONS_BUFFER-1; i++)
		{
			if (positions[valve_ID][i]==positions[valve_ID][i+1])
			{
				bool_result=false;
			}
			else
			{
				bool_result=true;
				break;
			}
			
		}
		flapperValveIsMoving&=bool_result;
	} 
	else
	{
	}
	return flapperValveIsMoving;
	
}

void	FlapperValveController::StopValveAtLimit(void){
		if (closing)
		{
			closing=false;
		}
	
		StopMotor();
		if (valve_ID==0)
		{
					hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValve1Controller));
					hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValve1FullyClossed));
		} 
		else
		{
					hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValve2Controller));
					hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValve2FullyClossed));
		}
	
		timeoutFlapperValveController=false;
		timeoutKeepControlling=false;
		unLockTimeout=false;
		paused=false;
		keepControlling=false;
}

void FlapperValveController::StopValveinBetween(bool gs){
		StopMotor();
		RemoveTimeoutTask();
		timeoutFlapperValveController=false;
		paused=false;
		timeoutKeepControlling=false;
		keepControlling=gs;
		if (gs)
		{
			
		} 
		else
		{
			StopControlling();
		}
}



uint8_t	FlapperValveController::CorrectTolerance(uint8_t sp){
	float floatTolerance=FLAPPER_VALVE_CONTROL_LIMIT*((abs(127-sp)-128))/256;
	tolerance=(uint8_t)floatTolerance;
	if (tolerance<1)
	{
		tolerance=0x01;
	}
	return tolerance;
}
uint8_t	FlapperValveController::StartMotor(){
	if (singlefv->controlOutputs.niAlcFvMotorEnable)
	{
		singlefv->SetEnable(false);
		delay_ms(10);
	}
	
		
	singlefv->SetEnable(true);
	flapperValveIsMoving=true;
	is_motor_on=flapperValveIsMoving;
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
		if (valve_ID==0)
		{
			hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValve1Controller),FLAPPER_VALVE_TIMEOUT);
		} 
		else
		{
			hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValve2Controller),FLAPPER_VALVE_TIMEOUT);
		}
		
		isControlling=true;
		StartMovingOnDirection();
	//	while(!gotSetpoint&(!doPeriodicTask)){
		gotSetpoint=false;
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
			standAlonePosition=parameters.flapperValveStandAloneMinimumPosition[valve_ID];
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
			maximumPosition=parameters.flapperValveMinimumPosition[valve_ID];
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
	currentPosition=singlefv->ReadActualPosition();
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
	currentPosition=singlefv->ReadActualPosition();
	//dataStruct.controllerStatus.position=currentPosition;
	dataStruct.actualPosition=currentPosition;
	dataStruct.setPointPosition=setpoint;
	dataStruct.inputStatus=singlefv->ReadStatusInputs();
	dataStruct.controlOutputs=singlefv->ReadControlStatus();
	
//	dataStruct.controllerStatus.isMoving=flapperValveIsMoving;
	dataStruct.controllerStatus.isDriverON=is_motor_on;
	dataStruct.controllerStatus.isControlling=isControlling;
	dataStruct.controllerStatus.NBC_Mode=NBC_Activated;
	dataStruct.controllerStatus.is_flapper_OK=isOK;
	dataStruct.controllerStatus.is_stand_alone=IsStandAloneMode();
	dataStruct.controllerStatus.got_setpoint=gotSetpoint;
	
	if (standAloneActivated)
	{
		currentMaximum=parameters.flapperValveStandAloneMinimumPosition[valve_ID];
	} 
	else
	{
		currentMaximum=parameters.flapperValveMinimumPosition[valve_ID];
	}
	FillPositionsFIFO(currentPosition);
}
void FlapperValveController::FillPositionsFIFO(uint8_t cp){
	for (uint8_t i = 0; i < FLAPPER_VALVE_POSITIONS_BUFFER-1; i++)
	{
		positions[valve_ID][FLAPPER_VALVE_POSITIONS_BUFFER-1-i]=positions[valve_ID][FLAPPER_VALVE_POSITIONS_BUFFER-2-i];
	}
	positions[valve_ID][0]=cp;
}

uint8_t	 FlapperValveController::GetCurrentPosition(){
	return singlefv->ReadActualPosition();
}

bool	FlapperValveController::Selftest(void){
		uint8_t localSetpoint=180;
		parameters=defaultParameters;
		uint8_t localPosition;
		bool must_be_tested=true;
	UpdateFlapperValveData();
	singlefv->SetEnable(false);
	singlefv->SetInvalidPosition(false);
	singlefv->ClearMoveFault(false);
	delay_ms(10);
	singlefv->ClearMoveFault(true);
	singlefv->WriteSetpoint(localSetpoint);
	singlefv->SetDirection(false);

	
		if (valve_ID==0)
		{
			hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValve1Controller),FLAPPER_VALVE_TIMEOUT);
		}
		else
		{
			hvacTimer.Start_oneShot_task(FUNC_PTR(TimeoutFlapperValve2Controller),FLAPPER_VALVE_TIMEOUT);
		}
	
		localPosition=singlefv->ReadActualPosition();		if ((abs(localPosition-localSetpoint)<(12+1)))
		{		#ifdef ATP_TEST
	
		
			if (valve_ID==0)
			{
				localSetpoint-=64;
				must_be_tested=true;
			}else{
				if ((abs(localPosition-ptrFlapperValveController[0]->GetCurrentPosition())<(6)))
				{
					localSetpoint=localPosition;
					must_be_tested=false;
				} 
				else
				{
				}
			}

	
#else
	
			if (valve_ID==0)
			{
				localSetpoint-=64;
				must_be_tested=true;
			}
			else
			{
				localSetpoint=localPosition;
				must_be_tested=false;
			}
			
			}else{
			if (valve_ID==1)
			{
				localSetpoint=localPosition;
				must_be_tested=false;
				
			}
			else
			{
			}
	
#endif
		}
		else
		{
#ifdef ATP_TEST
		
			if (valve_ID==1)
			{
				if ((abs(localPosition-ptrFlapperValveController[0]->GetCurrentPosition())<(6)))
				{
					localSetpoint=localPosition;
					must_be_tested=false;
				}
				else
				{
				}
			}

#endif
			
		}
	
	timeoutFlapperValveController=false;
	if (must_be_tested)
	{
		if (localPosition>localSetpoint)
		{
			singlefv->SetDirection(true);
		}
		else
		{
			singlefv->SetDirection(false);
		}
		
		singlefv->SetEnable(true);
		while ((abs(localPosition-localSetpoint)>12)&(!timeoutFlapperValveController))
		{
			localPosition=singlefv->ReadActualPosition();
		}
		RemoveTimeoutTask();

		singlefv->SetEnable(false);
	} 
	else
	{
		gotSetpoint=true;
	}
	gotSetpoint=!timeoutFlapperValveController;
	return gotSetpoint;
}


void	FlapperValveController::RemoveTimeoutTask(void){
	if (valve_ID==0)
	{
		hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValve1Controller));
	}
	else
	{
		hvacTimer.Remove_task(FUNC_PTR(TimeoutFlapperValve2Controller));
	}
}

FlapperValveController fvc;