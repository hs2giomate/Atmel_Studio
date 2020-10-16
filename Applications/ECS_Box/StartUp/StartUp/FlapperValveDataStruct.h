/* 
* FlapperValveDataStruct.h
*
* Created: 10/2/2020 5:19:02 PM
* Author: GMateusDP
*/


#ifndef __FLAPPERVALVEDATASTRUCT_H__
#define __FLAPPERVALVEDATASTRUCT_H__
#include "compiler.h"
struct CabinSwitch
{
	bool	_switch;
};

struct LimitSwitch
{
	bool	_switch;
};
struct ControlStatus
{
	CabinSwitch	cabin[2];
	LimitSwitch	limit[3];
	bool	niAlcFVMotorFault;
};
struct StatusInputs
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

struct FVDataStruct
{
	ControlStatus	controlStatus;
	StatusInputs	statusInputs;
	uint8_t			setPointPosition;
	uint8_t			actualPosition;
};
class FlapperValveDataStruct
{
//variables
public:
	FVDataStruct	fvDataStruct;
	ControlStatus FillControlStatus(uint8_t v);
	StatusInputs FillStatusInputs(uint8_t v);
	
protected:
private:
	uint8_t	i,j,k;
	ControlStatus	controlStatus;
	StatusInputs	statusInputs;
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
