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
#include "I2C_Asyn_Class.h"
#define	 FLAPPER_VALVE_EXPANDERS 4
#define IC_CONTROL_STATUS 00
#define IC_STATUS_INPUTS 01
#define IC_SETPOINT_POSITION 02
#define IC_ACTUAL_POSITION 03

class FlapperValve_Class: public FlapperValveDataStruct
{
//variables
public:
	volatile bool isOK,fv1StatusChanged,fv2StatusChanged;
	I2C_Asyn_Class* i2cClass;
	FVDataStruct	fvData;
protected:
private:
	 i2c_m_async_desc*	i2cDescr;
	 uint8_t	valveID;
	MCP23008_Class expanders[4];
	MCP23008_Class* currentExpander;
	ControlStatus	controlStatus;
	StatusInputs	statusInputs;
	
	uint8_t i,j,k,value,setpointPosition,actualPosition;
//functions
public:
	FlapperValve_Class();
	FlapperValve_Class(uint8_t);
	~FlapperValve_Class();
	bool	Init();
	void UpdateFlapperValveData();
	ControlStatus ReadControlStatus(void);
	StatusInputs ReadStatusInputs(void);
	uint8_t WriteSetpoint(uint8_t sp);
	uint8_t ReadActualPosition(void);
	operator	bool();
protected:
private:
	FlapperValve_Class( const FlapperValve_Class &c );
	FlapperValve_Class& operator=( const FlapperValve_Class &c );
	uint8_t InitExpanderArray(uint8_t fvID);
	bool Selftest(void);
	uint8_t ReadSetpoint();
}; //FlapperValve_Class
extern	FlapperValve_Class	fv1;
// static FlapperValve_Class	fv1(1);

#endif //__FLAPPERVALVE_CLASS_H__
