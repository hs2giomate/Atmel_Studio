/* 
* FlapperValveDataStruct.h
*
* Created: 10/2/2020 5:19:02 PM
* Author: GMateusDP
*/


#ifndef __FLAPPERVALVEDATASTRUCT_H__
#define __FLAPPERVALVEDATASTRUCT_H__
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
protected:
private:

//functions
public:
	FlapperValveDataStruct();
	~FlapperValveDataStruct();
protected:
private:
	FlapperValveDataStruct( const FlapperValveDataStruct &c );
	FlapperValveDataStruct& operator=( const FlapperValveDataStruct &c );

}; //FlapperValveDataStruct

#endif //__FLAPPERVALVEDATASTRUCT_H__
