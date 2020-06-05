/**--------------------------------------------------------------------------

@file PersistenceServices.cc

Implementation
Persistentes Speichern von Heizzustand, Betriebsstunden,...

Autor: Steffen Simon
$Date: 2018-06-07 09:51:15 +0200 (Do, 07. Jun 2018) $
$Revision: 7930 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Common/PersistenceServices.cc $

--------------------------------------------------------------------------*/
#include "coreNVM.h"
#include "coreRTC.h"
#include "PersistenceServices.h"
#include "version.h"
#ifdef NSD_SUPPORT
#include "NSDHandler.h"
#endif

#if __DEBUG__ > 0
#include "eDevice.h"
#endif

const uint32	validMagic = 0x14031869;


//	internal EEPROM: total 2048 bytes
typedef struct
	{
	//	eCore library private NVM area
	byte							eCorePrivate[0x40];
	//	Application private NVM area
	byte							applicationPrivate[0x40];
#ifdef NO_PERSISTENCE
	//	Network stack private NVM area
	byte							networkStackPrivate[0x400];
#endif

#ifndef MGS_ZST_Revision
	ConfigurationDataArea			configurationArea;
	CycleDataArea					cycleArea;
#endif

#ifdef NO_PERSISTENCE
	persistentUsageDataArea			usageDataArea;

#if __PARTNUMBER == 920
	persistentMaintenanceArea		maintenanceArea;
#endif
	persistentCycleCountArea		cycleCountArea;
	PersistentConfigurationDataArea	persistentConfigurationArea;
#if (PARTNUMBER == 932)
	PersistentLRUCyclesArea			persistentLRUCyclesArea;
#endif
	persistentWorkingStateArea		cateringStateArea[2];
#endif
	} internalEEPROMMemoryLayout;

//	external EEPROM: total 16*4096 bytes
typedef struct
	{
	//	eCore library private NVM area
	byte							eCorePrivate[0x40];
	//	Application private NVM area
	byte							applicationPrivate[0x40];

	persistentErrorArea				errorArea;

#ifdef MGS_ZST_Revision
	ConfigurationDataArea			configurationArea;
	CycleDataArea					cycleArea;
#endif
	} externalEEPROMMemoryLayout;

//	FRAM: total 4096 bytes
typedef struct
	{
	//	eCore library private NVM area
	byte							eCorePrivate[0x40];
	//	Application private NVM area
	byte							applicationPrivate[0x40];
#ifndef NO_PERSISTENCE
	//	Network stack private NVM area
	byte							networkStackPrivate[0x400];

	persistentUsageDataArea			usageDataArea;
	persistentCycleCountArea		cycleCountArea;
#if __PARTNUMBER == 920
	persistentMaintenanceArea		maintenanceArea;
#endif
	PersistentConfigurationDataArea	persistentConfigurationArea;
#if (PARTNUMBER == 932)
	PersistentLRUCyclesArea			persistentLRUCyclesArea;
#endif
#ifndef MGS_ZST_Revision
	persistentWorkingStateArea		cateringStateArea[2];
#endif
#endif
	} externalFRAMMemoryLayout;

#ifdef MGS_ZST_Revision
typedef struct
	{
	persistentWorkingStateArea		cateringStateArea[2];
	} RAMMemoryLayout;
#endif

internalEEPROMMemoryLayout&	InternalEEPROMProxy(*((internalEEPROMMemoryLayout*)NULL));
externalEEPROMMemoryLayout&	EEPROMProxy(*((externalEEPROMMemoryLayout*)NULL));

externalFRAMMemoryLayout&	FRAMProxy(*((externalFRAMMemoryLayout*)NULL));
#ifdef MGS_ZST_Revision
RAMMemoryLayout&	RAMProxy(*((RAMMemoryLayout*)NULL));
#endif

#if PARTNUMBER == 922
tick_t PersistenceServices::lastUpdate[kNumberOfUsageTimeCounters + 3];	
#else
tick_t PersistenceServices::lastUpdate[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions];	
#endif

PersistenceServices	persistent;

PersistenceServices::PersistenceServices(void)
	:lastValidSaveWorkingStateForGAINIndex(0)
	{
	}

bool PersistenceServices::readConfigurationDataArea(ConfigurationData& data)
	{
	bool		result(false);

#if defined(MGS_ZST_Revision)
	result = isPersistentAreaValid(kNVM1, EEPROMProxy.configurationArea.header, sizeof(ConfigurationDataArea));
#else
	result = isPersistentAreaValid(kNVM0, InternalEEPROMProxy.configurationArea.header, sizeof(ConfigurationDataArea));
#endif

	if (result)
		{
	#if defined(MGS_ZST_Revision)
		int32	address((int32)&EEPROMProxy.configurationArea.configuration);
		coreNVMRead(kNVM1, address, &data, sizeof(ConfigurationData));
	#else
		int32	address((int32)&InternalEEPROMProxy.configurationArea.configuration);
		coreNVMRead(kNVM0, address, &data, sizeof(ConfigurationData));
	#endif
		result &= (data.commons.version == kConfigurationDataVersion);
		result &= (data.commons.size == kConfigurationDataSize);
		result &= (data.commons.partNumber == PARTNUMBER);
		}
	
	return result;
	}

