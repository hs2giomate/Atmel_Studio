/* 
* LedDriverClass.h
*
* Created: 7/15/2020 2:07:09 PM
* Author: GMateusDP
*/


#ifndef __LEDDRIVERCLASS_H__
#define __LEDDRIVERCLASS_H__

#include "LEDLighting_Class.h"
#include "hpl_tc_config.h"
#define INITIAL_STATE	0
#define		NUMBER_STATES	6
#define		POWERON_SINGLE_TIME	4
#define		POWEROFF_SINGLE_TIME	1000
#define		PWM_WAVE_PERIOD  CONF_TC3_WAVE_PER_VAL 
#define		PWM_WAVE_DUTY  CONF_TC3_WAVE_DUTY_VAL
// #define		PWM_WAVE_PERIOD  1000000
// #define		PWM_WAVE_DUTY  500
#define		POWERON_TIME	50
#define		POWER_PERIOD	50
#define		POWEROFF_TIMR	50



class LedHandler_Class: public	LEDLighting_Class
{
//variables
public:
	volatile bool shutDown,restarted,poweron,deadTime,changeModeRequest,lightOn,ready,blinkOut;
	volatile uint32_t	dutyCycle,stepTime;
	bool request,onLila,onMagenta, onWhite;
protected:
private:
	uint8_t	modus;
	uint32_t	dimer,tapTime,tapLimit;
	
	
	BothSidePins	pins;
	StateLeds	state;
	uint8_t		totalPins;
	int i,j,k,m,n;
	uint32_t	period;	
	

//functions
public:
	LedHandler_Class();
	~LedHandler_Class();
	void	Init(void);
	void	Run(void);
	void	ActivateAllGreen(void);
	void	GreenWave(void);
	void	RedWave(void);
	void	BlueWave(void);
	void	LilaWave(void);
	void	MagentaWave(void);
	void	WhiteWave(void);
	void	ClearAll(void);
	void	ClearAllExcept(int);
	void	PowerOffPin(int);
	void	Dimmen(void);
	void	RunOnMode(void);
	void	Modulate(void);
	friend	void EVSYS_Handler(void);
	void	Dimmen(int	modulate=0);
	uint8_t GetTotalPins(void);
	void	RestartLedModus(void);
	bool	PowerOnSingleLed(int);
	bool	PowerOnLila(int pn);
	bool	PowerOnFail(int pn);

protected:
private:
	LedHandler_Class( const LedHandler_Class &c );
	LedHandler_Class& operator=( const LedHandler_Class &c );
	void	StartBlinker(uint8_t);
	void	PowerOnPin(int);
	void	SimplePowerON(int);
	void	SetPowerOnTimer(void);
	void	SetPowerOffTimer(void);
	void	ChangeModus(void);
	void	LightOnActivePins(void);
	void	LightOffActivePins(void);

}; //LedDriverClass
extern	LedHandler_Class		lighter;
#endif //__LEDDRIVERCLASS_H__
