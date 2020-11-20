/* 
* FlapperValveDataStruct.cpp
*
* Created: 10/2/2020 5:19:01 PM
* Author: GMateusDP
*/


#include "FlapperValveDataStruct.h"

// default constructor
FlapperValveDataStruct::FlapperValveDataStruct()
{
} //FlapperValveDataStruct

// default destructor
FlapperValveDataStruct::~FlapperValveDataStruct()
{
} //~FlapperValveDataStruct

void FlapperValveDataStruct::FillCabinSwitch(uint8_t v){
	for (uint8_t ii = 0; ii < 2; ii++)
	{
		controlStatus.cabin[ii]=(v&(0x80>>ii))>0;
	}
}
void FlapperValveDataStruct::FillLimitSwitch(uint8_t v){
	for (uint8_t ii = 0; ii < 3; ii++)
	{
		controlStatus.limit[2-ii]=(v&(0x20>>ii))>0;
	}
}
InputStatus FlapperValveDataStruct::FillInputStatus(uint8_t v){
	FillCabinSwitch(v);
	FillLimitSwitch(v);
	controlStatus.niAlcFVMotorFault=((v&(0x01))>0);
	return controlStatus;
}


ControlStatus FlapperValveDataStruct::FillControlOutputs(uint8_t v){
	
	if ( STANDALONE > 0)
	{
		controlOutputs.iAlcFvStandAloneOut=controlStatus.cabin[0];
	} 
	else
	{
		controlOutputs.iAlcFvStandAloneOut=((v&(0x80))>0);
	}

// 	#else
// 		controlOutputs.iAlcFvStandAloneOut=((v&(0x80))>0);
// 	#endif
	
	controlOutputs.niAlcFvClearMoveFault=((v&(0x40))>0);
	controlOutputs.iFvMotorPhaset=((v&(0x20))>0);
	controlOutputs.iAlcFvMoveTimer=((v&(0x10))>0);
	controlOutputs.iAlcFvPositionChanged=((v&(0x08))>0);
	controlOutputs.iAlcFvInvalidPosition=((v&(0x04))>0);
	controlOutputs.iAlcFvFailsToMove=((v&(0x2))>0);
	controlOutputs.niAlcFvMotorEnable=((v&(0x1))>0);
	return controlOutputs;
}