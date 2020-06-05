#ifdef NSD_SUPPORT

#include "coreNSDDiagnostics.h"

#if defined(AVR)
const NSDDiag	NSDDiagList[] PROGMEM =
#else
const NSDDiag	NSDDiagList[] =
#endif
	{
		{0, 0},
		{0, 0},

		{kNSDDiagCateringProcessQualityDamagedBit, kDIAGWarning},
		{kNSDDiagMaximalCateringTemperatureNotReachedBit, kDIAGWarning},
		{kNSDDiagMaximalCateringTemperatureExceededBit, kDIAGWarning},
		{kNSDDiagDataTransmissionInterruptedBit, kDIAGWarning},
		{kNSDDiagFileAlreadyExistsBit, kDIAGWarning},
		{kNSDDiagServerNotInsertedBit, kDIAGWarning},
		{kNSDDiagServerNotLockedBit, kDIAGWarning},
		{kNSDDiagDoorOpenBit, kDIAGWarning},
		{kNSDDiagServerFullBit, kDIAGWarning},
		{kNSDDiagFoodNotRemovedBit, kDIAGWarning},
		{kNSDDiagCalcificationBit, kDIAGWarning},
		{kNSDDiagOverheatBit, kDIAGWarning},
		{kNSDDiagOnboardMaintenanceBit, kDIAGWarning},
		{kNSDDiagInvalidDeviceIDBit, kDIAGWarning},
		{kNSDDiagDataTransferDeniedBit, kDIAGWarning},
		{kNSDDiagGeneralOverflowBit, kDIAGWarning},
		{kNSDDiagMaintenanceBit, kDIAGWarning},
		{kNSDDiagAirFilterBit, kDIAGWarning},
		{kNSDDiagPowerLossLT200msecBit, kDIAGWarning},
		{kNSDDiagPowerLossGE200msecBit, kDIAGWarning},
		{kNSDDiagPinProgrammingFeederIDBit, kDIAGWarning},
		{kNSDDiagPinProgrammingDeviceIDBit, kDIAGWarning},
		{kNSDDiagPinProgrammingPrimaryBit, kDIAGWarning},
		{kNSDDiagTemporaryFaultBit, kDIAGWarning},
		{kNSDDiagNetworkFailureBit, kDIAGWarning},
		{kNSDDiagOutOfServiceBit, kDIAGWarning},

		{kNSDDiagDataTransmissionErrorBit, kDIAGMalfunction},
		{kNSDDiagCommandErrorBit, kDIAGMalfunction},
		{kNSDDiagConfigurationErrorBit, kDIAGMalfunction},
		{kNSDDiagPowerFactorCorrectionDeviceBit, kDIAGMalfunction},
		{kNSDDiagOverPressureDevice1Bit, kDIAGMalfunction},
		{kNSDDiagNoWaterBit, kDIAGMalfunction},
		{kNSDDiagMalfunctionBit, kDIAGMalfunction},
		{kNSDDiagTankServerWaterOverflowBit, kDIAGMalfunction},
		{kNSDDiagLowPressureBit, kDIAGMalfunction},
		{kNSDDiagOverPressureBit, kDIAGMalfunction},

		{kNSDDiagChecksumBit, kDIAGFailure},
		{kNSDDiagPhaseLossBit, kDIAGFailure},
		{kNSDDiagPhaseALossBit, kDIAGFailure},
		{kNSDDiagPhaseBLossBit, kDIAGFailure},
		{kNSDDiagPhaseCLossBit, kDIAGFailure},
		{kNSDDiagInternalVDCDeviceBit, kDIAGFailure},
		{kNSDDiagFusePhaseABit, kDIAGFailure},
		{kNSDDiagFusePhaseBBit, kDIAGFailure},
		{kNSDDiagFusePhaseCBit, kDIAGFailure},
		{kNSDDiagPinProgrammingParityErrorBit, kDIAGFailure},
		{kNSDDiagHardwareBit, kDIAGFailure},
		{kNSDDiagHeaterCooler1Bit, kDIAGFailure},
		{kNSDDiagHeaterCooler2Bit, kDIAGFailure},
		{kNSDDiagHeaterCooler3Bit, kDIAGFailure},
		{kNSDDiagHeaterCooler4Bit, kDIAGFailure},
		{kNSDDiagPumpMotor1Bit, kDIAGFailure},
		{kNSDDiagPumpMotor2Bit, kDIAGFailure},
		{kNSDDiagValve1Sensor1Bit, kDIAGFailure},
		{kNSDDiagValve2Sensor2Bit, kDIAGFailure},
		{kNSDDiagValve3Sensor3Bit, kDIAGFailure},
		{kNSDDiagBurstDiskBit, kDIAGFailure},
		{kNSDDiagRotatingPlateBit, kDIAGFailure},
		{kNSDDiagFan1Bit, kDIAGFailure},
		{kNSDDiagFan2Bit, kDIAGFailure},
		{kNSDDiagDisplayBit, kDIAGFailure},
		{kNSDDiagOverPressureDevice2Bit, kDIAGFailure},
		{kNSDDiagOverheatDevice1Bit, kDIAGFailure},
		{kNSDDiagOverheatDevice2Bit, kDIAGFailure},
		{kNSDDiagOverheatDevice3Bit, kDIAGFailure},
		{kNSDDiagGeneralSoftwareFailureBit, kDIAGFailure},
	};

#endif	//	NSD_SUPPORT


