/*--------------------------------------------------------------------------

coreNSDDiagnostics.h

Interface
Definition Errorcodes, Beschreibungen, Kategorie

Autor: Steffen Simon

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Network/coreNSDDiagnostics.h $
--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT

#ifndef CORE_NSD_DIAGNOSTICS_H
#define CORE_NSD_DIAGNOSTICS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(AVR)
#include <avr/pgmspace.h>
#endif
#include "coreTypes.h"

typedef enum
	{
	kDIAGWarning = 0,
	kDIAGMalfunction = 1,
	kDIAGFailure = 2,
	} kNSDDIAGClassification;

typedef enum
	{
	kNSDDiagNoError = 0,
	kNSDDiagNoHandler,

	kNSDDiagCateringProcessQualityDamaged,
	kNSDDiagMaximalCateringTemperatureNotReached,
	kNSDDiagMaximalCateringTemperatureExceeded,
	kNSDDiagDataTransmissionInterrupted,
	kNSDDiagFileAlreadyExists,
	kNSDDiagServerNotInserted,
	kNSDDiagServerNotLocked,
	kNSDDiagDoorOpen,
	kNSDDiagServerFull,
	kNSDDiagFoodNotRemoved,
	kNSDDiagCalcification,
	kNSDDiagOverheat,
	kNSDDiagOnboardMaintenance,
	kNSDDiagInvalidDeviceID,
   	kNSDDiagDataTransferDenied,
   	kNSDDiagGeneralOverflow,
   	kNSDDiagMaintenance,
   	kNSDDiagAirFilter,
   	kNSDDiagPowerLossLT200msec,
   	kNSDDiagPowerLossGE200msec,
	kNSDDiagPinProgrammingFeederID,
	kNSDDiagPinProgrammingDeviceID,
	kNSDDiagPinProgrammingPrimary,
	kNSDDiagTemporaryFault,
	kNSDDiagNetworkFailure,
	kNSDDiagOutOfService,

	kNSDDiagDataTransmissionError,
	kNSDDiagCommandError,
	kNSDDiagConfigurationError,
	kNSDDiagPowerFactorCorrectionDevice,
	kNSDDiagOverPressureDevice1,
	kNSDDiagNoWater,
	kNSDDiagMalfunction,
	kNSDDiagTankServerWaterOverflow,
	kNSDDiagLowPressure,
	kNSDDiagOverPressure,

	kNSDDiagChecksum,
	kNSDDiagPhaseLoss,
	kNSDDiagPhaseALoss,
	kNSDDiagPhaseBLoss,
	kNSDDiagPhaseCLoss,
	kNSDDiagInternalVDCDevice,
	kNSDDiagFusePhaseA,
	kNSDDiagFusePhaseB,
	kNSDDiagFusePhaseC,
	kNSDDiagPinProgrammingParityError,
	kNSDDiagHardware,
	kNSDDiagHeaterCooler1,
	kNSDDiagHeaterCooler2,
	kNSDDiagHeaterCooler3,
	kNSDDiagHeaterCooler4,
	kNSDDiagPumpMotor1,
	kNSDDiagPumpMotor2,
	kNSDDiagValve1Sensor1,
	kNSDDiagValve2Sensor2,
	kNSDDiagValve3Sensor3,
	kNSDDiagBurstDisk,
	kNSDDiagRotatingPlate,
	kNSDDiagFan1,
	kNSDDiagFan2,
	kNSDDiagDisplay,
	kNSDDiagOverPressureDevice2,
	kNSDDiagOverheatDevice1,
	kNSDDiagOverheatDevice2,
	kNSDDiagOverheatDevice3,
	kNSDDiagGeneralSoftwareFailure,
	} NSDDiagCode;

typedef enum
	{
	kNSDDiagCateringProcessQualityDamagedBit = 16,
	kNSDDiagMaximalCateringTemperatureNotReachedBit = 17,
	kNSDDiagMaximalCateringTemperatureExceededBit = 18,
	kNSDDiagDataTransmissionInterruptedBit = 19,
	kNSDDiagFileAlreadyExistsBit = 20,
	kNSDDiagServerNotInsertedBit = 21,
	kNSDDiagServerNotLockedBit = 22,
	kNSDDiagDoorOpenBit = 23,
	kNSDDiagServerFullBit = 24,
	kNSDDiagFoodNotRemovedBit = 25,
	kNSDDiagCalcificationBit = 26,
	kNSDDiagOverheatBit = 27,
	kNSDDiagOnboardMaintenanceBit = 28,
	kNSDDiagInvalidDeviceIDBit = 29,
   	kNSDDiagDataTransferDeniedBit = 31,
   	kNSDDiagGeneralOverflowBit = 34,
   	kNSDDiagMaintenanceBit = 36,
   	kNSDDiagAirFilterBit = 38,
   	kNSDDiagPowerLossLT200msecBit = 40,
   	kNSDDiagPowerLossGE200msecBit = 41,
	kNSDDiagPinProgrammingFeederIDBit = 42,
	kNSDDiagPinProgrammingDeviceIDBit = 43,
	kNSDDiagPinProgrammingPrimaryBit = 44,
	kNSDDiagTemporaryFaultBit = 45,
	kNSDDiagNetworkFailureBit = 46,
	kNSDDiagOutOfServiceBit = 47,
	} NSDWarningBit;
	
typedef enum
	{
	kNSDDiagDataTransmissionErrorBit = 16,
	kNSDDiagCommandErrorBit = 17,
	kNSDDiagConfigurationErrorBit = 18,
	kNSDDiagOverPressureDevice1Bit = 20,
	kNSDDiagNoWaterBit = 21,
	kNSDDiagMalfunctionBit = 22,
	kNSDDiagTankServerWaterOverflowBit = 23,
	kNSDDiagLowPressureBit = 24,
	kNSDDiagOverPressureBit = 25,
	} NSDMalfunctionBit;

typedef enum
	{
	kNSDDiagChecksumBit = 16,
	kNSDDiagPowerFactorCorrectionDeviceBit = 17,
	kNSDDiagPhaseLossBit = 18,
	kNSDDiagPhaseALossBit = 19,
	kNSDDiagPhaseBLossBit = 20,
	kNSDDiagPhaseCLossBit = 21,
	kNSDDiagInternalVDCDeviceBit = 22,
	kNSDDiagFusePhaseABit = 23,
	kNSDDiagFusePhaseBBit = 24,
	kNSDDiagFusePhaseCBit = 25,
	kNSDDiagPinProgrammingParityErrorBit = 26,
	kNSDDiagHardwareBit = 29,
	kNSDDiagHeaterCooler1Bit = 30,
	kNSDDiagHeaterCooler2Bit = 31,
	kNSDDiagHeaterCooler3Bit = 32,
	kNSDDiagHeaterCooler4Bit = 33,
	kNSDDiagPumpMotor1Bit = 34,
	kNSDDiagPumpMotor2Bit = 35,
	kNSDDiagValve1Sensor1Bit = 36,
	kNSDDiagValve2Sensor2Bit = 37,
	kNSDDiagValve3Sensor3Bit = 38,
	kNSDDiagBurstDiskBit = 39,
	kNSDDiagRotatingPlateBit = 40,
	kNSDDiagFan1Bit = 41,
	kNSDDiagFan2Bit = 42,
	kNSDDiagDisplayBit = 43,
	kNSDDiagOverPressureDevice2Bit = 44,
	kNSDDiagOverheatDevice1Bit = 45,
	kNSDDiagOverheatDevice2Bit = 46,
	kNSDDiagOverheatDevice3Bit = 47,
	kNSDDiagGeneralSoftwareFailureBit = 53,
	} NSDFailureBit;

typedef struct
	{
	uint8	diagBit;
	uint8	diagClassification;
	} NSDDiag;

#if defined(AVR)
extern const NSDDiag	NSDDiagList[] PROGMEM;
#else
extern const NSDDiag	NSDDiagList[];
#endif

#ifdef __cplusplus
	}
#endif

#endif

#endif	//	NSD_SUPPORT



