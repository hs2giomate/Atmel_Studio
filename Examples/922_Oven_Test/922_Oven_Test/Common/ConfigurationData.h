/*--------------------------------------------------------------------------

ConfigurationData.h

Interface
Data structure for GAIN configuration data

Author: Steffen Simon

$Date: 2018-03-01 12:06:47 +0100 (Do, 01. Mrz 2018) $
$Revision: 7761 $
$Author: steffen $

--------------------------------------------------------------------------*/
#ifndef CONFIGURATION_DATA_H
#define CONFIGURATION_DATA_H

#ifdef AVR
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ConfigurationCommons.h"

#define kCycleDataSize						((uint16)sizeof(CycleData))
#define kConfigurationDataSize				((uint16)sizeof(ConfigurationData))
#define kPersistentConfigurationDataSize	((uint16)sizeof(PersistentConfigurationData))

#if (PARTNUMBER == 916)
	#define kCycleDataVersion					((uint16)0x0004)
	#define kConfigurationDataVersion			((uint16)0x0015)
	#define kPersistentConfigurationDataVersion	((uint16)0x0005)
	#define	kNumberOfCycleDescriptions			11
	#define	kNumberOfContinuesCycleDescriptions	0
	#define	kDefaultCycleDescription			0
#elif (PARTNUMBER == 920)
	#define kCycleDataVersion					((uint16)0x0004)
	#define kConfigurationDataVersion			((uint16)0x0017)
	#define kPersistentConfigurationDataVersion	((uint16)0x0004)

	#define	kNumberOfCycleDescriptions			2
	#define	kNumberOfContinuesCycleDescriptions	0
	#define	kDefaultCycleDescription			0
#elif (PARTNUMBER == 921)
	#define kCycleDataVersion					((uint16)0x0003)
	#define kConfigurationDataVersion			((uint16)0x0011)
	#define kPersistentConfigurationDataVersion	((uint16)0x0004)

	#define	kNumberOfCycleDescriptions			3
	#define	kNumberOfContinuesCycleDescriptions	0
	#define	kDefaultCycleDescription			1
#elif (PARTNUMBER == 922)
	#define kCycleDataVersion					((uint16)0x0004)
	#define kConfigurationDataVersion			((uint16)0x0012)
	#define kPersistentConfigurationDataVersion	((uint16)0x0004)

	#define	kNumberOfCycleDescriptions			13
	#define	kNumberOfContinuesCycleDescriptions	0
	#define	kDefaultCycleDescription			0
#elif (PARTNUMBER == 928)
	#define kCycleDataVersion					((uint16)0x0003)
	#define kConfigurationDataVersion			((uint16)0x0010)
	#define kPersistentConfigurationDataVersion	((uint16)0x0004)

	#define	kNumberOfCycleDescriptions			3
	#define	kNumberOfContinuesCycleDescriptions	0
	#define	kDefaultCycleDescription			1
#elif (PARTNUMBER == 932)
	#define kCycleDataVersion					((uint16)0x0005)
	#define kConfigurationDataVersion			((uint16)0x0004)
	#define kPersistentConfigurationDataVersion	((uint16)0x0002)
	#define	kNumberOfCycleDescriptions			12
	#define	kNumberOfContinuesCycleDescriptions	0
	#define	kDefaultCycleDescription			0
#else
	#error "Part Number not supported"
#endif

#if (PARTNUMBER == 916)
enum
	{
	kCycleIsFixed = 0,
	kCycleCanModifyTime = 1,
	kCycleCanModifyPower = 2,
	};
#elif (PARTNUMBER == 920)
enum
	{
	kModeCoffee = 0,
	kModeHotWater = 1,
	};
#elif (PARTNUMBER == 921)
enum
	{
	kStandardCycle = 0,
	kOvershootCycle = 1,
	};
#elif (PARTNUMBER == 922)
enum
	{
	kConvectionMode,
	kDefrostMode,
	kSteamMode,
	};

enum
	{
	kContinousMode = 0x20,
	};

#define kCycleModeMask		0x0F
#define kCycleIsModifiable	0x10
#elif (PARTNUMBER == 928)
enum
	{
	kStandardCycle = 0,
	kOvershootCycle = 1,
	};
#elif (PARTNUMBER == 932)
enum
	{
	kCycleIsDeactivated = 0,
	kCycleIsActivated = 1,
	};
#else
	#error "Part Number not supported"
#endif

#ifndef AVR
#pragma pack(2)
#endif

#if (PARTNUMBER == 916)
typedef struct
	{
	uint8	stepFlags;
	uint8	reserved;
	int16	targetPower;
	int16	stepDuration;
	int16	minimumStepDuration;
	int16	maximumStepDuration;
	} CycleStep;
#endif

