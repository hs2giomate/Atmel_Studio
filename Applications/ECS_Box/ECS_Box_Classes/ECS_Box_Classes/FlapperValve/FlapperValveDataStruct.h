/* 
* FlapperValveDataStruct.h
*
* Created: 10/2/2020 5:19:02 PM
* Author: GMateusDP
*/


#ifndef __FLAPPERVALVEDATASTRUCT_H__
#define __FLAPPERVALVEDATASTRUCT_H__
#include "compiler.h"

struct __attribute__((__packed__)) InputStatus
{
	bool	cabin[2];
	bool	limit[3];
	bool	niAlcFVMotorFault;
};
struct __attribute__((__packed__)) ControlStatus
{
	bool	iAlcFvStandAloneOut;
	bool	niAlcFvClearMoveFault;
	bool	iAlcFvMoveTimer;
	bool	iFvMotorPhaset;
	bool	iAlcFvPositionChanged;
	bool	iAlcFvInvalidPosition;
	bool	iAlcFvFailsToMove;
	bool	niAlcFvMotorEnable;
};
struct __attribute__((__packed__)) FlapperValveControllerStatus
{
	bool	isMoving;
	bool	isControlling;
	bool	NBC_Mode;
	uint8_t	minimunAir;

};

struct __attribute__((__packed__)) FVDataStruct
{
	
	InputStatus	inputStatus;
	ControlStatus	controlOutputs;
	uint8_t			actualPosition;
	uint8_t			setPointPosition;
	FlapperValveControllerStatus	controllerStatus;
};
enum StatusFlapperValve
{
	FRESHAIR=0,
	INTERM,
	RECYCLE,
	NBC,
};

#define STANDALONE 1

class FlapperValveDataStruct
{
//variables
public:
	FVDataStruct	fvDataStruct;
	InputStatus FillInputStatus(uint8_t v);
	ControlStatus FillControlOutputs(uint8_t v);
	
protected:
private:
	InputStatus	controlStatus;
	ControlStatus	controlOutputs;
//functions
public:
	FlapperValveDataStruct();
	~FlapperValveDataStruct();
protected:
private:
	FlapperValveDataStruct( const FlapperValveDataStruct &c );
	FlapperValveDataStruct& operator=( const FlapperValveDataStruct &c );
	void FillLimitSwitch(uint8_t v);
	void FillCabinSwitch(uint8_t v);
}; //FlapperValveDataStruct

#endif //__FLAPPERVALVEDATASTRUCT_H__
