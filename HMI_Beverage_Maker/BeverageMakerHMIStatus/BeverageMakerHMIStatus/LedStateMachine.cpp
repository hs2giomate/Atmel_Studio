/* 
* LedStateMachine.cpp
*
* Created: 11/4/2020 3:53:48 PM
* Author: GMateusDP
*/


#include "LedStateMachine.h"

// default constructor

LedStateMachine::LedStateMachine()
{
	scrollingSpot=2;
	//timedicrete=0;
} //LedStateMachine

// default destructor
LedStateMachine::~LedStateMachine()
{
} //~LedStateMachine

//uint32_t	LedStateMachine::timedicrete;
 static uint32_t timedicrete;
 static uint8_t ledIndexProgressBar=0;
 static uint8_t stateIndex=0;

bool LedStateMachine::Init(){
	oldSec = gdwSecondsAlive;
	systemInitialisation();
}
void LedStateMachine::RunPeriodicTask(){
		__builtin_avr_wdr();	/* Watchdog Timer Reset */
		
		mainTimerService();		/* Update general timers / counters */
		btnManageButtonScan();	// Manage button scanning / debounce / reporting 
		manageManualResetDetect();
// 
// 		esfHALTtestManage();
// 
 	//	pscCommandManager();	/* handle any message received from the PSC */

		ledManager();
}

void LedStateMachine::StandbyOff(){
	
	
			if (gc100msTick != oldSec)
			{
	
				oldSec=gc100msTick;
		
					//pscHALTprogressBarStep(3);
					for (uint8_t step=0; step < 20; step++)
					{
							ledSetProgressSegmentColour(step,Full, 0,0, 255);
					
						
					}
					//ledManager();
					ledSetAllButtonColour(0,0,255);
					if (stateChange)
					{
						ledSetAllIndicatorColour(0,0,0);
						stateChange=false;
					}
					//ledManager();
			}
		
}

void LedStateMachine::WhiteBarIntensity(uint8_t intens){
	
		//pscHALTprogressBarStep(3);
		for (uint8_t step=0; step < 20; step++)
		{
			ledSetProgressSegmentColour(step,intens, 81,81, 81);
					
		}

	
	
}

void LedStateMachine::WhiteBarPulsing(void){
		
// 	if (gc100msTick != old10mSec)
// 	{
	
		float	intensityLed=127*sin((timedicrete%360)*PI/(180*1))+128;
		uint32_t floattoInt=(uint32_t)intensityLed;
		intesityLedDiscrete=(uint8_t)(floattoInt&0xff);
		WhiteBarIntensity(intesityLedDiscrete);
		timedicrete++;
//	}
		
}

void LedStateMachine::LeftButtonGreenIntensity(uint8_t intens){
	
	ledSetLeftButtonColourInt(intens,0,81,0);
	
}
void LedStateMachine::LedLeftButtonGreenPulsing(void){
	
			
			float	intensityLed=127*sin((timedicrete%360)*PI/(180*1))+128;
			uint32_t floattoInt=(uint32_t)intensityLed;
			intesityLedDiscrete=(uint8_t)(floattoInt&0xff);
			LeftButtonGreenIntensity(intesityLedDiscrete);
			timedicrete++;
		//}
}
void LedStateMachine::Heating(void){
		if (gc100msTick != old10mSec)
		{
			old10mSec=gc100msTick;
	WhiteBarPulsing();
 	timedicrete--;
	if (stateChange)
	{
		ledSetAllIndicatorColour(0,0,0);
		stateChange=false;
	}
 	LedLeftButtonGreenPulsing();
		}
	
}

