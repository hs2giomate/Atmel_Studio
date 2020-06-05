/*--------------------------------------------------------------------------

ConfigurationData.c

Implementation
Data structure for GAIN configuration data

Author: Steffen Simon

$Date: 2018-06-28 10:11:57 +0200 (Do, 28. Jun 2018) $
$Revision: 7966 $
$Author: steffen $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Common/ConfigurationData.c $

--------------------------------------------------------------------------*/
#include "ConfigurationData.h"

const ConfigurationData factoryDefaultsConfiguration PROGMEM =
	{
		{
		kConfigurationDataSize,
		kConfigurationDataVersion,
		PARTNUMBER,
	#if (PARTNUMBER == 922)
		1,		//	sub part number
	#else
		0,		//	sub part number
	#endif
		0,		//	Programme ID

		kDisplayBrightnessHigh,		//	displayBrighness;
		kTemperatureUnitCelsius,	//	temperatureUnit;
	#if (PARTNUMBER == 916)
        {1000, 1000, 1000},
	#elif (PARTNUMBER == 932)
        {1000, 1000, 1000},
	#elif (PARTNUMBER == 920)
        {1800, 1800, 1800},
	#elif (PARTNUMBER == 921)
        {700, 700, 700},
	#elif (PARTNUMBER == 922)
        {1500, 1500, 1500},
	#else
        {1300, 1300, 1300},
  	#endif
		},

#if (PARTNUMBER == 916)
	0.0,	//	exhaustTemperatureOffset;
	0.0,	//	magnetronTemperatureOffset;
	70.0,	//	minimumTargetVoltage;
	160.0,	//	maximumTargetVoltage;
	-0.056,
	16,
	1.35,
	80.0,
	5.0,	//	voltageDeviationLimit;
	10.0,	//	lowerPowerDeviationPercentageLimit;
	120,
#elif (PARTNUMBER == 920)
	0.0,	//	inletTemperatureOffset
	0.0,	//	outletTemperatureOffset
	0,		//	pressure sensor offset
	99,		//	overheatTemperature;
	120,	//	heaterOverheatedTemperature;
	4,		//	switchedModeEntryTemperatureDelta
	0,      //	switchedModeExitTemperatureDelta
	6.0,	//	estimatedFlowPerSecond;
	0.4,	//	k_p_rampup;
	0.0,	//	k_i_rampup;
	4.0,	//	k_d_rampup;
	0.4,	//	k_p;
	0.01,	//	k_i;
	8.0,	//	k_d;
	32,		//	PWMDutyCycleDelta;
	24000,	//	Minimum duty cycle
	65000,	//	Maximum duty cycle
	12,
	18,
	20,
	0,
	1010,	//	descaleWarningLimit;
	1210,	//	descaleRequestedLimit;
#elif (PARTNUMBER == 921)
	0.0,		//	cabinetTemperatureOffset
#elif (PARTNUMBER == 922)
	0.0,	//	cabinetTemperatureOffset
	0,		//	Pressure sensor offset
	180,	//	steamerOverheatTemperature
	150,	//	steamerOperationalTemperature
	25000,	//	steamGenerationOffTime
	30000,	//	steamGenerationDelayTime
	100000,	//	steamGenerationFailureTime
	5000,	//	resumeOperationDelayTime
#elif (PARTNUMBER == 928)
	0.0,		//	cabinetTemperatureOffset
#elif (PARTNUMBER == 932)
	{0.13},		//	mSMPS;
	{-586.0},	//	bSMPS;
	85.0,	//	smpsTemperatureUpperThreshold;
	60.0,	//	smpsTemperatureLowerThreshold;

	{0.13},		//	mExhaust;
	{-586.0},	//	bExhaust;
	75.0,	//	exhaustTemperatureUpperThreshold;
	50.0,	//	exhaustTemperatureLowerThreshold;

	{0.11, 0.14, 0.17},			//	mMagnetron;
	{-493.0, -659.0, -787.0},	//	bMagnetron;

	160.0,	//	magnetronTemperatureUpperThreshold;
	120.0,	//	magnetronTemperatureCutoffThreshold
	55.0,	//	magnetronTemperatureLowerThreshold;
	830.0,	//	reducedPowerValue
	70.0,	//	minimumTargetVoltage;
	160.0,	//	maximumTargetVoltage;
	-0.019,	//	m
	15.5,	//	zA0
	1.46,	//	aPowerCalculation
	80.0,	//	bPowerCalculation
	10.0,	//	voltageDeviationLimit;
	10.0,	//	lowerPowerDeviationPercentageLimit;

	20,		//	Smec;
	120,	//	SRmec;
	-0.16,	//	Tmg;
	26.0,	//	Tmo;

	120,	//	magnetronFanOnAfterCateringCycleTime
#endif
	};	

