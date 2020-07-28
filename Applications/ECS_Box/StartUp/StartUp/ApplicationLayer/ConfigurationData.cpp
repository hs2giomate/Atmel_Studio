/*
 * ConfigurationData.cpp
 *
 * Created: 6/23/2020 5:30:58 PM
 *  Author: GMateusDP
 */ 

#include "ConfigurationData.h"
const ConfigurationData factoryDefaultsConfiguration =
{
	
		{
			kConfigurationDataSize,
			kConfigurationDataVersion,
			1804,
			0,		//	Programme ID
			0,
			kDisplayBrightnessHigh,		//	displayBrighness;
			kTemperatureUnitCelsius,
			{1000, 1000, 1000},
		},
		//	temperatureUnit;
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
		160.0,	//	magnetronTemperatureUpperThreshold; ->
		120.0,	//	magnetronTemperatureCutoffThreshold -> Selfprotect
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
		 
};

const CycleData factoryDefaultsCycles =
{
	{
		kCycleDataSize,
		kCycleDataVersion,

		kNumberOfCycleDescriptions,	//	numberOfCycleDescriptions
		kDefaultCycleDescription,	//	defaultCycleDescription
		sizeof(CycleDescription),	//	sizeOfCycleDescription
		
	},

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

};