void LedStateMachine::BlueBarDarkSpotScrolling(void){
	
	if (gc1sTick != oldSec)
	{
		
		 oldSec=gc1sTick;
		if (ledIndexProgressBar<1)
		{
			ledSetProgressSegmentColour(ledIndexProgressBar,0, 0,0, 0);
			ledSetProgressSegmentColour(ledIndexProgressBar+1,128, 0,0, 255);
			ledSetProgressSegmentColour(ledIndexProgressBar+2,255, 0,0, 255);
		} 
		else
		{
			if (ledIndexProgressBar<2)
			{
					ledSetProgressSegmentColour(ledIndexProgressBar-1,128, 0,0, 255);
					ledSetProgressSegmentColour(ledIndexProgressBar,0, 0,0, 0);
					ledSetProgressSegmentColour(ledIndexProgressBar+1,128, 0,0, 255);
					ledSetProgressSegmentColour(ledIndexProgressBar+2,255, 0,0, 255);
			} 
			else
			{
				if (ledIndexProgressBar<17)
				{
					ledSetProgressSegmentColour(ledIndexProgressBar-2,255, 0,0, 255);
					ledSetProgressSegmentColour(ledIndexProgressBar-1,128, 0,0, 255);
					ledSetProgressSegmentColour(ledIndexProgressBar,0, 0,0, 0);
					ledSetProgressSegmentColour(ledIndexProgressBar+1,128, 0,0, 255);
					ledSetProgressSegmentColour(ledIndexProgressBar+2,255, 0,0, 255);
				}
				else
				{
					if (ledIndexProgressBar<18)
					{
						ledSetProgressSegmentColour(ledIndexProgressBar-2,255, 0,0, 255);
						ledSetProgressSegmentColour(ledIndexProgressBar-1,128, 0,0, 255);
						ledSetProgressSegmentColour(ledIndexProgressBar,0, 0,0, 0);
						ledSetProgressSegmentColour(ledIndexProgressBar+1,128, 0,0, 255);
					}
					else
					{
						ledSetProgressSegmentColour(ledIndexProgressBar-2,255, 0,0, 255);
						ledSetProgressSegmentColour(ledIndexProgressBar-1,128, 0,0, 255);
						ledSetProgressSegmentColour(ledIndexProgressBar,0, 0,0, 0);
					}
				}
			}
		}
		
			
	
		
	/*	}*/
		ledIndexProgressBar++;
		if (ledIndexProgressBar>19)
		{
			ledIndexProgressBar=0;
		}


	}
}

void LedStateMachine::Filling(void){
	BlueBarDarkSpotScrolling();
	ledSetLeftButtonColourInt(255,0,0,255);
	ledSetAllIndicatorColour(0,0,0);
}

void	LedStateMachine::Ready(void){
	if (gc100msTick != old10mSec)
	{
		old10mSec=gc100msTick;
		ledSetLeftButtonColourInt(255,0,255,0);
		for (uint8_t step=0; step < 20; step++)
		{
			ledSetProgressSegmentColour(step,255, 0,81, 0);
			
		}
	}
}

void LedStateMachine::WaitNoWater(void){

	WaitState();
	if (stateChange)
	{
		ledSetAllIndicatorColour(0,0,0);
		stateChange=false;
	}
	ledSetIndicatorColour(1, 81, 81, 81);			
}

void LedStateMachine::WaitOverTemperature(void){

	WaitState();
	if (stateChange)
	{
		ledSetAllIndicatorColour(0,0,0);
		stateChange=false;
	}
	ledSetIndicatorColour(3, 81, 40, 40);
}

void LedStateMachine::WaitState(void){

	if (gc1sTick != oldSec)
	{
		
		oldSec=gc1sTick;
		if (ledIndexProgressBar<1)
		{
			ledSetProgressSegmentColour(18,0, 0,0, 0);
			ledSetProgressSegmentColour(19,0, 0,0, 0);
			ledSetProgressSegmentColour(ledIndexProgressBar,255, 81,81, 81);
			ledSetProgressSegmentColour(ledIndexProgressBar+1,128,  81,81, 81);
			
		}
		else
		{
			if (ledIndexProgressBar<2)
			{
				ledSetProgressSegmentColour(ledIndexProgressBar-1,128, 81,81, 81);
				ledSetProgressSegmentColour(ledIndexProgressBar,255, 81,81, 81);
				ledSetProgressSegmentColour(ledIndexProgressBar+1,128,  81,81, 81);
				
			}
			else
			{
				if (ledIndexProgressBar<20)
				{
					ledSetProgressSegmentColour(ledIndexProgressBar-2,0, 0,0, 0);
					ledSetProgressSegmentColour(ledIndexProgressBar-1,128, 81,81, 81);
					ledSetProgressSegmentColour(ledIndexProgressBar,255, 81,81, 81);
					ledSetProgressSegmentColour(ledIndexProgressBar+1,128,  81,81, 81);
					
				}
				else
				{
					
				}
			}
			
		}
		ledIndexProgressBar++;
		if (ledIndexProgressBar>19)
		{
			ledIndexProgressBar=0;
		}
		
		}else{
		if (gc100msTick != old10mSec)
		{
			old10mSec=gc100msTick;
			LedLeftButtonGreenPulsing();
			//	ledManager();
			//	return;
			
		}
	}

	

}

