/**--------------------------------------------------------------------------

@file PersistenceServices.cc

Implementation
Persistentes Speichern von Heizzustand, Betriebsstunden,...

Autor: Steffen Simon
--------------------------------------------------------------------------*/

#include "PersistenceServices.h"
#include "Timer_Class.h"
#include "MemoryManagment_Class.h"

const uint32	validMagic = 0x14031869;


//	internal EEPROM: total 2048 bytes




internalEEPROMMemoryLayout&	InternalEEPROMProxy(*((internalEEPROMMemoryLayout*)NULL));
externalEEPROMMemoryLayout&	EEPROMProxy(*((externalEEPROMMemoryLayout*)NULL));

externalFRAMMemoryLayout&	FRAMProxy(*((externalFRAMMemoryLayout*)NULL));


tick_t PersistenceServices::lastUpdate[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions];	




PersistenceServices::PersistenceServices(void)
	:lastValidSaveWorkingStateForGAINIndex(0)
	{
	}

bool PersistenceServices::readConfigurationDataArea(ConfigurationData& data)
	{
	bool		result(false);



	if (result)
		{

		result &= (data.commons.version == kConfigurationDataVersion);
		result &= (data.commons.size == kConfigurationDataSize);
		result &= (data.commons.partNumber == PARTNUMBER);
		}
	
	return result;
	}

bool PersistenceServices::writeConfigurationDataArea(ConfigurationData& data)
	{
	bool	result(true);

	data.commons.version = kConfigurationDataVersion;
	data.commons.size = kConfigurationDataSize;
	data.commons.partNumber = PARTNUMBER;



	

	return result;
	}

bool PersistenceServices::readCycleDictionaryArea(CycleDictionary& dict)
	{
	bool		result(false);


	if (result)
		{

	
		result = (dict.size == kCycleDataSize) && (dict.version == kCycleDataVersion);
		}


	
	return result;
	}

bool PersistenceServices::writeCycleDictionaryArea(CycleDictionary& dict)
	{
	bool		result(true);

	dict.size = kCycleDataSize;
	dict.version = kCycleDataVersion;

	
	
	return result;
	}

bool PersistenceServices::readCycleDescription(CycleDescription& cycle, uint8 id)
	{
	bool		result(false);



	if (result)
		{


		}

	return result;
	}

bool PersistenceServices::writeCycleDescription(CycleDescription& cycle, uint8 id)
	{
	bool		result(true);



	return result;
}


bool PersistenceServices::readLRUCyclesArea(LRUCycles& lru)
	{
	bool		result;


	result = isPersistentAreaValid(kNVM2, FRAMProxy.persistentLRUCyclesArea.header, sizeof(PersistentLRUCyclesArea));
	if (result)
		{
		int32	address((int32)&FRAMProxy.persistentLRUCyclesArea.lru);

		}



	
	return result;
	}

bool PersistenceServices::writeLRUCyclesArea(LRUCycles& lru)
	{
	bool		result(true);


	int32	address((int32)&FRAMProxy.persistentLRUCyclesArea.lru);

    invalidatePersistentArea(kNVM2, FRAMProxy.persistentLRUCyclesArea.header);
	    validatePersistentArea(kNVM2, FRAMProxy.persistentLRUCyclesArea.header, sizeof(PersistentLRUCyclesArea));


	return result;
	}


void PersistenceServices::invalidatePersistentConfigurationDataArea(void)
	{

	invalidatePersistentArea(kNVM2,FRAMProxy.persistentConfigurationArea.header);

	}

