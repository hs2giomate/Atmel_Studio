/* 
* PBIT_Class.cpp
*
* Created: 6/22/2020 6:21:14 PM
* Author: GMateusDP
*/


#include "PBIT_Class.h"
#include "Interfaces_Class.h"

// default constructor
PBIT_Class::PBIT_Class()
{
} //PBIT_Class

// default destructor
PBIT_Class::~PBIT_Class()
{
} //~PBIT_Class

uint8_t	PBIT_Class::GetStatus(void){
	uint8_t	s=0;
	label371=arinc.ReadBufferLabel(int(371));
	datafield=0;
	datafield=arinc.GetDataField(label371);

	for (uint8_t i=0;i<32;i++)
	{
		s+=(uint8_t)(datafield>>i)&0x01;
	}
	return	s;
}	

bool	PBIT_Class::ISPBITOK(){
	
	if (GetStatus()>0)
	{
		isOK=false;
	} 
	else
	{
		isOK=true;
	}
	return isOK;
}

PBIT_Class::operator bool(){
	return	isOK;
}

uint32_t  PBIT_Class::CheckCurrentStatus(HVACStatus& cs){
	interfaces.GetStatus(cs);
	uint32_t	*s=(uint32_t*)&cs.statusBits;
	uint32_t	v=*s;

	for (int i=0;i<32;i++)
	{
		v+=(0x01<<i)&(v);
		
	}
	if (v==0)
	{
		isOK=true;
	}
	return	v;
}

PBIT_Class	pBIT;