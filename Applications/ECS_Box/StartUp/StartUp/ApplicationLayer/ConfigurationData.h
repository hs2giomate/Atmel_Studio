/*--------------------------------------------------------------------------

ConfigurationData.h

Interface
Data structure for GAIN configuration data

Author: Steffen Simon
--------------------------------------------------------------------------*/
#ifndef CONFIGURATION_DATA_H
#define CONFIGURATION_DATA_H





#include "ConfigurationCommons.h"

	#define kCycleDataSize						((uint32_t)sizeof(CycleData))
	#define kConfigurationDataSize				((uint32_t)sizeof(ConfigurationData))
	#define kPersistentConfigurationDataSize	((uint32_t)sizeof(PersistentConfigurationData))


	#define kCycleDataVersion					((uint32_t)0x0005)
	#define kConfigurationDataVersion			((uint32_t)0x0004)
	#define kPersistentConfigurationDataVersion	((uint32_t)0x0002)
	#define	kNumberOfCycleDescriptions			12
	#define	kNumberOfContinuesCycleDescriptions	0
	#define	kDefaultCycleDescription			0



enum
	{
	kCycleIsDeactivated = 0,
	kCycleIsActivated = 1,
	};




typedef struct
	{
	char	description[20];
	uint8	cycleFlags;							//	0: convection mode, 1: defrost mode, 2: steam mode
	uint8	reserved;
	int32_t		power;
	int32_t		duration;
	int32_t		maximumDuration;

	} CycleDescription;

typedef struct
	{
	ConfigurationDataCommons	commons;
	//>	Data structure payload
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

	uint32_t	Smec;
    uint32_t	SRmec;      //!< Tells when to switch from self protect state to  standby on in case of empty caven
	//float	Tmec;
	float	Tmg;
	//float	Tms;
	float	Tmo;

	uint32_t	magnetronFanOnAfterCateringCycleTime;

	} ConfigurationData;

typedef		void	 (*PTR_CONFIG_DATA)(ConfigurationData *);

typedef struct
	{
	CycleDictionary		dictionary;
	CycleDescription	cycles[kNumberOfCycleDescriptions + kNumberOfContinuesCycleDescriptions];
	} CycleData;

typedef struct
	{
	//> Data structure size
	uint32_t	size;
	//> Data structure version
	uint32_t	version;
	//>	Part number, i.e. 922 (steam oven, convection oven)
	uint32_t	partNumber;
	int32_t		cycleDescriptionID;
	CycleDescription	cycle;
	tick_t	delayTime;
	} PersistentConfigurationData;

#ifndef AVR
#pragma pack()
#endif

struct ECSConfiguration
{
	uint32_t	crc32;
	uint32_t	magic;
	uint32_t	size;
	uint32_t	version;
};

typedef struct ECSConfiguration	ECSConfiguration;

typedef		void	(*PTR_CTR_STATE)(ECSConfiguration*);

extern const ConfigurationData	factoryDefaultsConfiguration;
extern const CycleData			factoryDefaultsCycles ;

#endif	// CONFIGURATION_DATA_H
