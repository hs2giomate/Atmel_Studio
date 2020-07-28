/* 
* LedDriverClass.cpp
*
* Created: 7/15/2020 2:07:08 PM
* Author: GMateusDP
*/


#include "LedHandler_Class.h"
#include "Timer_Class.h"
//#include "FlashMemoryClass.h"
#include "NMIButton_Class.h"


LedHandler_Class		*ptrLedHandler;
uint32_t	tempTicks;
static void RestartLeds(const struct timer_task *const timer_task)
{
	ptrLedHandler->restarted=true;
}
static void PWMPeriodExpired(struct _pwm_device *device){
	

	ptrLedHandler->deadTime=!lighter.deadTime;
}

static void BlinkTimer(const struct timer_task *const timer_task)
{
	ptrLedHandler->blinkOut=true;
}

static void PowerOnTimer(const struct timer_task *const timer_task)
{
	ptrLedHandler->deadTime=true;
}
static void PowerOffTimer(const struct timer_task *const timer_task)
{
	ptrLedHandler->deadTime=false;
}



// default constructor
LedHandler_Class::LedHandler_Class()
{
	ready=false;
	modus=INITIAL_STATE;
	ptrLedHandler=this;
	totalPins=kTOTALLEFTPINS+kTOTALRIGHTPINS;
	dutyCycle=500;
	stepTime=POWERON_SINGLE_TIME;
	period=POWER_PERIOD;
	
	} //LedDriverClass

// default destructor
LedHandler_Class::~LedHandler_Class()
{
} //~LedDriverClass

void	LedHandler_Class::Init(void){
	InitMapping();
	for (int i=0;i<kTOTALLEFTPINS;i++)
	{
		pins.allPins[i]=leftPins.pinsLeft[i];
		pins.allPorts[i]=leftPins.portsLeft[i];
	}
	for (int i=0;i<kTOTALRIGHTPINS;i++)
	{
		pins.allPins[totalPins-i-1]=rightPins.pinsRight[i];
		pins.allPorts[totalPins-i-1]=rightPins.portsRight[i];
	}
	SetBothSidesPins(pins);
	deadTime=false;

	ledTimer.Init();
	pwmTimer.Init();
	DefineColors(state);
	ready=true;
	pwm_set_parameters(&PWM_0,PWM_WAVE_PERIOD,PWM_WAVE_DUTY);
	pwm_register_callback(&PWM_0,PWM_PERIOD_CB, (pwm_cb_t)PWMPeriodExpired);
	pwmTimer.stop();
	tapLimit=COUNTER_TIME*(kTOTALLEFTPINS+kTOTALLEFTPINS);

}

void	LedHandler_Class::Run(void){
	ClearAll();
	//GreenWave();

	while(1){
		if (nmi.IsButtonPressed())
		{
	
			nmi.OnPressed();
	
			if (!changeModeRequest)
			{
		
					RunOnMode();
					
			} 
			else
			{
				ChangeModus();
				
			}
		
		
		} 
		else
		{
		
			if (changeModeRequest){
				
				ChangeModus();
			
			}else{
				
				RunOnMode();
			}
		
		}
		
	}
}

void	LedHandler_Class::ChangeModus(void){
		ledTimer.stop();
		restarted=false;
		ledTimer.Start_oneShot_task((FUNC_PTR)RestartLeds,POWEROFF_SINGLE_TIME);
		ClearAll();
		nmi.pressedTimerStarted=false;
		changeModeRequest=false;
		stepTime=COUNTER_TIME;
		modus=(modus+1)%NUMBER_STATES;
		if (modus>NUMBER_STATES)
		{
			modus=0;
		}
		
		while(!restarted);
		//ledTimer.remove_task();
		RunOnMode();
}
void	LedHandler_Class::RestartLedModus(void){
			ledTimer.stop();
			restarted=false;
			ledTimer.Start_oneShot_task((FUNC_PTR)RestartLeds,POWEROFF_SINGLE_TIME);
			ClearAll();
			nmi.pressedTimerStarted=false;
			changeModeRequest=false;
			stepTime=COUNTER_TIME;
}

