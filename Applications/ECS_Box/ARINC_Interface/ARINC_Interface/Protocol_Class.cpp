/* 
* Protocol_Class.cpp
*
* Created: 6/19/2020 5:54:35 PM
* Author: GMateusDP
*/


#include "Protocol_Class.h"
#include "stdlib.h"
#include "math.h"

// virtual destructor
Protocol_Class::~Protocol_Class()
{
} //~Protocol_Class

uint8_t	Protocol_Class::GetActiveLine(void){
	uint32_t	data=ReadBufferLabel(Label2Byte(270));
	uint8_t	key=uint8_t((data&0xc0000)>>18);

	return key;
}

uint32_t Protocol_Class::GetDataField(uint32_t label){
	uint32_t	data;
	uint8_t lhex=Label2Byte(label);
	datagram.bit.label=lhex;
	uint32_t d=ReadBufferLabel(lhex);
	data=(0x1ffffc00&d)>>10;
	datagram.bit.dataField=data;

	return	data;
}
uint32_t Protocol_Class::MakeLabel374(uint32_t d,float v){
	value=d<<12;
	label=Label2Byte(374);
	value|=(uint32_t)label;
	if (v<0)
	{
		value|=0x1<<28;
	}
	value=MakeEvenParity(value);
	return value;
	
}
uint32_t Protocol_Class::MakeTXLabel275(uint8_t d){
	value=((uint32_t)d)<<10;
	label=Label2Byte(275);
	value|=(uint32_t)label;

	value=MakeEvenParity(value);
	return value;
	
}
uint32_t Protocol_Class::MakeTXLabel375(uint8_t d){
	value=((uint32_t)d)<<10;
	label=Label2Byte(375);
	value|=(uint32_t)label;

	value=MakeEvenParity(value);
	return value;
	
}
uint32_t Protocol_Class::MakeLabel270(uint32_t d){
	
	value=d<<10;
	label=Label2Byte(270);
	value|=(uint32_t)label;
//	value|=(uint32_t)(((uint8_t)WavingValue()))<<19;
	value=MakeEvenParity(value);
	return value;
	
}
uint32_t Protocol_Class::MakeEvenParity(uint32_t data){
	if (FindParity(data))
	{
		data|=0x01<<31;
	} 
	else
	{
		
	}
	return data;
}
uint8_t Protocol_Class::WavingValue(void){
	testByte=int(90*sin(counter*3.141516/(180*200)))+90;
	counter++;
	return testByte;
}