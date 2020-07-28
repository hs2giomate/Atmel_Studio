/* 
* Interfaces_Class.cpp
*
* Created: 7/6/2020 9:05:06 AM
* Author: GMateusDP
*/


#include "Interfaces_Class.h"
#include "CDC_Class.h"

// default constructor
Interfaces_Class::Interfaces_Class()
{
} //Interfaces_Class

// default destructor
Interfaces_Class::~Interfaces_Class()
{
} //~Interfaces_Class

uint32_t Interfaces_Class::CheckCommunication(void)
{
	uint32_t	r;
	uint8_t comBuffer[64];
	if (usb.rxReady)
	{
		event	e;
		r=usb.readData(comBuffer, sizeof(comBuffer));
		listener.SendEventSelf(e);
	}
	return r;
}

uint32_t	Interfaces_Class::GetStatus(HVACStatus& s){
	s=status;
	uint32_t	*p=(uint32_t*)(&s.statusBits);
	for (int i=0;i<sizeof(status.statusBits);i++)
	{
		s.rawStatus+=(*p)&(0x01<<i);
		p++;
	}
	return	status.rawStatus;
}

Interfaces_Class	communicator;