/*	
bool PersistenceServices::writeConfigurationDataArea(ConfigurationData& data)
	{
	//	NVMHeader	header;
	bool		result(false);

#if defined(MGS_ZST_Revision)
	int32		address((int32)&EEPROMProxy.configurationArea.header);
	coreNVMRead(kNVM1, address, &header, sizeof(header));
#else
	int32		address((int32)&InternalEEPROMProxy.configurationArea.header);
	coreNVMRead(kNVM0, address, &header, sizeof(header));
#endif
	result = (header.magic == validMagic) && (header.size == (uint32)sizeof(ConfigurationDataArea));

	if (result)
		{
		data.commons.version = kConfigurationDataVersion;
		data.commons.size = kConfigurationDataSize;
		data.commons.partNumber = PARTNUMBER;

	#if defined(MGS_ZST_Revision)
		address = (int32)&EEPROMProxy.configurationArea.configuration;
		coreNVMWrite(kNVM1, address, &data, sizeof(ConfigurationData));
	#else
		address = (int32)&InternalEEPROMProxy.configurationArea.configuration;
		coreNVMWrite(kNVM0, address, &data, sizeof(ConfigurationData));
	#endif
		}
#if __DEBUG__ > 4
	else
		{
		controller.asc0.lock();
		controller.asc0 << "writeConfigurationDataArea: header.magic == " << kModeHex << header.magic << '/' << kModeHex << validMagic << newline;
		controller.asc0 << "    header.size == " << kModeHex << header.magic << '/' << sizeof(ConfigurationDataArea) << newline;
		controller.asc0.unlock();
		}
#endif
	
	return result;
	}
*/

bool PersistenceServices::writeConfigurationDataArea(ConfigurationData& data)
	{
	bool	result(true);

	data.commons.version = kConfigurationDataVersion;
	data.commons.size = kConfigurationDataSize;
	data.commons.partNumber = PARTNUMBER;

#if defined(MGS_ZST_Revision)
	int32	address((int32)&EEPROMProxy.configurationArea.configuration);
	coreNVMWrite(kNVM1, address, &data, sizeof(ConfigurationData));
	validatePersistentArea(kNVM1, EEPROMProxy.configurationArea.header, sizeof(ConfigurationDataArea));
#else
	int32	address((int32)&InternalEEPROMProxy.configurationArea.configuration);
	coreNVMWrite(kNVM0, address, &data, sizeof(ConfigurationData));
	validatePersistentArea(kNVM0, InternalEEPROMProxy.configurationArea.header, sizeof(ConfigurationDataArea));
#endif

	return result;
	}

