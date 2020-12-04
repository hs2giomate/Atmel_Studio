/* 
* LedStateMachine.h
*
* Created: 11/4/2020 3:53:49 PM
* Author: GMateusDP
*/


#ifndef __LEDSTATEMACHINE_H__
#define __LEDSTATEMACHINE_H__
#include "bevMakerHmi.h"
#include "initialise.h"
#include "mainTimer.h"
#include "pscCommand.h"
#include "display.h"
#include "ledDriverIc.h"
#include "buttons.h"
#include "externalFlashMem.h"
#include "manualResetDetect.h"
#include "adc.h"
#include "rs485test.h"
#include "lowPower.h"
#include "proxSensor.h"
#include "quadDecoder.h"
#include "ledDriverIc_Private_pca9955.h"
#include "math.h"
#define PI 3.14159265

enum states
{
	kNotDegined=0,
	kStandbyOff,
	kFilling,
	kHeating,
	kReady,
	kWaitNotWater,
	kWaitOvertemperature,
	kFaultLowWater,
	kFaultOvertemperature,
	kFaultDrainage,
	kNumberStates
	
	
};
class LedStateMachine
{
//variables
public:
protected:
private:
	bool stateChange:1;
		uint32_t oldSec ;
		uint32_t old10mSec ;
		bool stateChanged = false;
		uint8_t indip=0;
		uint8_t RightSide = 10;
		uint8_t LeftSide = 10;
		uint8_t per = 0;
		bool bPattern = 1;
		uint8_t state ,lastState;
		float LEDinten[10];
		uint8_t scrollingSpot;
	//	static 	uint32_t	timedicrete;
		//float	intensityLed=0;
		uint8_t intesityLedDiscrete;

//functions
public:
	LedStateMachine();
	~LedStateMachine();
	bool Init();
	void RunPeriodicTask();
	void StandbyOff();
	void WhiteBarPulsing();
	void LedLeftButtonGreenPulsing(void);
	void Heating(void);
	void Filling(void);
	void	Ready(void);
	void WaitNoWater(void);
	void WaitOverTemperature(void);
	void FaultLowWater(void);
	void FaultOvertemeperature(void);
	void FaultDrainage(void);
	void	RunManual(void);
	void HandleStates(uint8_t index);
	void	RunState(uint8_t index=0);
protected:
private:
	LedStateMachine( const LedStateMachine &c );
	LedStateMachine& operator=( const LedStateMachine &c );
	void WhiteBarIntensity(uint8_t intens);
	void LeftButtonGreenIntensity(uint8_t intens);
	void BlueBarDarkSpotScrolling(void);
	void WaitState(void);

}; //LedStateMachine

#endif //__LEDSTATEMACHINE_H__
