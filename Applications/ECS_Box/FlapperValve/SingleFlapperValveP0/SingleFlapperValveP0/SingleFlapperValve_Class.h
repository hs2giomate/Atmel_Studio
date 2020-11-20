/* 
* FlapperValve_Class.h
*
* Created: 7/31/2020 3:45:11 PM
* Author: GMateusDP
*/


#ifndef __FLAPPERVALVE_CLASS_H__
#define __FLAPPERVALVE_CLASS_H__
#include "FlapperValveDataStruct.h"
#include "MCP23008_Class.h"

#include "MCP23017_Class.h"
#define	 FLAPPER_VALVE_EXPANDERS 4
#define IC_CONTROL_STATUS 00
#define IC_STATUS_INPUTS 01
#define IC_SETPOINT_POSITION 02
#define IC_ACTUAL_POSITION 03
// #ifdef SELF_STANDALONE
//
// #else
// 	#define STANDALONE 0
// #endif // SELF_STANDALONE



class SingleFlapperValve_Class: private FlapperValveDataStruct
{
//variables
public:
	volatile bool isOK,fv1StatusChanged,regulatorTimeout;
	//I2C_Asyn_Class* i2cClass;
	FVDataStruct	fvData;
	StatusFlapperValve statusFlapperValve;
	ControlStatus	controlOutputs;
	InputStatus  statusInputs;
 
protected:
private:
	I2C_Sync_Class*	i2c;
	 i2c_m_async_desc*	i2cDescr;
	 uint8_t	valveID,offset;
	MCP23008_Class* expanders[FLAPPER_VALVE_EXPANDERS ];
	MCP23008_Class* currentExpander;
	MCP23017_Class*	simpleExpander;

	
	uint8_t i,j,k,value,setpointPosition,actualPosition,lastPosition;
//functions
public:
	SingleFlapperValve_Class();
	SingleFlapperValve_Class(uint8_t _iD);
	~SingleFlapperValve_Class();
	bool	Init(void);
	void UpdateFlapperValveData();
	ControlStatus ReadControlStatus(void);
	InputStatus ReadStatusInputs(void);
	uint8_t WriteSetpoint(uint8_t sp);
	uint8_t ReadActualPosition(void);
	operator	bool();
	uint8_t GetCurrentAngle(void);
	bool IsPositionChanged(void);
	uint8_t SetOffsetPosition(void);
	uint8_t SetVentilatePosition(void);
	uint8_t WriteControlStatus(uint8_t controlByte);
	uint8_t SetEnable(bool b);
	uint8_t ClearMoveFault(bool b);
	uint8_t SetDirection(bool b);
	
protected:
private:
	SingleFlapperValve_Class( const SingleFlapperValve_Class &c );
	SingleFlapperValve_Class& operator=( const SingleFlapperValve_Class &c );
	uint8_t InitExpanderArray(uint8_t fvID);
	bool Selftest(void);
	uint8_t ReadSetpoint();
}; //FlapperValve_Class
extern	SingleFlapperValve_Class	fv1;
// static FlapperValve_Class	fv1(1);

#endif //__FLAPPERVALVE_CLASS_H__
