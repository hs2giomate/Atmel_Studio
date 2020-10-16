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
		controlStatus.cabin[ii]._switch=(v&&(0x80>>ii))>0;
	}
}
void FlapperValveDataStruct::FillLimitSwitch(uint8_t v){
	for (uint8_t ii = 0; ii < 3; ii++)
	{
		controlStatus.limit[ii]._switch=(v&&(0x20>>ii))>0;
	}
}
ControlStatus FlapperValveDataStruct::FillControlStatus(uint8_t v){
	FillCabinSwitch(v);
	FillLimitSwitch(v);
	controlStatus.niAlcFVMotorFault=(v&&(0x01)>0);
	return controlStatus;
}

StatusInputs FlapperValveDataStruct::FillStatusInputs(uint8_t v){
	statusInputs.iAlcFvStandAloneOut=(v&&(0x80)>0);
	statusInputs.niAlcFvClearMoveFault=(v&&(0x40)>0);
	statusInputs.iFvMotorPhaset=(v&&(0x20)>0);
	statusInputs.iAlcFvMoveTimer=(v&&(0x10)>0);
	statusInputs.iAlcFvPositionChanged=(v&&(0x08)>0);
	statusInputs.iAlcFvInvalidPosition=(v&&(0x04)>0);
	statusInputs.iAlcFvFailsToMove=(v&&(0x2)>0);
	statusInputs.niAlcFvMotorEnable=(v&&(0x1)>0);
	return statusInputs;
}