void	LedHandler_Class::RunOnMode(void){
	switch (modus)
	{
		case 0:
			GreenWave();
		/* Your code here */
		break;
		case 1:
			RedWave();
		/* Your code here */
		break;
		case 2:
			BlueWave();
		/* Your code here */
		break;
		case 3:
			LilaWave();
		/* Your code here */
		break;
		case 4:
			MagentaWave();
		/* Your code here */
		break;
		default:
			GreenWave();
		/* Your code here */
		break;
	}
}

void	LedHandler_Class::ActivateAllGreen(void){
	for (i = 0; i < totalPins; i++)	{
		if (state.color[i]==0)
		{
			state.status[i]=1;
		}
		else
		{
			state.status[i]=0;
		}
	}
		
	
}

void	LedHandler_Class::GreenWave(void){
	request=false;
		do 
		{
			for ( k=0;k<totalPins;k++)
			{
				if (IsGreen(k))
				{
					if (!PowerOnSingleLed(k))
					{
						break;
					}
					
				}
				else
				{
					PowerOffPin(k);
				}
							
			}
		}while((!changeModeRequest));
	request=false;
	
}
void	LedHandler_Class::RedWave(void){
	request=false;
	do
	{
		for ( k=1;k<totalPins;k++)
		{
			if (IsRed(k))
			{
				if (!PowerOnSingleLed(k))
				{
					break;
				}
				
			}
			else
			{
				PowerOffPin(k);
			}
			
		}
	}while((!changeModeRequest));
	request=false;
	
}
void	LedHandler_Class::BlueWave(void){
	request=false;
	do
	{
		for ( k=2;k<totalPins;k++)
		{
			if (IsBlue(k))
			{
				if (!PowerOnSingleLed(k))
				{
					break;
				}
							
			}
			else
			{
				PowerOffPin(k);
			}
			
		}
	}while((!changeModeRequest));
	request=false;
	
}
void	LedHandler_Class::LilaWave(void){
	request=false;
	onLila=true;
	do
	{
		for ( k=2;k<totalPins;k++)
		{
			if (IsBlue(k))
			{
				if (!PowerOnSingleLed(k))
				{
					break;
				}
				k++;
			}
			else
			{
				PowerOffPin(k);
			}
		
			
		}
	}while((!changeModeRequest));
	request=false;
	onLila=false;
	
}
void	LedHandler_Class::MagentaWave(void){
	request=false;
	onMagenta=true;
	do
	{
		for ( k=3;k<totalPins;k++)
		{
			if (IsGreen(k))
			{
				if (!PowerOnSingleLed(k))
				{
					break;
				}
				k++;
			}
			else
			{
				PowerOffPin(k);
			}
			
			
		}
	}while((!changeModeRequest));
	request=false;
	onMagenta=false;
	
}

