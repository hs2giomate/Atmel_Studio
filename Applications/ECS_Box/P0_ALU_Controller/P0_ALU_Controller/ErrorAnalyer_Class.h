/* 
* ErrorAnalyer_Class.h
*
* Created: 7/3/2020 1:36:34 PM
* Author: GMateusDP
*/


#ifndef __ERRORANALYER_CLASS_H__
#define __ERRORANALYER_CLASS_H__

#include "GainDefinitions.h"

/**
	GAIN specific error codes
*/
enum
	{
	kGAINErrorMagnetronTemperatureSensorFailure = kGAINCommonErrorCount,
	kGAINErrorExhaustTemperatureSensorFailure,
	kGAINErrorSMPSTemperatureSensorFailure,
	kGAINErrorPEMFailure,
	kGAINErrorMagnetronFanFailure,

	kGAINErrorMagnetronTemperatureOutOfRange,
	kGAINErrorExhaustTemperatureOutOfRange,
	kGAINErrorSMPSTemperatureOutOfRange,
	kGAINErrorCouldNotSetTargetVoltage,
	kGAINErrorSMPSInternalTemperatureOutOfRange,
	kGAINErrorCouldNotSetTargetPDC,
	kGAINErrorEmptyCavity,
	kGAINErrorInsufficientSMPSCooling,
	kGAINErrorCount,
	};

extern const errorDescription	GAINErrorDescription[];


class ErrorAnalyer_Class
{
//variables
public:
protected:
private:

//functions
public:
	ErrorAnalyer_Class();
	~ErrorAnalyer_Class();
protected:
private:
	ErrorAnalyer_Class( const ErrorAnalyer_Class &c );
	ErrorAnalyer_Class& operator=( const ErrorAnalyer_Class &c );

}; //ErrorAnalyer_Class

#endif //__ERRORANALYER_CLASS_H__