bool PersistenceServices::readPersistentConfigurationDataArea(PersistentConfigurationData& data)
	{
	bool		result;

	result = isPersistentAreaValid(kNVM2, FRAMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
	if (result)
		{
		int32	address((int32)&FRAMProxy.persistentConfigurationArea.configuration);
	
		}

	result &= (data.version == kPersistentConfigurationDataVersion);
	result &= (data.size == kPersistentConfigurationDataSize);
	result &= (data.partNumber == PARTNUMBER);
	return result;
	}
	
bool PersistenceServices::writePersistentConfigurationDataArea(PersistentConfigurationData& data)
	{
	bool		result(true);

	data.version = kPersistentConfigurationDataVersion;
	data.size = kPersistentConfigurationDataSize;
	data.partNumber = PARTNUMBER;

	int32	address((int32)&FRAMProxy.persistentConfigurationArea.configuration);

    invalidatePersistentArea(kNVM2, FRAMProxy.persistentConfigurationArea.header);
	 validatePersistentArea(kNVM2, FRAMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));


	return result;
	}

/** Fehler im NVM protokollieren */
void PersistenceServices::logError(uint16 error, uint16 annotation)
	{

	errorLogEntry	newError;
	uint16	numberOfErrors;
	uint16	currentErrorIndex;
	uint32	currentErrorCounter;
	
	newError.errorID = error;
	newError.annotation = annotation;
	newError.timestamp = eventTimer.Get_ticks();

	newError.totalUpTime = getUptime();

	if (numberOfErrors == 0)
		{
	
		currentErrorIndex = 0;
		}
	else
		currentErrorIndex++;
		
	currentErrorIndex &= (kPersistentErrorAreaSize - 1);
	numberOfErrors++;
	currentErrorCounter++;

	
#if __DEBUG__ > 4
	
	usb<< "logError: currentErrorIndex == " << currentErrorIndex
					<< ", numberOfErrors == " << numberOfErrors << ", currentErrorCounter == " << currentErrorCounter << newline;
	
#endif

	}

/** Anzahl gespeicherter Fehler auslesen */
int16 PersistenceServices::getErrorCount(void) const
	{

	return 0;

	}

uint8 PersistenceServices::getErrorCounter(uint32 counter[])
	{

#if __DEBUG__ > 4
	
	usb << "getErrorCounter: " << (uint16)kGAINErrorCount << newline;

#endif
	return kGAINErrorCount;
	}

int16 PersistenceServices::getCurrentErrorIndex(void) const
	{

	return 0;

	}

/** Gespeicherten Fehler auslesen */
bool PersistenceServices::getError(int16 index, errorLogEntry& error)
	{
	bool	result(false);
#if !defined(NO_PERSISTENCE)
	int16	numberOfErrors;
	

	
	if (index < 0)
		{

		
		result = true;
		}
	else if ((index < numberOfErrors) && (index < kPersistentErrorAreaSize))
		{

		
		result = true;
		}
#endif
	return result;
	}