void	LedHandler_Class::WhiteWave(void){
	request=false;
	onWhite=true;
	do
	{
		for ( k=2;k<totalPins;k++)
		{
			if (IsBlue(k))
			{
				if (!PowerOnSingleLed(k))
				{
					break;
				}
				
			}
			else
			{
				PowerOffPin(k);
			}
			
			
		}
	}while((!changeModeRequest));
	request=false;
	onWhite=false;
	
}
bool	LedHandler_Class::PowerOnSingleLed(int pn){

	if (onLila){
		StartBlinker(1);
		PowerOnPin(pn);
		if (pn<kTOTALLEFTPINS)
		{
			SetPin(pins.allPorts[pn+2],pins.allPins[pn+2]);
			state.status[pn+2]=1;
		}
		else
		{
			SetPin(pins.allPorts[pn-2],pins.allPins[pn-2]);
			state.status[pn-2]=1;
		}
		
		while((!blinkOut)){
			//Dimmen(0);
		}
		PowerOffPin(pn);
		if (pn<kTOTALLEFTPINS)
		{
			ClearPin(pins.allPorts[pn+2],pins.allPins[pn+2]);
			state.status[pn+2]=0;
		}
		else
		{
			ClearPin(pins.allPorts[pn-2],pins.allPins[pn-2]);
			state.status[pn-2]=0;
		}
						
	}else{
		if (onMagenta)
		{
				StartBlinker(1);
				PowerOnPin(pn);
				if (pn<kTOTALLEFTPINS)
				{
						SetPin(pins.allPorts[pn-1],pins.allPins[pn-1]);
						state.status[pn-1]=1;
				} 
				else
				{
					if (pn+1<totalPins)
					{
						SetPin(pins.allPorts[pn+1],pins.allPins[pn+1]);
						state.status[pn+1]=1;
					}
					
				}
				while((!blinkOut)){
					//Dimmen(0);
				}
				PowerOffPin(pn);
				if (pn<kTOTALLEFTPINS)
				{
					ClearPin(pins.allPorts[pn-1],pins.allPins[pn-1]);
					state.status[pn-1]=0;
				}
				else
				{
					if (pn+1<totalPins)
					{
						ClearPin(pins.allPorts[pn+1],pins.allPins[pn+1]);
						state.status[pn+1]=0;
					}
				
				}
		
		} 
		else
		{
			if (onWhite)
			{
				StartBlinker(0);
				PowerOnPin(pn);
				if (pn<kTOTALLEFTPINS)
				{
					SetPin(pins.allPorts[pn+2],pins.allPins[pn+2]);
					state.status[pn+2]=1;
					SetPin(pins.allPorts[pn+1],pins.allPins[pn+1]);
					state.status[pn+1]=1;
				}
				else
				{
					SetPin(pins.allPorts[pn-2],pins.allPins[pn-2]);
					state.status[pn-2]=1;
					SetPin(pins.allPorts[pn-1],pins.allPins[pn-1]);
					state.status[pn-1]=1;
				}
				
				while((!blinkOut)){
					//Dimmen(0);
				}
				PowerOffPin(pn);
				if (pn<kTOTALLEFTPINS)
				{
					ClearPin(pins.allPorts[pn+2],pins.allPins[pn+2]);
					state.status[pn+2]=0;
					ClearPin(pins.allPorts[pn+1],pins.allPins[pn+1]);
					state.status[pn+1]=0;
				}
				else
				{
					ClearPin(pins.allPorts[pn-2],pins.allPins[pn-2]);
					state.status[pn-2]=0;
					ClearPin(pins.allPorts[pn-1],pins.allPins[pn-1]);
					state.status[pn-1]=0;
				}	
			} 
			else
			{
				StartBlinker(0);
				PowerOnPin(pn);
				while((!blinkOut)){
					Dimmen(0);
				}
				PowerOffPin(pn);
			}
		
		}
	}
	

	if (nmi.IsButtonPressed())
	{
		request=true;
		nmi.OnPressed();
		
		}else{
		if (request&&nmi.pressedTimerStarted)
		{
			nmi.OnRelease();
			request=false;
			if (stepTime>tapLimit)
			{
				tapTime=tapLimit;
			} 
			else
			{
				tapTime=stepTime;
			}
			if (nmi.pressCounter<(512*COUNTER_TIME/(tapTime+COUNTER_TIME))){
				changeModeRequest=true;
				return false;
			}else{
			changeModeRequest=false;
				
			}
			
		}
		else
		{
		}
	}
	return true;
}

bool	LedHandler_Class::PowerOnFail(int pn){
	if (IsRed(pn)){
		SimplePowerON(pn);
	}else{
		if (IsRed(pn+1)){
			SimplePowerON(pn+1);
		}else{
			if (IsRed(pn+2)){
				SimplePowerON(pn+2);
			}
		}
		
	}
	if (nmi.IsButtonPressed())
	{
		changeModeRequest=true;
		return false;
	}
	return true;
	
}

void	LedHandler_Class::SimplePowerON(int pn){
	StartBlinker(0);
	PowerOnPin(pn);
	while((!blinkOut)){
		//Dimmen(0);
	}
	PowerOffPin(pn);
}