bool PersistenceServices::readCycleDictionaryArea(CycleDictionary& dict)
	{
	bool		result(false);

#if defined(MGS_ZST_Revision)
	result = isPersistentAreaValid(kNVM1, EEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#else
	result = isPersistentAreaValid(kNVM0, InternalEEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#endif
	if (result)
		{
	#if defined(MGS_ZST_Revision)
		int32	address((int32)&EEPROMProxy.cycleArea.dictionary);
		coreNVMRead(kNVM1, address, &dict, sizeof(CycleDictionary));
	#else
		int32	address((int32)&InternalEEPROMProxy.cycleArea.dictionary);
		coreNVMRead(kNVM0, address, &dict, sizeof(CycleDictionary));
	#endif
		
		result = (dict.size == kCycleDataSize) && (dict.version == kCycleDataVersion);
		}

#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "readCycleDictionary: " << (result?"yes":"no") << newline;
	controller.asc0.unlock();
#endif
	
	return result;
	}

bool PersistenceServices::writeCycleDictionaryArea(CycleDictionary& dict)
	{
	bool		result(true);

	dict.size = kCycleDataSize;
	dict.version = kCycleDataVersion;
#if defined(MGS_ZST_Revision)
	int32	address((int32)&EEPROMProxy.cycleArea.dictionary);
	coreNVMWrite(kNVM1, address, &dict, sizeof(CycleDictionary));
	validatePersistentArea(kNVM1, EEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#else
	int32	address((int32)&InternalEEPROMProxy.cycleArea.dictionary);
	coreNVMWrite(kNVM0, address, &dict, sizeof(CycleDictionary));
	validatePersistentArea(kNVM0, InternalEEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#endif
	
	return result;
	}

bool PersistenceServices::readCycleDescription(CycleDescription& cycle, uint8 id)
	{
	bool		result(false);

#if defined(MGS_ZST_Revision)
	result = isPersistentAreaValid(kNVM1, EEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#else
	result = isPersistentAreaValid(kNVM0, InternalEEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#endif
	if (result)
		{
	#if defined(MGS_ZST_Revision)
		int32	address((int32)&EEPROMProxy.cycleArea.cycles.cycles[id]);
		coreNVMRead(kNVM1, address, &cycle, sizeof(CycleDescription));
	#else
		int32	address((int32)&InternalEEPROMProxy.cycleArea.cycles.cycles[id]);
		coreNVMRead(kNVM0, address, &cycle, sizeof(CycleDescription));
	#endif
		}

#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "readCycleDescription #" << id << (result?" yes":" no") << newline;
	controller.asc0.unlock();
#endif
	
	return result;
	}

bool PersistenceServices::writeCycleDescription(CycleDescription& cycle, uint8 id)
	{
	bool		result(true);

#if defined(MGS_ZST_Revision)
	int32	address((int32)&EEPROMProxy.cycleArea.cycles.cycles[id]);
	coreNVMWrite(kNVM1, address, &cycle, sizeof(CycleDescription));
#else
	int32	address((int32)&InternalEEPROMProxy.cycleArea.cycles.cycles[id]);
	coreNVMWrite(kNVM0, address, &cycle, sizeof(CycleDescription));
#endif

#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "writeCycleDescription: " << (result?"yes":"no") << newline;
	controller.asc0.unlock();
#endif
	
	return result;
	}

#if (PARTNUMBER == 932)
bool PersistenceServices::readLRUCyclesArea(LRUCycles& lru)
	{
	bool		result;

#ifdef NO_PERSISTENCE
	result = isPersistentAreaValid(kNVM0, InternalEEPROMProxy.persistentLRUCyclesArea.header, sizeof(PersistentLRUCyclesArea));
	if (result)
		{
		int32	address((int32)&InternalEEPROMProxy.persistentLRUCyclesArea.lru);
		coreNVMRead(kNVM0, address, &lru, sizeof(PersistentLRUCyclesArea));
		}
#else
	result = isPersistentAreaValid(kNVM2, FRAMProxy.persistentLRUCyclesArea.header, sizeof(PersistentLRUCyclesArea));
	if (result)
		{
		int32	address((int32)&FRAMProxy.persistentLRUCyclesArea.lru);
		coreNVMRead(kNVM2, address, &lru, sizeof(PersistentLRUCyclesArea));
		}
#endif

#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "readLRUCyclesArea: " << (result?"yes":"no") << newline;
	controller.asc0.unlock();
#endif
	
	return result;
	}

bool PersistenceServices::writeLRUCyclesArea(LRUCycles& lru)
	{
	bool		result(true);

#ifdef NO_PERSISTENCE
	int32	address((int32)&InternalEEPROMProxy.persistentLRUCyclesArea.lru);

    invalidatePersistentArea(kNVM0, InternalEEPROMProxy.persistentLRUCyclesArea.header);
	coreNVMWrite(kNVM0, address, &lru, sizeof(PersistentLRUCyclesArea));
    validatePersistentArea(kNVM0, InternalEEPROMProxy.persistentConfigurationArea.header, sizeof(PersistentLRUCyclesArea));
#else
	int32	address((int32)&FRAMProxy.persistentLRUCyclesArea.lru);

    invalidatePersistentArea(kNVM2, FRAMProxy.persistentLRUCyclesArea.header);
	coreNVMWrite(kNVM2, address, &lru, sizeof(PersistentLRUCyclesArea));
    validatePersistentArea(kNVM2, FRAMProxy.persistentLRUCyclesArea.header, sizeof(PersistentLRUCyclesArea));
#endif
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "writeLRUCyclesArea done" << newline;
	controller.asc0.unlock();
#endif
	return result;
	}
#endif

void PersistenceServices::invalidatePersistentConfigurationDataArea(void)
	{
#ifdef NO_PERSISTENCE
	invalidatePersistentArea(kNVM0,InternalEEPROMProxy.persistentConfigurationArea.header);
#else
	invalidatePersistentArea(kNVM2,FRAMProxy.persistentConfigurationArea.header);
#endif
	}

bool PersistenceServices::readPersistentConfigurationDataArea(PersistentConfigurationData& data)
	{
	bool		result;

#ifdef NO_PERSISTENCE
	result = isPersistentAreaValid(kNVM0, InternalEEPROMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
	if (result)
		{
		int32	address((int32)&InternalEEPROMProxy.persistentConfigurationArea.configuration);
		coreNVMRead(kNVM0, address, &data, sizeof(PersistentConfigurationData));
		}
#else
	result = isPersistentAreaValid(kNVM2, FRAMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
	if (result)
		{
		int32	address((int32)&FRAMProxy.persistentConfigurationArea.configuration);
		coreNVMRead(kNVM2, address, &data, sizeof(PersistentConfigurationData));
		}
#endif
	result &= (data.version == kPersistentConfigurationDataVersion);
	result &= (data.size == kPersistentConfigurationDataSize);
	result &= (data.partNumber == PARTNUMBER);

#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "readPersistentConfigurationDataArea: " << (result?"yes":"no") << newline;
	if (!result)
		{
		controller.asc0 << "data.version == " << data.version << ", expected " << kPersistentConfigurationDataVersion << newline;
		controller.asc0 << "data.size == " << data.size << ", expected " << kPersistentConfigurationDataSize << newline;
		controller.asc0 << "data.partNumber == " << data.partNumber << ", expected " << (uint16)PARTNUMBER << newline;
		}
	controller.asc0.unlock();
#endif
	
	return result;
	}
	
bool PersistenceServices::writePersistentConfigurationDataArea(PersistentConfigurationData& data)
	{
	bool		result(true);

	data.version = kPersistentConfigurationDataVersion;
	data.size = kPersistentConfigurationDataSize;
	data.partNumber = PARTNUMBER;

#ifdef NO_PERSISTENCE
	int32	address((int32)&InternalEEPROMProxy.persistentConfigurationArea.configuration);

    invalidatePersistentArea(kNVM0, InternalEEPROMProxy.persistentConfigurationArea.header);
	coreNVMWrite(kNVM0, address, &data, sizeof(PersistentConfigurationData));
    validatePersistentArea(kNVM0, InternalEEPROMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
#else
	int32	address((int32)&FRAMProxy.persistentConfigurationArea.configuration);

    invalidatePersistentArea(kNVM2, FRAMProxy.persistentConfigurationArea.header);
	coreNVMWrite(kNVM2, address, &data, sizeof(PersistentConfigurationData));
    validatePersistentArea(kNVM2, FRAMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
#endif
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "writePersistentConfigurationDataArea done" << newline;
	controller.asc0.unlock();
#endif
	
	return result;
	}

/** Fehler im NVM protokollieren */
void PersistenceServices::logError(uint16 error, uint16 annotation)
	{
#if !defined(NO_PERSISTENCE)
	errorLogEntry	newError;
	uint16	numberOfErrors;
	uint16	currentErrorIndex;
	uint32	currentErrorCounter;
	
	newError.errorID = error;
	newError.annotation = annotation;
	newError.timestamp = coreSystemTimerTicks();
#ifdef NSD_SUPPORT
	newError.totalUpTime = NSD.getACFlightInfoUTCValue();
#else
	newError.totalUpTime = getUptime();
#endif
	coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.numberOfErrors, &numberOfErrors, sizeof(numberOfErrors));
	coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.currentErrorIndex, &currentErrorIndex, sizeof(currentErrorIndex));
	coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.errorCounters[error], &currentErrorCounter, sizeof(currentErrorCounter));

	if (numberOfErrors == 0)
		{
		coreNVMWrite(kNVM1, (int32)&EEPROMProxy.errorArea.firstError, &newError, sizeof(newError));
		currentErrorIndex = 0;
		}
	else
		currentErrorIndex++;
		
	currentErrorIndex &= (kPersistentErrorAreaSize - 1);
	numberOfErrors++;
	currentErrorCounter++;

	coreNVMWrite(kNVM1, (int32)&EEPROMProxy.errorArea.errors[currentErrorIndex], &newError, sizeof(newError));
	coreNVMWrite(kNVM1, (int32)&EEPROMProxy.errorArea.numberOfErrors, &numberOfErrors, sizeof(numberOfErrors));
	coreNVMWrite(kNVM1, (int32)&EEPROMProxy.errorArea.currentErrorIndex, &currentErrorIndex, sizeof(currentErrorIndex));
	coreNVMWrite(kNVM1, (int32)&EEPROMProxy.errorArea.errorCounters[error], &currentErrorCounter, sizeof(currentErrorCounter));
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "logError: currentErrorIndex == " << currentErrorIndex
					<< ", numberOfErrors == " << numberOfErrors << ", currentErrorCounter == " << currentErrorCounter << newline;
	controller.asc0.unlock();
#endif
#endif
	}

/** Anzahl gespeicherter Fehler auslesen */
int16 PersistenceServices::getErrorCount(void) const
	{
#if !defined(NO_PERSISTENCE)
	int16	numberOfErrors;

	coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.numberOfErrors, &numberOfErrors, 2);
	return numberOfErrors;
#else
	return 0;
#endif
	}

uint8 PersistenceServices::getErrorCounter(uint32 counter[])
	{
#if !defined(NO_PERSISTENCE)
	coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.errorCounters, counter, sizeof(EEPROMProxy.errorArea.errorCounters));
#endif	
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "getErrorCounter: " << (uint16)kGAINErrorCount << newline;
	controller.asc0.unlock();
#endif
	return kGAINErrorCount;
	}

int16 PersistenceServices::getCurrentErrorIndex(void) const
	{
#if !defined(NO_PERSISTENCE)
	int16	currentErrorIndex;

	coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.currentErrorIndex, &currentErrorIndex, 2);
	return currentErrorIndex;
#else
	return 0;
#endif
	}

/** Gespeicherten Fehler auslesen */
bool PersistenceServices::getError(int16 index, errorLogEntry& error)
	{
	bool	result(false);
#if !defined(NO_PERSISTENCE)
	int16	numberOfErrors;
	
	coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.numberOfErrors, &numberOfErrors, 2);
	
	if (index < 0)
		{
		coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.firstError, &error, sizeof(error));
		
		result = true;
		}
	else if ((index < numberOfErrors) && (index < kPersistentErrorAreaSize))
		{
		coreNVMRead(kNVM1, (int32)&EEPROMProxy.errorArea.errors[index], &error, sizeof(error));
		
		result = true;
		}
#endif
	return result;
	}

bool PersistenceServices::validWorkingStateForGAINAvailable(tick_t timerValue, bool resumeLastStateValid)
	{
	bool		result(false);
	uint8		i;
	int			currentBuild;
	uint8		currentState;

	for (i = 0; !result && (i < 2); i++)
		{
	#if defined(NO_PERSISTENCE)
		result = isPersistentAreaValid(kNVM0, InternalEEPROMProxy.cateringStateArea[i].header, sizeof(GAINWorkingState));
	#elif defined(MGS_ZST_Revision)
		result = isPersistentAreaValid(kNVM0, RAMProxy.cateringStateArea[i].header, sizeof(GAINWorkingState));
	#else
		result = isPersistentAreaValid(kNVM2, FRAMProxy.cateringStateArea[i].header, sizeof(GAINWorkingState));
	#endif
			
		if (result)
			{
		#if __DEBUG__ > 3
			controller.asc0.lock();
			controller.asc0 << "timer: " << stream::time2string(timerValue, true) << newline;
			controller.asc0.unlock();
		#endif
			result = (timerValue > 0);
		#if (__DEBUG__ > 3)
			if (!result)
				{
				controller.asc0 << "timer expired << newline";
				}
		#endif

			if (result && !resumeLastStateValid)
				{
			#if defined(NO_PERSISTENCE)
				int32	address((int32)&InternalEEPROMProxy.cateringStateArea[i].GAINWorkingStateSave.currentState);
				coreNVMRead(kNVM0, address, &currentState, sizeof(uint8));
			#elif defined(MGS_ZST_Revision)
				int32	address((int32)&RAMProxy.cateringStateArea[i].GAINWorkingStateSave.currentState);
				coreNVMRead(kNVM0, address, &currentState, sizeof(uint8));
			#else
				int32	address((int32)&FRAMProxy.cateringStateArea[i].GAINWorkingStateSave.currentState);
				coreNVMRead(kNVM2, address, &currentState, sizeof(uint8));
			#endif
				result = ((currentState >= kGAINStateDelayed) && (currentState < kGAINStateFinished));
			#if (__DEBUG__ > 3)
				controller.asc0 << "currentState == " << currentState << newline;
				if (!result)
					{
					controller.asc0 << "no valid state for power resume << newline";
					}
			#endif
				}

			if (result)
				{
			#if defined(NO_PERSISTENCE)
				int32	address((int32)&InternalEEPROMProxy.cateringStateArea[i].GAINWorkingStateSave.build);
				coreNVMRead(kNVM0, address, &currentBuild, sizeof(int));
			#else
			#if defined(MGS_ZST_Revision)
				int32	address((int32)&RAMProxy.cateringStateArea[i].GAINWorkingStateSave.build);
				coreNVMRead(kNVM0, address, &currentBuild, sizeof(int));
			#else
				int32	address((int32)&FRAMProxy.cateringStateArea[i].GAINWorkingStateSave.build);
				coreNVMRead(kNVM2, address, &currentBuild, sizeof(int));
			#endif
			#endif
				result = (currentBuild == build);
				if (!result)
					{
					invalidatePersistentConfigurationDataArea();
				#if (__DEBUG__ > 3)
					controller.asc0 << "state.build is " << currentBuild << ", build  is " << build << newline;
				#endif
					}
				}
		
			if (result)
				lastValidSaveWorkingStateForGAINIndex = i;
			else
				clearWorkingStateForGAIN();
		#if (__DEBUG__ > 3)
			if (!result)
				{
				controller.asc0 << ": clearing working state!" << newline;
				}
		#endif
			}
		}

#if __DEBUG__ > 0
	controller.asc0.lock();
	controller.asc0 << "validWorkingStateForGAINAvailable: " << (result?"yes":"no");
	if (result)
		controller.asc0 << " @" << lastValidSaveWorkingStateForGAINIndex;
	controller.asc0 << newline;
	controller.asc0.unlock();
#endif

	return result;
	}

void PersistenceServices::saveWorkingStateForGAIN(GAINWorkingState& v)
	{
	uint8	nextValidSaveWorkingStateForGAINIndex((lastValidSaveWorkingStateForGAINIndex + 1) % 2);

#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "saveWorkingStateForGAIN @" << nextValidSaveWorkingStateForGAINIndex
					<< "( was: " << lastValidSaveWorkingStateForGAINIndex << ")"
					<< " state is " << v.currentState << newline;
	controller.asc0.unlock();
#endif

#if defined(NO_PERSISTENCE)
	//	Get catering state save area base address
	int32	address((int32)&InternalEEPROMProxy.cateringStateArea[nextValidSaveWorkingStateForGAINIndex].GAINWorkingStateSave);

/*
	//	Invalidate catering state save area
	invalidatePersistentArea(kNVM0, InternalEEPROMProxy.cateringStateArea[nextValidSaveWorkingStateForGAINIndex].header);
*/
	//	Write catering state save area
	coreNVMWrite(kNVM0, address, &v, sizeof(GAINWorkingState));
	//	Validate catering state save area
	validatePersistentArea(kNVM0, InternalEEPROMProxy.cateringStateArea[nextValidSaveWorkingStateForGAINIndex].header, sizeof(GAINWorkingState));
	//	Invalidate previous catering state save area
	invalidatePersistentArea(kNVM0, InternalEEPROMProxy.cateringStateArea[lastValidSaveWorkingStateForGAINIndex].header);
#else
#if defined(MGS_ZST_Revision)
	//	Get catering state save area base address
	int32	address((int32)&RAMProxy.cateringStateArea[nextValidSaveWorkingStateForGAINIndex].GAINWorkingStateSave);

	//	Write catering state save area
	coreNVMWrite(kNVM0, address, &v, sizeof(GAINWorkingState));
	//	Validate catering state save area
	validatePersistentArea(kNVM0, RAMProxy.cateringStateArea[nextValidSaveWorkingStateForGAINIndex].header, sizeof(GAINWorkingState));
	//	Invalidate previous catering state save area
	invalidatePersistentArea(kNVM0, RAMProxy.cateringStateArea[lastValidSaveWorkingStateForGAINIndex].header);
#else
	//	Get catering state save area base address
	int32	address((int32)&FRAMProxy.cateringStateArea[nextValidSaveWorkingStateForGAINIndex].GAINWorkingStateSave);

	//	Write catering state save area
	coreNVMWrite(kNVM2, address, &v, sizeof(GAINWorkingState));
	//	Validate catering state save area
	validatePersistentArea(kNVM2, FRAMProxy.cateringStateArea[nextValidSaveWorkingStateForGAINIndex].header, sizeof(GAINWorkingState));
	//	Invalidate previous catering state save area
	invalidatePersistentArea(kNVM2, FRAMProxy.cateringStateArea[lastValidSaveWorkingStateForGAINIndex].header);
#endif
#endif
	//	Update valid working state index	
	lastValidSaveWorkingStateForGAINIndex = nextValidSaveWorkingStateForGAINIndex;

#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "...done" << newline;
	controller.asc0.unlock();
#endif
	}

void PersistenceServices::restoreWorkingStateForGAIN(GAINWorkingState& v)
	{
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "restoreWorkingStateForGAIN" << "@" << lastValidSaveWorkingStateForGAINIndex << newline;
	controller.asc0.unlock();
#endif
	int32	address;

#if defined(NO_PERSISTENCE)
	address = (int32)&InternalEEPROMProxy.cateringStateArea[lastValidSaveWorkingStateForGAINIndex].GAINWorkingStateSave;
	coreNVMRead(kNVM0, address, &v, sizeof(GAINWorkingState));
#else
	#if defined(MGS_ZST_Revision)
	address = (int32)&RAMProxy.cateringStateArea[lastValidSaveWorkingStateForGAINIndex].GAINWorkingStateSave;
	coreNVMRead(kNVM0, address, &v, sizeof(GAINWorkingState));
	#else
	address = (int32)&FRAMProxy.cateringStateArea[lastValidSaveWorkingStateForGAINIndex].GAINWorkingStateSave;
	coreNVMRead(kNVM2, address, &v, sizeof(GAINWorkingState));
	#endif
#endif
	}

void PersistenceServices::clearWorkingStateForGAIN(void)
	{
	uint8		i;

	for (i = 0; i < 2; i++)
		{
	#if __DEBUG__ > 3
		controller.asc0.lock();
		controller.asc0 << "clearWorkingStateForGAIN" << "@" << i << newline;
		controller.asc0.unlock();
	#endif
	#if defined(NO_PERSISTENCE)
		invalidatePersistentArea(kNVM0, InternalEEPROMProxy.cateringStateArea[i].header);
	#else
	#if defined(MGS_ZST_Revision)
		invalidatePersistentArea(kNVM0, RAMProxy.cateringStateArea[i].header);
	#else
		invalidatePersistentArea(kNVM2, FRAMProxy.cateringStateArea[i].header);
	#endif
	#endif
		}

	lastValidSaveWorkingStateForGAINIndex = 0;	
	}

/** Error List im NVM löschen  */
void PersistenceServices::initializeErrorArea(void)
	{
#if !defined(NO_PERSISTENCE)
    eraseNVMArea(kNVM1, (int32)&EEPROMProxy.errorArea.header,
					sizeof(EEPROMProxy.errorArea) - sizeof(EEPROMProxy.errorArea.errors));
	validatePersistentArea(kNVM1, EEPROMProxy.errorArea.header, sizeof(persistentErrorArea));
#endif
	}

/** Usage area im NVM löschen  */
void PersistenceServices::initializeUsageArea(void)
	{
#if defined(NO_PERSISTENCE)
	initializeNVMArea(kNVM0, InternalEEPROMProxy.usageDataArea.header, sizeof(persistentUsageDataArea));
#else
	initializeNVMArea(kNVM2, FRAMProxy.usageDataArea.header, sizeof(persistentUsageDataArea));
#endif
	}

/** Maintenance area im NVM löschen  */
void PersistenceServices::initializeMaintenanceArea(void)
	{
/*
#if __PARTNUMBER == 920
#if defined(NO_PERSISTENCE)
	initializeNVMArea(kNVM0, InternalEEPROMProxy.maintenanceArea.header, sizeof(persistentMaintenanceArea));
#else
	initializeNVMArea(kNVM2, FRAMProxy.maintenanceArea.header, sizeof(persistentMaintenanceArea));
#endif
#endif
*/
#if PARTNUMBER == 920
	clearUsageTime(kUsageTimeHeaterOnWithoutMaintenance);
#endif
	}

/** NVM überprüfen und ggfs. reparieren  */
void PersistenceServices::checkAndRepairNVMConsistency(void)
	{
#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "checkAndRepairNVMConsistency" << newline;
	controller.asc0.unlock();
#endif
#ifdef MGS_ZST_Revision
	checkAndRepairNVMArea(kNVM1, EEPROMProxy.configurationArea.header, sizeof(ConfigurationDataArea));
	checkAndRepairNVMArea(kNVM1, EEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#else
	checkAndRepairNVMArea(kNVM0, InternalEEPROMProxy.configurationArea.header, sizeof(ConfigurationDataArea));
	checkAndRepairNVMArea(kNVM0, InternalEEPROMProxy.cycleArea.header, sizeof(CycleDataArea));
#endif
#if defined (NO_PERSISTENCE)
	checkAndRepairNVMArea(kNVM0, InternalEEPROMProxy.usageDataArea.header, sizeof(persistentUsageDataArea));
	checkAndRepairNVMArea(kNVM0, InternalEEPROMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
#else
	checkAndRepairNVMArea(kNVM1, EEPROMProxy.errorArea.header, sizeof(persistentErrorArea));
	checkAndRepairNVMArea(kNVM2, FRAMProxy.usageDataArea.header, sizeof(persistentUsageDataArea));
	checkAndRepairNVMArea(kNVM2, FRAMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
#endif
#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "...done" << newline;
	controller.asc0.unlock();
#endif
	}

/*
	protected section
*/

/*
	private section
*/
uint8 PersistenceServices::getUsageData(uint32 usage[])
	{
#if PARTNUMBER == 922
	uint8	nUsageCounters(kNumberOfUsageTimeCounters + 3 + kNumberOfUsageCounters);
#else
	uint8	nUsageCounters(kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions + kNumberOfUsageCounters);
#endif
#if defined(NO_PERSISTENCE)
	int32	address((int32)&(InternalEEPROMProxy.usageDataArea.usageTime));
	coreNVMRead(kNVM0, address, usage, sizeof(uint32)*(uint32)nUsageCounters);
#else
	int32	address((int32)&(FRAMProxy.usageDataArea.usageTime));
	coreNVMRead(kNVM2, address, usage, sizeof(uint32)*(uint32)nUsageCounters);
#endif	
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "getUsageData: " << nUsageCounters << newline;
	controller.asc0.unlock();
#endif
	return nUsageCounters;
	}

uint32 PersistenceServices::getUsageTime(uint8 counter) const
	{
	uint32	seconds;
#if defined(NO_PERSISTENCE)
	int32	address((int32)&(InternalEEPROMProxy.usageDataArea.usageTime[counter]));
	coreNVMRead(kNVM0, address, &seconds, 4);
#else
	int32	address((int32)&(FRAMProxy.usageDataArea.usageTime[counter]));
	coreNVMRead(kNVM2, address, &seconds, 4);
#endif
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "getUsageTime: " << counter << ", seconds: " << seconds << newline;
	controller.asc0.unlock();
#endif
	return seconds;
	}

void PersistenceServices::clearUsageTime(uint8 counter)
	{
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "clearUsageTime: " << counter << newline;
	controller.asc0.unlock();
#endif

	int32	seconds(0);
#if defined(NO_PERSISTENCE)
	int32	address((int32)&(InternalEEPROMProxy.usageDataArea.usageTime[counter]));
	coreNVMWrite(kNVM0, address, &seconds, 4);
#else
	int32	address((int32)&(FRAMProxy.usageDataArea.usageTime[counter]));
	coreNVMWrite(kNVM2, address, &seconds, 4);
#endif
	}

void PersistenceServices::updateUsageTime(uint8 counter, bool start)
	{
	tick_t	now(coreSystemTimerTicks());
	
	if (start)
		lastUpdate[counter] = 0;
  	
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "updateUsageTime: " << counter << ", start: " << (start?"true":"false");
	controller.asc0 << ", lastUpdate: " << lastUpdate[counter];
	controller.asc0 << ", now: " << now << newline;
	controller.asc0.unlock();
#endif
	if (lastUpdate[counter])
		{
		int32	deltaSeconds(now - lastUpdate[counter]);

	#if __DEBUG__ > 4
		controller.asc0.lock();
		controller.asc0 << "updateUsageTime deltaSeconds: " << deltaSeconds << newline;
		controller.asc0.unlock();
	#endif
		if (deltaSeconds >= 1000)
			{
			int32	seconds;
		#if defined(NO_PERSISTENCE)
			int32	address((int32)&(InternalEEPROMProxy.usageDataArea.usageTime[counter]));
			coreNVMRead(kNVM0, address, &seconds, 4);
			seconds += deltaSeconds/1000;
			coreNVMWrite(kNVM0, address, &seconds, 4);
		#else
			int32	address((int32)&(FRAMProxy.usageDataArea.usageTime[counter]));
			coreNVMRead(kNVM2, address, &seconds, 4);
			seconds += deltaSeconds/1000;
			coreNVMWrite(kNVM2, address, &seconds, 4);
		#endif
			lastUpdate[counter] = now - (deltaSeconds%1000);
		#if __DEBUG__ > 4
			controller.asc0.lock();
			controller.asc0 << "updateUsageTime seconds: " << seconds << newline;
			controller.asc0.unlock();
		#endif
			}
		}
	else
		lastUpdate[counter] = now;
	}

#ifdef PERSISENT_USAGE_COUNTER_SUPPORT
uint32 PersistenceServices::getUsageCounter(uint8 counter) const
	{
	uint32	value;
#if defined(NO_PERSISTENCE)
	int32	address((int32)&(InternalEEPROMProxy.usageDataArea.usageCounter[counter]));
	coreNVMRead(kNVM0, address, &value, 4);
#else
	int32	address((int32)&(FRAMProxy.usageDataArea.usageCounter[counter]));
	coreNVMRead(kNVM2, address, &value, 4);
#endif
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "getUsageCounter: " << counter << ", value: " << value << newline;
	controller.asc0.unlock();
#endif
	return value;
	}

void PersistenceServices::clearUsageCounter(uint8 counter)
	{
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "clearUsageCounter: " << counter << newline;
	controller.asc0.unlock();
#endif

	uint32	value(0);
#if defined(NO_PERSISTENCE)
	int32	address((int32)&(InternalEEPROMProxy.usageDataArea.usageCounter[counter]));
	coreNVMWrite(kNVM0, address, &value, 4);
#else
	int32	address((int32)&(FRAMProxy.usageDataArea.usageCounter[counter]));
	coreNVMWrite(kNVM2, address, &value, 4);
#endif
	}

void PersistenceServices::updateUsageCounter(uint8 counter, int32 increment)
	{
#if __DEBUG__ > 2
	controller.asc0.lock();
	controller.asc0 << "updateUsageCounter: " << counter << newline;
	controller.asc0.unlock();
#endif
	uint32	value;
#if defined(NO_PERSISTENCE)
	int32	address((int32)&(InternalEEPROMProxy.usageDataArea.usageCounter[counter]));
	coreNVMRead(kNVM0, address, &value, 4);
	value += increment;
	coreNVMWrite(kNVM0, address, &value, 4);
#else
	int32	address((int32)&(FRAMProxy.usageDataArea.usageCounter[counter]));
	coreNVMRead(kNVM2, address, &value, 4);
#if __DEBUG__ > 2
	controller.asc0.lock();
	controller.asc0 << "updateUsageCounter: value before " << value << newline;
	controller.asc0.unlock();
#endif
	value += increment;
#if __DEBUG__ > 2
	controller.asc0.lock();
	controller.asc0 << "updateUsageCounter: value after " << value << newline;
	controller.asc0.unlock();
#endif
	coreNVMWrite(kNVM2, address, &value, 4);
#endif
	}
#endif

bool PersistenceServices::isPersistentAreaValid(uint8 nvm, NVMHeader& adr, uint32 size)
	{
	NVMHeader	header;
	bool		result;

	coreNVMRead(nvm, (int32)&adr, &header, sizeof(header));
	result = (header.magic == validMagic) && (header.size == size);
	
#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "isPersistentAreaValid " << (result?"true":"false") << ' ' << nvm << '@' << kModeHex << &adr << ": magic == " << kModeHex << header.magic << ", size == " << header.size << newline;
	if (!result)
		{
		controller.asc0 << "header.magic == " << kModeHex << header.magic << ", expected " << kModeHex << validMagic  << newline;
		controller.asc0 << "header.size == " << header.size << ", expected " << size << newline;
		}
	controller.asc0.unlock();
#endif
	return result;
	}

void PersistenceServices::invalidatePersistentArea(uint8 nvm, NVMHeader& adr)
	{
	NVMHeader	header;

	header.magic = 0;
	header.size = 0;

	coreNVMWrite(nvm, (int32)&adr, &header, sizeof(header));
	
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "invalidatePersistentArea " << nvm << '@' << kModeHex << &adr << ": magic == " << kModeHex << header.magic << ", size == " << header.size << newline;
	controller.asc0.unlock();
#endif
	}

void PersistenceServices::validatePersistentArea(uint8 nvm, NVMHeader& adr, uint32 size)
	{
	NVMHeader	header;

	header.magic = validMagic;
	header.size = size;

	coreNVMWrite(nvm, (int32)&adr, &header, sizeof(header));
	
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "validatePersistentArea " << nvm << '@' << kModeHex << &adr << ": magic == " << kModeHex << header.magic << ", size == " << header.size << newline;
	controller.asc0.unlock();
#endif
	}

/** Area im NVM überprüfen und ggfs. löschen und magic/size setzen */
void PersistenceServices::checkAndRepairNVMArea(uint8 nvm, NVMHeader& header, uint32 size)
	{
#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "checkAndRepairNVMArea: kNVM" << nvm << " " << kModeHex << &header << ":" << (int32)size << " byte" << newline;
	controller.asc0.unlock();
#endif
/*
	coreNVMRead(nvm, adr, &magic, sizeof(magic));
	coreNVMRead(nvm, adr + sizeof(magic), &size, sizeof(size));
*/
	if (!isPersistentAreaValid(nvm, header, size))
		{
		initializeNVMArea(nvm, header, size);
		}
#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "done." << newline;
	controller.asc0.unlock();
#endif
	}
 	
void PersistenceServices::eraseNVMArea(uint8 nvm, int32 adr, uint32 size)
	{
	int32	end(adr + size);
	int32	address;
	int32	value[4] = {0,0,0,0};

	for (address = adr + sizeof(NVMHeader); (address + 16) < end; address++)
		{
		coreNVMWrite(nvm, address, &value, 16);
		}
	for (; address < end; address++)
		{
		coreNVMWrite(nvm, address, &value, 1);
		}
	}

/** Area im NVM löschen und magic/size setzen */
void PersistenceServices::initializeNVMArea(uint8 nvm, NVMHeader& header, uint32 size)
	{
#if __DEBUG__ > 3
	controller.asc0.lock();
	controller.asc0 << "initializeNVMArea: kNVM" << nvm << " " << kModeHex << &header << ":" << (int32)size << " byte" << newline;
	controller.asc0.unlock();
#endif
    eraseNVMArea(nvm, (int32)&header, size);
	validatePersistentArea(nvm, header, size);
/*
	address = start;
	coreNVMWrite(nvm, address, &validMagic, sizeof(validMagic));
	address = start + sizeof(validMagic);
	areaSize = n;
	coreNVMWrite(nvm, address, &areaSize, sizeof(areaSize));
*/
	}

