/*--------------------------------------------------------------------------

error.cc

Implementation
Internal error handling

Autor: Steffen Simon

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/922/error.c $

--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT
#include "coreNSDDiagnostics.h"
#endif

#include "error.h"

/** \brief Error Text Strings: layerErrorDescription ovenErrorDescription
 */
const errorDescription	GAINErrorDescription[] PROGMEM =
	{
		{
		kGAINNoError,
	#ifdef NSD_SUPPORT
		kNSDDiagNoError,
	#else
		0,
	#endif
		},		
		{
		kGAINGenericError|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagGeneralSoftwareFailure,
	#else
		0,
	#endif
		},
		{
		kGAINErrorIllegalStateTransition|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagGeneralSoftwareFailure,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorPinProgramming|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagPinProgrammingParityError,
	#else
		0,
	#endif
		},		

		{
		kGAINErrorSafetyRelayFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagHardware,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorPinCodingBoardFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagHardware,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorHMIFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagDisplay,
	#else
		0,
	#endif
		},		

		{
		kGAINErrorPhaseLost|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagPhaseLoss,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorControllerTemperatureOutOfRange,
	#ifdef NSD_SUPPORT
		kNSDDiagTemporaryFault,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorAmbientTemperatureOutOfRange,
	#ifdef NSD_SUPPORT
		kNSDDiagTemporaryFault,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorAmbientSensorFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagValve1Sensor1,
	#else
		0,
	#endif
		},

	//	GAIN specific errors		
		{
		kGAINCabinetTemperatureSensorFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagValve2Sensor2,
	#else
		0,
	#endif
		},		
		{
		kGAINSteamerTemperatureSensorFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagValve2Sensor2,
	#else
		0,
	#endif
		},		
		{
		kGAINWaterPressureSensorFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagValve3Sensor3,
	#else
		0,
	#endif
		},		
		{
		kGAINPrimaryHeaterFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagHeaterCooler1,
	#else
		0,
	#endif
		},		
		{
		kGAINSecondaryHeaterFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagHeaterCooler2,
	#else
		0,
	#endif
		},		
#ifdef FANSPEED_FAULT_IS_RECOVERABLE
		{
		kGAINErrorFanSpeedOutOfRange,
	#ifdef NSD_SUPPORT
		kNSDDiagTemporaryFault,
	#else
		0,
	#endif
		},		
#else
		{
		kGAINErrorFanSpeedOutOfRange|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagHardware,
	#else
		0,
	#endif
		},		
#endif
		{
		kGAINErrorFanFailure,
	#ifdef NSD_SUPPORT
		kNSDDiagTemporaryFault,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorFanMotorOverTemperature,
	#ifdef NSD_SUPPORT
		kNSDDiagTemporaryFault,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorFanControllerOverTemperature,
	#ifdef NSD_SUPPORT
		kNSDDiagTemporaryFault,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorValveMechanicalFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagValve3Sensor3,
	#else
		0,
	#endif
		},		
		{
		kGAINErrorNonResetableThermalFuseFailure|0x2000,
	#ifdef NSD_SUPPORT
		kNSDDiagHardware,
	#else
		0,
	#endif
		},		
	};
 	


