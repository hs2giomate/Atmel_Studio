/* 
* ARINC_Buffer_Class.cpp
*
* Created: 10/9/2020 3:47:30 PM
* Author: GMateusDP
*/


#include "ARINC_Buffer_Class.h"
#include "TemperatureSensors_Class.h"
#include "string.h"


// default constructor
ARINC_Buffer_Class::ARINC_Buffer_Class()
{
} //ARINC_Buffer_Class

// default destructor
ARINC_Buffer_Class::~ARINC_Buffer_Class()
{
} //~ARINC_Buffer_Class

void ARINC_Buffer_Class::SaveTemperature(void){
	octalLabel=Label2Byte(COCKPITS_TEMPERATURE);
	index=GetIndexTXLabelarray(FlipByte(octalLabel),LabelsArrayTX);
	float temperature=temperatures.lastValue;
	data=MakeLabel374(FloatTwoComplement(temperature,15),temperature);
	Uint32FourBytesArray(data,localArray);
	memcpy(transmitBuffer[index],localArray,4);
}