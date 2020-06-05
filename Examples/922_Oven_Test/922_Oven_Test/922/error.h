/*--------------------------------------------------------------------------

error.h

Interface
Internal error handling

Autor: Steffen Simon

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

--------------------------------------------------------------------------*/
#ifndef ERROR_H
#define ERROR_H

#include <avr/pgmspace.h>
#include "GainDefinitions.h"

/**
	GAIN specific error codes
*/
enum
	{
	kGAINCabinetTemperatureSensorFailure = kGAINCommonErrorCount,
	kGAINSteamerTemperatureSensorFailure,
	kGAINWaterPressureSensorFailure,
	
	kGAINPrimaryHeaterFailure,
	kGAINSecondaryHeaterFailure,
	
	kGAINErrorFanSpeedOutOfRange,
	kGAINErrorFanFailure,
	kGAINErrorFanMotorOverTemperature,
	kGAINErrorFanControllerOverTemperature,

	kGAINErrorValveMechanicalFailure,
	kGAINErrorNonResetableThermalFuseFailure,
	
	kGAINErrorCount,
	};

extern const errorDescription	GAINErrorDescription[] PROGMEM;

#endif

