/* 
* Interfaces_Class.h
*
* Created: 7/6/2020 9:05:06 AM
* Author: GMateusDP
*/


#ifndef __INTERFACES_CLASS_H__
#define __INTERFACES_CLASS_H__
#include "coreTypes.h"
#include "EventHandler_Class.h"
#include "ConfigState_Class.h"

struct InternCommunicationRequest
{

	bool temperatureSensorsMessage;
	bool flapperValvesMessage;
	
};
struct ExternCommunicationRequest
{
	bool arinc1GotMessage;
	bool arinc2GotMessage;
	bool usbGotMessage;

	
};

struct CommunicationRequest
{
	InternCommunicationRequest	internRequest;
	ExternCommunicationRequest	externRequest;
	
};

class Interfaces_Class
{
//variables
public:
	volatile bool isOK,isMaintenanceActivated;
protected:
private:
	HVACState	hvacState;
	HVACStatus	status;
	uint8_t		checkResult,i;
	uint32_t result;
	static CommunicationRequest request;
	uint8_t	size,j,k;
	float lastTemperature;
		
	
//functions
public:
	Interfaces_Class();
	~Interfaces_Class();
	bool Init(void);
	CommunicationRequest	CheckCommunication(void);
	uint32_t	CheckInternalCommunication(void);
	uint32_t	CheckExternalCommunication(void);
	uint32_t	GetStatus(HVACStatus&);
	bool	CheckFlapperValveInterface(uint8_t add);
	bool	CheckTemperatures(void);
	bool CheckUSBInterface(void);
	bool	CheckArincInterface(void);
	bool	IsCommunicationRequest();
protected:
private:
	Interfaces_Class( const Interfaces_Class &c );
	Interfaces_Class& operator=( const Interfaces_Class &c );

}; //Interfaces_Class

extern 	Interfaces_Class	interfaces;
//static 	Interfaces_Class	interfaces;
#endif //__INTERFACES_CLASS_H__