const CycleData factoryDefaultsCycles PROGMEM =
	{
		{
		kCycleDataSize,
		kCycleDataVersion,

		kNumberOfCycleDescriptions,	//	numberOfCycleDescriptions
		kDefaultCycleDescription,	//	defaultCycleDescription
		sizeof(CycleDescription),	//	sizeOfCycleDescription
		},	

#if (PARTNUMBER == 916)
		{
		{"Manual Mode", 0, 1, 0, 0, 0, 0, {{kCycleCanModifyTime|kCycleCanModifyPower, 0, 450, 300, 15, 1200}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}}},
		{"Preset 1-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 2-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 3-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 4-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 5-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 6-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 7-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 8-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 9-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		{"Preset 10-empty", 0, 0, 0, 0, 0, 0, {{kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}, {kCycleIsFixed,0, 0, 0, 0, 0}}},
		}
#elif (PARTNUMBER == 920)
		{
		{"Brew", kModeCoffee, 0, 1800, 89.5},
		{"Hot Water", kModeHotWater, 0, 1600, 89.5},
		}
#elif (PARTNUMBER == 921)
		{
	#ifdef OFFLINE
		{"Beverage Chill", kOvershootCycle, 0, 8, 1, -1, 2 * 60, 13, -2, 60, 11*60},
	#else
		{"Beverage Chill", kOvershootCycle, 0, 8, 1, -1, 15 * 60, 13, -2, 60, 11*60},
	#endif
		{"Refrigerate", kStandardCycle, 0, 4, 1, -1, 0, 0, 0, 60, 9*60},
		{"Freeze", kStandardCycle, 0, -18, 1, -1, 0, 0, 0, 60, 4*60},
		}
#elif (PARTNUMBER == 922)
		{
		{"Steam", kSteamMode, 0x00, 150, 28 * 60},
		{"Convection", kConvectionMode, 0x00, 190, 32 * 60},
		{"Defrost", kDefrostMode, 0x00, 70, 30 * 60},
		{"Preset-1", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-2", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-3", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-4", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-5", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-6", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-7", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-8", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-9", kConvectionMode, 0x00, 35, 3 * 60},
		{"Preset-10", kConvectionMode, 0x00, 35, 3 * 60}
		}
#elif (PARTNUMBER == 928)
		{
	#ifdef OFFLINE
		{"Beverage Chill", kOvershootCycle, 0, 8, 1, -1, 2 * 60, 13, -2, 60, 11*60},
	#else
		{"Beverage Chill", kOvershootCycle, 0, 8, 1, -1, 15 * 60, 13, -2, 60, 11*60},
	#endif
		{"Refrigerate", kStandardCycle, 0, 4, 1, -1, 0, 0, 0, 60, 9*60},
		{"Freeze", kStandardCycle, 0, -18, 1, -1, 0, 0, 0, 60, 4*60},
		}
#elif (PARTNUMBER == 932)
		{
		{"Manual", kCycleIsActivated, 0, 600, 15, 1200},
		{"Quick Start", kCycleIsActivated, 0, 600, 30, 300},
		{"+30 secs", kCycleIsActivated, 0, 240, 30, 30},

		{"Preset 1", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 2", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 3", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 4", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 5", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 6", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 7", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 8", kCycleIsDeactivated, 0, 0, 0, 0},
		{"Preset 9", kCycleIsDeactivated, 0, 0, 0, 0},
		}
#endif
	};	