typedef struct
	{
	char	description[20];
	uint8	cycleFlags;							//	0: convection mode, 1: defrost mode, 2: steam mode
#if (PARTNUMBER != 916)
	uint8	reserved;
#endif
#if (PARTNUMBER == 916)
	uint8		numberOfSteps;
	int16		cycleDuration;
	int16		minimumCycleDuration;
	int16		maximumCycleDuration;
	int16		maximumPower;
	CycleStep	cycleSteps[4];
#elif (PARTNUMBER == 920)
	int16	targetVolume;
	float	targetTemperature;
#elif (PARTNUMBER == 921)
	int16	targetTemperature;
	int8	targetTemperatureUpperDeviation;	//	+1...+127
	int8	targetTemperatureLowerDeviation;	//	-1...-128
	int16	overshootTime;						//	0 == no overshoot mode
	int16	overshootEntryTemperature;			//	if cabinet temperature >= overshootEntryTemperature switch to overshoot mode
	int16	overshootTargetTemperature;			//	overshoot target temperature
	int16	defaultModeOnTime;
	int16	defaultModeOffTime;
#elif (PARTNUMBER == 922)
	int16	targetTemperature;
	int16	cycleDuration;
#elif (PARTNUMBER == 928)
	int16	targetTemperature;
	int8	targetTemperatureUpperDeviation;	//	+1...+127
	int8	targetTemperatureLowerDeviation;	//	-1...-128
	int16	overshootTime;						//	0 == no overshoot mode
	int16	overshootEntryTemperature;			//	if cabinet temperature >= overshootEntryTemperature switch to overshoot mode
	int16	overshootTargetTemperature;			//	overshoot target temperature
	int16	defaultModeOnTime;
	int16	defaultModeOffTime;
#elif (PARTNUMBER == 932)
	int16		power;
	int16		duration;
	int16		maximumDuration;
#endif
	} CycleDescription;

typedef struct
	{
	ConfigurationDataCommons	commons;
	//>	Data structure payload
#if (PARTNUMBER == 916)
	float	exhaustTemperatureOffset;
	float	magnetronTemperatureOffset;
	float	minimumTargetVoltage;
	float	maximumTargetVoltage;
	float	m;
	float	zA0;
	float	aPowerCalculation;
	float	bPowerCalculation;

	float	voltageDeviationLimit;
	float	lowerPowerDeviationPercentageLimit;

	uint16	magnetronFanOnAfterCateringCycleTime;
#elif (PARTNUMBER == 917)
#elif (PARTNUMBER == 920)
	float	inletTemperatureOffset;
	float	outletTemperatureOffset;
	int16	pressureSensorOffset;
	int16	overheatTemperature;
	int16	heaterOverheatedTemperature;
	int16	switchedModeEntryTemperatureDelta;
	int16	switchedModeExitTemperatureDelta;
	float	estimatedFlowPerSecond;
	float	k_p_rampup;
	float	k_i_rampup;
	float	k_d_rampup;
	float	k_p;
	float	k_i;
	float	k_d;
	int16	PWMDutyCycleDelta;
	uint16	PWMMinimumDutyCycle;
	uint16	PWMMaximumDutyCycle;
	uint8	evacuationTimer1;
	uint8	evacuationTimer2;
	uint8	evacuationTimer3;
	uint8	sensorInverted;						//	0: Non inverted, 1: Inverted
	int16	descaleWarningLimit;
	int16	descaleRequestedLimit;
#elif (PARTNUMBER == 921)
	float	cabinetTemperatureOffset;
#elif (PARTNUMBER == 922)
	float	cabinetTemperatureOffset;
	int16	pressureSensorOffset;
	int16	steamerOverheatTemperature;
	int16	steamerOperationalTemperature;
	tick_t	steamGenerationOffTime;
	tick_t	steamGenerationDelayTime;
	tick_t	steamGenerationFailureTime;
	tick_t	resumeOperationDelayTime;
#elif (PARTNUMBER == 928)
	float	cabinetTemperatureOffset;
#elif (PARTNUMBER == 932)
	float	mSMPS[1];
	float	bSMPS[1];
	float	smpsTemperatureUpperThreshold;
	float	smpsTemperatureLowerThreshold;

	float	mExhaust[1];
	float	bExhaust[1];
	float	exhaustTemperatureUpperThreshold;
	float	exhaustTemperatureLowerThreshold;

	float	mMagnetron[3];
	float	bMagnetron[3];
	float	magnetronTemperatureUpperThreshold;
	float	magnetronTemperatureCutoffThreshold;
	float	magnetronTemperatureLowerThreshold;
	
	float	reducedPowerValue;

	float	minimumTargetVoltage;
	float	maximumTargetVoltage;
	
	float	m;
	float	zA0;
	float	aPowerCalculation;
	float	bPowerCalculation;

	float	voltageDeviationLimit;
	float	lowerPowerDeviationPercentageLimit;

	uint16	Smec;
	uint16	SRmec;
	//float	Tmec;
	float	Tmg;
	//float	Tms;
	float	Tmo;

	uint16	magnetronFanOnAfterCateringCycleTime;
#endif
	} ConfigurationData;

typedef struct
	{
	CycleDictionary		dictionary;
	CycleDescription	cycles[kNumberOfCycleDescriptions + kNumberOfContinuesCycleDescriptions];
	} CycleData;

typedef struct
	{
	//> Data structure size
	uint16	size;
	//> Data structure version
	uint16	version;
	//>	Part number, i.e. 922 (steam oven, convection oven)
	uint16	partNumber;

	int16				cycleDescriptionID;
	CycleDescription	cycle;
#if (PARTNUMBER == 916)
	tick_t	delayTime;
#elif (PARTNUMBER == 932)
	tick_t	delayTime;
#elif (PARTNUMBER == 917)
#elif (PARTNUMBER == 920)
#elif (PARTNUMBER == 921)
#elif (PARTNUMBER == 922)
	tick_t	delayTime;
#elif (PARTNUMBER == 928)
#endif
#if (PARTNUMBER != 932)
	bool	alarmState;
#endif
	} PersistentConfigurationData;

#ifndef AVR
#pragma pack()
#endif

extern const ConfigurationData	factoryDefaultsConfiguration PROGMEM;
extern const CycleData			factoryDefaultsCycles PROGMEM;

#ifdef __cplusplus
	}
#endif

#endif	// CONFIGURATION_DATA_H