void LedStateMachine::FaultLowWater(void){
	if (stateChange)
	{
		ledSetAllIndicatorColour(0,0,0);
		stateChange=false;
	}
	ledSetIndicatorColour(1, 81, 81, 81);
	ledSetLeftButtonColourInt(255,81,60, 8);
	for (uint8_t step=0; step < 20; step++)
	{
		ledSetProgressSegmentColour(step,255, 81,60, 8);
		
	}
}
void LedStateMachine::FaultOvertemeperature(void){
	if (stateChange)
	{
		 ledSetAllIndicatorColour(0,0,0);
		 stateChange=false;
	}
	
	ledSetIndicatorColour(3, 81, 40, 40);
	ledSetLeftButtonColourInt(255,81,60, 8);
	for (uint8_t step=0; step < 20; step++)
	{
		ledSetProgressSegmentColour(step,255, 81,60, 8);
		
	}
}
void LedStateMachine::FaultDrainage(void){
	 if (stateChange)
	 {
		 ledSetAllIndicatorColour(0,0,0);
		 stateChange=false;
	 }
	ledSetIndicatorColour(2, 40, 40, 81);
	ledSetLeftButtonColourInt(255,81,60, 8);
	for (uint8_t step=0; step < 20; step++)
	{
		ledSetProgressSegmentColour(step,255, 81,60, 8);
		
	}
}

void LedStateMachine::HandleStates(uint8_t index){
	switch (index)
	{
		case kStandbyOff:
		/* Your code here */
			StandbyOff();
		break;
		case kFilling:
			Filling();
			break;
		case kHeating:
			Heating();
			break;
		case kReady:
			Ready();
			break;
		case kWaitNotWater:
			WaitNoWater();
			break;
		case kWaitOvertemperature:
			WaitOverTemperature();
			break;
		case kFaultLowWater:
			FaultLowWater();
			break;
		case kFaultOvertemperature:
			FaultOvertemeperature();
			break;
		case kFaultDrainage:
			FaultDrainage();
			break;
		default:
		/* Your code here */
			WaitNoWater();
		break;
	}
}

void	LedStateMachine::RunManual(void){
	while (1)
	{
		RunPeriodicTask();
		if (gButtonStates_Array[1].m_bActive_Confirmed==true)
		{
			stateIndex++;
			while (gButtonStates_Array[1].m_bUnconfirmedState_Active==true)
			{
				btnManageButtonScan();	
			}
			asm("nop");
			stateChange=true;
		}
		if (stateIndex>=kNumberStates)
		{
			stateIndex=1;
		}
		HandleStates(stateIndex);
	}

	
}
void	LedStateMachine::RunState(uint8_t index ){


			if (state!=index)
			{
				lastState=state;
				state=index;
				stateChange=true;
				if (state>=kNumberStates)
				{
					stateIndex=1;
				}
				
			}
			HandleStates(state);
			RunPeriodicTask();


// 		
// 		if (gButtonStates_Array[1].m_bActive_Confirmed==true)
// 		{
// 			state++;
// 			while (gButtonStates_Array[1].m_bUnconfirmedState_Active==true)
// 			{
// 				btnManageButtonScan();
// 			}
// 			asm("nop");
// 			
// 		}
		
	
		

	
}


	