bool PersistenceServices::validWorkingStateForGAINAvailable(tick_t timerValue, bool resumeLastStateValid)
	{
	bool		result(false);
	uint8		i;
    int			currentBuild = 0;
    uint8		currentState = 0;

	for (i = 0; !result && (i < 2); i++)
    {

	
		if (result)
        {
  

			result = (timerValue > 0);
  

			if (result && !resumeLastStateValid)
				{
		
			
				result = ((currentState >= kGAINStateDelayed) && (currentState < kGAINStateFinished));

				}

            //Check software build (software version): Don't resume in case of power spike during software update
			if (result)
            {
            
             

				if (!result)
                {
					invalidatePersistentConfigurationDataArea();
                    #if (__DEBUG__ > 3)
                       usb<< "state.build is " << currentBuild << ", build  is " << build << newline;
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
				usb << ": clearing working state!" << newline;
				}
		#endif
        }
    }


	return result;
}

void PersistenceServices::saveWorkingStateForGAIN(HVACState& v)
	{
	uint8	nextValidSaveWorkingStateForGAINIndex((lastValidSaveWorkingStateForGAINIndex + 1) % 2);






	//	Write catering state save area
	//	Validate catering state save area

	//	Update valid working state index	
	lastValidSaveWorkingStateForGAINIndex = nextValidSaveWorkingStateForGAINIndex;


	}

void PersistenceServices::restoreWorkingStateForGAIN(HVACState& v)
	{
	int32	address;
	


	}

void PersistenceServices::clearWorkingStateForGAIN(void)
	{
	uint8		i;

	for (i = 0; i < 2; i++)
		{


	

		}

	lastValidSaveWorkingStateForGAINIndex = 0;	
	}

/** Error List im NVM l�schen  */
void PersistenceServices::initializeErrorArea(void)
	{

    eraseNVMArea(kNVM1, (int32)&EEPROMProxy.errorArea.header,
					sizeof(EEPROMProxy.errorArea) - sizeof(EEPROMProxy.errorArea.errors));
	validatePersistentArea(kNVM1, EEPROMProxy.errorArea.header, sizeof(persistentErrorArea));

	}

/** Usage area im NVM l�schen  */
void PersistenceServices::initializeUsageArea(void)
	{

	initializeNVMArea(kNVM2, FRAMProxy.usageDataArea.header, sizeof(persistentUsageDataArea));

	}

/** Usage area im NVM lï¿½schen  */
void PersistenceServices::initializeTemperatureErrorArea(void)
{

    initializeNVMArea(kNVM2, FRAMProxy.persistentTemperaturCounter.header, sizeof(PersistentTemperatureCounterArea));

}

/** Maintenance area im NVM l�schen  */
void PersistenceServices::initializeMaintenanceArea(void)
	{

	}

/** NVM �berpr�fen und ggfs. reparieren  */
void PersistenceServices::checkAndRepairNVMConsistency(void)
	{


	checkAndRepairNVMArea(kNVM1, EEPROMProxy.errorArea.header, sizeof(persistentErrorArea));
	checkAndRepairNVMArea(kNVM2, FRAMProxy.usageDataArea.header, sizeof(persistentUsageDataArea));
	checkAndRepairNVMArea(kNVM2, FRAMProxy.persistentConfigurationArea.header, sizeof(PersistentConfigurationDataArea));
    checkAndRepairNVMArea(kNVM2, FRAMProxy.persistentTemperaturCounter.header, sizeof(PersistentTemperatureCounterArea));


	}

/*
	protected section
*/

/*
	private section
*/
uint8 PersistenceServices::getUsageData(uint32 usage[])
	{

	uint8	nUsageCounters(kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions + kNumberOfUsageCounters);



	return nUsageCounters;
	}


uint8	PersistenceServices::getTemperatureOutOfRangeData(uint32 usage[])
{

	
    int32	address((int32)&(FRAMProxy.persistentTemperaturCounter.errorCountersTs));


    return 0;
}

uint32 PersistenceServices::getUsageTime(uint8 counter) const
	{
	uint32	seconds;

	int32	address((int32)&(FRAMProxy.usageDataArea.usageTime[counter]));



	return seconds;
	}

void PersistenceServices::clearUsageTime(uint8 counter)
	{

	int32	seconds(0);

	int32	address((int32)&(FRAMProxy.usageDataArea.usageTime[counter]));


	}

void PersistenceServices::updateUsageTime(uint8 counter, bool start)
	{
	tick_t	now(0);
	
	if (start)
		lastUpdate[counter] = 0;
  	

	if (lastUpdate[counter])
		{
		int32	deltaSeconds(now - lastUpdate[counter]);


		if (deltaSeconds >= 1000)
			{
			int32	seconds;
		
			int32	address((int32)&(FRAMProxy.usageDataArea.usageTime[counter]));
	
			seconds += deltaSeconds/1000;


			lastUpdate[counter] = now - (deltaSeconds%1000);

			}
		}
	else
		lastUpdate[counter] = now;
	}

#ifdef PERSISENT_USAGE_COUNTER_SUPPORT
uint32 PersistenceServices::getUsageCounter(uint8 counter) const
	{
	uint32	value;

	int32	address((int32)&(FRAMProxy.usageDataArea.usageCounter[counter]));



	return value;
	}

void PersistenceServices::clearUsageCounter(uint8 counter)
	{


	uint32	value(0);

	int32	address((int32)&(FRAMProxy.usageDataArea.usageCounter[counter]));

	}

void PersistenceServices::updateUsageCounter(uint8 counter, int32 increment)
	{

	uint32	value;

	int32	address((int32)&(FRAMProxy.usageDataArea.usageCounter[counter]));

	value += increment;

	}
	
bool PersistenceServices::IsConfigurationDataUpdated()
{
	uint32_t	ver=factoryDefaultsConfiguration.commons.version;
	uint32_t	verUp=memory.ReadUpdatedConfigurationData(configuration);

	if ((verUp>0))
	{
		return false;
	} 
	else
	{
		uint32_t	refCRC=memory.ReadCRCConfigurationData();
		uint32_t	crc=CalculateCRC((uint32_t *)PTR_CONFIG_DATA(&configuration),sizeof(configuration));
		if (refCRC==crc)
		{
			verUp=configuration.commons.version;
			return (verUp>ver);
		} 
		else
		{
			return	false;
		}
		
	}
	return	false;

}
bool PersistenceServices::IsConfigurationDataValid(ConfigurationData& cd)
{
	configuration=cd;
	uint32_t	refCRC=memory.ReadCRCConfigurationData();
	uint32_t	crc=CalculateCRC((uint32_t *)PTR_CONFIG_DATA(&configuration),sizeof(configuration));
	return	 (refCRC==crc);
	
}


void PersistenceServices::updateTemperaturCounter(uint8 counter, int32 increment)
    {

    uint32	value;

    int32	address((int32)&(FRAMProxy.persistentTemperaturCounter.errorCountersTs[counter]));

    value += increment;


    }
#endif


bool PersistenceServices::isPersistentAreaValid(uint8 nvm, NVMHeader& adr, uint32 size)
	{
	NVMHeader	header;
	bool		result;


	result = (header.magic == validMagic) && (header.size == size);
	

	return result;
	}
bool PersistenceServices::IsStateDataValid(HVACState& as)
{
	uint32_t	refCRC=memory.ReadCRCApplicationState();
	uint32_t	crc=CalculateCRC((uint32_t *)PTR_HVAC_STATE(&as),sizeof(lastState));
	return (refCRC==crc);
}
bool PersistenceServices::IsFastStateDataValid(HVACState& as)
{
	uint32_t	refCRC=memory.ReadFastCRCApplicationState();
	uint32_t	crc=CalculateCRC((uint32_t *)PTR_HVAC_STATE(&as),sizeof(HVACState));
	return (refCRC==crc);
}

void PersistenceServices::invalidatePersistentArea(uint8 nvm, NVMHeader& adr)
	{
	NVMHeader	header;

	header.magic = 0;
	header.size = 0;

	}

void PersistenceServices::validatePersistentArea(uint8 nvm, NVMHeader& adr, uint32 size)
	{
	NVMHeader	header;

	header.magic = validMagic;
	header.size = size;


	

	}

/** Area im NVM �berpr�fen und ggfs. l�schen und magic/size setzen */
void PersistenceServices::checkAndRepairNVMArea(uint8 nvm, NVMHeader& header, uint32 size)
	{

/*
	coreNVMRead(nvm, adr, &magic, sizeof(magic));
	coreNVMRead(nvm, adr + sizeof(magic), &size, sizeof(size));
*/
	if (!isPersistentAreaValid(nvm, header, size))
		{
		initializeNVMArea(nvm, header, size);
		}

	}
 	
void PersistenceServices::eraseNVMArea(uint8 nvm, int32 adr, uint32 size)
	{
	int32	end(adr + size);
	int32	address;
	int32	value[4] = {0,0,0,0};

	for (address = adr + sizeof(NVMHeader); (address + 16) < end; address++)
		{

		}
	for (; address < end; address++)
		{
	
		}
	}

/** Area im NVM l�schen und magic/size setzen */
void PersistenceServices::initializeNVMArea(uint8 nvm, NVMHeader& header, uint32 size)
	{

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