bool	LedHandler_Class::PowerOnLila(int pn){

	StartBlinker(0);
	PowerOnPin(pn);
	if (pn<kTOTALLEFTPINS)
	{
		SetPin(pins.allPorts[pn+2],pins.allPins[pn+2]);
		state.status[pn+2]=1;
	}
	else
	{
		SetPin(pins.allPorts[pn-2],pins.allPins[pn-2]);
		state.status[pn-2]=1;
	}
		
	while((!blinkOut)){
		//Dimmen(0);
	}
	PowerOffPin(pn);
	if (pn<kTOTALLEFTPINS)
	{
		ClearPin(pins.allPorts[pn+2],pins.allPins[pn+2]);
		state.status[pn+2]=0;
	}
	else
	{
		ClearPin(pins.allPorts[pn-2],pins.allPins[pn-2]);
		state.status[pn-2]=0;
	}
		
	if (nmi.IsButtonPressed())
	{
		changeModeRequest=true;
		return false;
	}
	return true;
}
void	LedHandler_Class::StartBlinker(uint8_t lapsus){
		ledTimer.stop();
		if (lapsus>0)
		{
			ledTimer.Start_oneShot_task((FUNC_PTR)BlinkTimer,2*stepTime);
		} 
		else
		{
			ledTimer.Start_oneShot_task((FUNC_PTR)BlinkTimer,stepTime);
		}
		

		blinkOut=false;
}
	
void	LedHandler_Class::PowerOnPin(int	pn){
		SetPowerOnTimer();
		SetPin(pins.allPorts[pn],pins.allPins[pn]);
		state.status[pn]=1;
		poweron=true;
	}
void	LedHandler_Class::PowerOffPin(int	pn){
	ClearPin(pins.allPorts[pn],pins.allPins[pn]);
	pwm_disable(&PWM_0);
	state.status[pn]=0;
	poweron=false;
}

void	LedHandler_Class::ClearAll(void){
	for (i = 0; i < totalPins; i++)
	{
		ClearPin(pins.allPorts[i],pins.allPins[i]);
		state.status[i]=0;
	}
}
void	LedHandler_Class::ClearAllExcept(int ex){
	for (i = 0; i < totalPins; i++)
	{
		if (i==ex)
		{
		
		}else{
			ClearPin(pins.allPorts[i],pins.allPins[i]);
			state.status[i]=0;
		}
		
	}
}

void	LedHandler_Class::Dimmen(int	modulate){
	if (stepTime>3*POWERON_SINGLE_TIME_MAX/4)
	{
		if (modulate>0)
		{
			//	pwm_set_parameters(&PWM_0, period, dutyCycle);
		}
		if (deadTime)
		{
			if (poweron)
			{
				LightOffActivePins();
				poweron=false;
			}			
		}
		else
		{
			if (!poweron)
			{
				LightOnActivePins();
				//SetPowerOnTimer();
				poweron=true;
				
			}
			
			
		}
	}
	
	
}
void	LedHandler_Class::SetPowerOnTimer(void){
		pwmTimer.stop();
		pwm_enable(&PWM_0);
		//pwmTimer.Start_oneShot_task((FUNC_PTR)PowerOnTimer,int(PWM_WAVE_PERIOD/20));

}
void	LedHandler_Class::SetPowerOffTimer(void){
	pwmTimer.stop();
//	pwmTimer.remove_task();
	pwmTimer.add_oneShot_task((FUNC_PTR)PowerOffTimer,uint32_t(period*(1000-dutyCycle)/1000));
	pwmTimer.start();
}

void	LedHandler_Class::LightOnActivePins(void){
		for (i = 0; i < totalPins; i++)
		{
			if (state.status[i]==1)
			{
				SetPin(pins.allPorts[i],pins.allPins[i]);
			} 
			else
			{
				//ClearPin(pins.allPorts[i],pins.allPins[i]);
				
			}
			
		}
}

void	LedHandler_Class::LightOffActivePins(void){
	for (i = 0; i < totalPins; i++)
	{

			ClearPin(pins.allPorts[i],pins.allPins[i]);

	}
}
void	LedHandler_Class::Modulate(void){
	
}
uint8_t	LedHandler_Class::GetTotalPins(void){
	return	totalPins;
}

LedHandler_Class		lighter;
