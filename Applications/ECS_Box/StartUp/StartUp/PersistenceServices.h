/*--------------------------------------------------------------------------

PersistenceServices.h

Interface
Zugriff auf persistente Speicherbereiche: software customization area, hardware description area,
	heating program area,...
Persistentes Speichern von Heizzustand, Betriebsstunden,...

Author: Steffen Simon
--------------------------------------------------------------------------*/
#ifndef PERSISTENCE_SERVICES_H
#define PERSISTENCE_SERVICES_H


#include "ConfigurationData.h"
#include "ErrorAnalyer_Class.h"
#include "coreTypes.h"
#include "ConfigState_Class.h"
#include "CRC32_Class.h"

extern const uint32	validMagic;



#define	kPersistentErrorAreaSize	0x100
#define PERSISENT_USAGE_COUNTER_SUPPORT

enum
	{
	kUsageTimeUptime = 0,			/**< Kumlierte Betriebszeit GAIN powered */
	kUsageTimeStateON,				/**< Kumlierte Betriebszeit GAIN state ON */
	kNumberOfUsageTimeCounters
	};

enum
	{
	kUsageCounterDoorLatchOpen = 0,		/**< Door Latch Open Cycles */
    kUsageCounterEmptyCaven,
	kNumberOfUsageCounters
	};
enum
	{
		kNVM0 = 0,	//!	RTC RAM
		kNVM1 = 1,	//!	EEPROM
		kNVM2 = 2,	//!	FRAM
		kNumberOfNVMS
	};

/**!
 * \brief Beschreibung FlashROM-Speicherbereich: crc, date, magic, size, time.

	Dieser Block steht vor den eigentlichen Nutzdaten jedes Speicherblocks.
	Die "Magic number" \ref magic zeigt einen gültigen Bereich an.
	Die Prüfsumme wird vor jedem lesenden Zugriff berechnet und mit  \ref crc
	verglichen.
*/
typedef struct
	{
	uint32		magic;		//< "Magic number"
	uint32		size;		//< Größe des  Speicherblocks in Byte
	uint32		crc;		//< Prüfsumme des  Speicherblocks
	} NVMHeader;

typedef struct
	{
	NVMHeader	header;
	uint32		usageTime[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions];	/**< Usage time counters */

#ifdef PERSISENT_USAGE_COUNTER_SUPPORT
	uint32		usageCounter[kNumberOfUsageCounters];	/**< Usage counters */
#endif
	} persistentUsageDataArea;

typedef struct
	{
	NVMHeader	header;
	uint32		cycleCount[kNumberOfCycleDescriptions];			/**< Cycle counters */
	} persistentCycleCountArea;

typedef struct
	{
	NVMHeader		header;

	errorLogEntry	firstError;
	uint16			numberOfErrors;
	uint16			currentErrorIndex;
	uint32			errorCounters[kGAINErrorCount];
	errorLogEntry	errors[kPersistentErrorAreaSize];
	} persistentErrorArea;

typedef struct
	{
	NVMHeader			header;
	HVACState	GAINWorkingStateSave;
	} persistentWorkingStateArea;

typedef struct
	{
	NVMHeader			header;
	ConfigurationData	configuration;
	} ConfigurationDataArea;

typedef struct
	{
	NVMHeader			header;
	CycleDictionary		dictionary;
	CycleData			cycles;
	} CycleDataArea;

typedef struct
	{
	NVMHeader			header;
	LRUCycles			lru;
	} PersistentLRUCyclesArea;

typedef struct
   {
    NVMHeader			header;
    uint32				errorCountersTs[3];
    } PersistentTemperatureCounterArea;

typedef struct
	{
	NVMHeader			header;
	PersistentConfigurationData	configuration;
	} PersistentConfigurationDataArea;

/** \brief Firmware Version: BuildDate, UpdateDate, updateID
 */
typedef struct
	{
  /** Datum der Firmware-Erstellung */
	char	firmwareBuildDate[14];

  /** Datum des Installierens der Firmware */
	char	firmwareUpdateDate[14];

  /** Kennung der Installation */
	char	updateID[4];	
	} firmwareDescription;
	
typedef struct
{
	//	eCore library private NVM area
	byte							eCorePrivate[0x40];
	//	Application private NVM area
	byte							applicationPrivate[0x40];




} internalEEPROMMemoryLayout;

//	external EEPROM: total 16*4096 bytes
typedef struct
{
	//	eCore library private NVM area
	byte							eCorePrivate[0x40];
	//	Application private NVM area
	byte							applicationPrivate[0x40];

	persistentErrorArea				errorArea;  //!< Stored errors

} externalEEPROMMemoryLayout;   //<! Stuff that shown in Maintenance tool

//	FRAM: total 4096 bytes
typedef struct
{
	//	eCore library private NVM area
	byte							eCorePrivate[0x40];
	//	Application private NVM area
	byte							applicationPrivate[0x40];

	//	Network stack private NVM area
	byte							networkStackPrivate[0x400];

	persistentUsageDataArea			usageDataArea;
	persistentCycleCountArea		cycleCountArea;
	PersistentConfigurationDataArea	persistentConfigurationArea;
	PersistentLRUCyclesArea			persistentLRUCyclesArea;

	PersistentTemperatureCounterArea	persistentTemperaturCounter;

} externalFRAMMemoryLayout;	

/*
 	Dient dem Zugriff auf software customization area, hardware description area,
	heating program area und dem persistentes Speichern von aktuellem Heizzustand.
*/
class PersistenceServices: public	CRC32_Class
	{
		//variables
	public:
		HVACState	lastState,currentState;
		
	private:
		ConfigurationData	configuration;
	public:
		/** Konstruktor */
		PersistenceServices(void);

		/** Auslesen der ConfigurationData aus dem NVM */
		bool	readConfigurationDataArea(ConfigurationData&);

		/** Schreiben der ConfigurationData ins NVM */
		bool	writeConfigurationDataArea(ConfigurationData&);

		bool	readCycleDictionaryArea(CycleDictionary&);
		bool	writeCycleDictionaryArea(CycleDictionary&);

		bool	readCycleDescription(CycleDescription&, uint8 id);
		bool	writeCycleDescription(CycleDescription&, uint8 id);
		bool	readLRUCyclesArea(LRUCycles&);
		bool	writeLRUCyclesArea(LRUCycles&);

		/** Invalidate PersistentConfigurationData */
		void	invalidatePersistentConfigurationDataArea(void);

		/** Auslesen der PersistentConfigurationData aus dem NVM */
		bool	readPersistentConfigurationDataArea(PersistentConfigurationData&);

		/** Schreiben der ConfigurationData ins NVM */
		bool	writePersistentConfigurationDataArea(PersistentConfigurationData&);

		//	Get NSD private date area adress
		uint16	getNSDDataAreaAddress(void) const;
		
		/** Maximale Anzahl zu speichernder Fehler auslesen */
		uint16	getMaximumNumberOfErrorEntries(void) const;

		/** Fehler im NVM protokollieren */
		void	logError(uint16 error, uint16 = 0);

		/** Anzahl aktuell empfangener Fehler auslesen
			Caveat: Number of received Errors may be larger than actual available error storage.
		 */
		int16	getErrorCount(void) const;

		/** Auslesen der Usage times) */
		uint8	getErrorCounter(uint32 counter[]);

		/** Gespeicherten Fehler auslesen */
		bool	getError(int16 index, errorLogEntry& error);

		/** Gespeicherten Fehler auslesen */
		int16	getCurrentErrorIndex(void) const;

		/** Auslesen der Usage times) */
		uint8	getUsageData(uint32 usage[]);

        /** Reads temperatur out of range data form NVM */
        uint8	getTemperatureOutOfRangeData(uint32 usage[]);

		/** Uptime auslesen (in Sekunden) */
		uint32	getUptime(void) const;

		/** CateringCycle time auslesen (in Sekunden)  */
		uint32	getCateringCycleTime(void) const;
		uint32	getCateringCycleTime(uint8 mode) const;

		/** Uptime updaten */
		void	updateUptime(bool start);

		/** CateringCycle time updaten */
		void	updateCateringCycleTime(bool start);
		void	updateCateringCycleTime(uint8 mode, bool start);
	
		void	increaseDoorLatchOpenCounter(void);

        void	increaseEmptyCaven(void);
        uint32  getEmptyCavenCounter(void);

        void    clearEmptyCavenCounter(void);

        void	increaseTempreturCounter(uint8 tmp);

		/** Abfrage, liefert True wenn saveStateIsValid */
		bool	validWorkingStateForGAINAvailable(tick_t timerValue, bool resumeLastStateValid);
		
		void	saveWorkingStateForGAIN(HVACState& v);
		
		void	restoreWorkingStateForGAIN(HVACState& v);

		/** Zustand im NVM ungültig erklären mit saveStateIsValid */
		void	clearWorkingStateForGAIN(void);

		/** Error List im NVM löschen  */
		void	initializeErrorArea(void);

		/** Usage area im NVM löschen  */
		void	initializeUsageArea(void);

        /** Temperature out of range data im NVM lï¿½schen  */
        void	initializeTemperatureErrorArea(void);

		/** Maintenance area im NVM löschen  */
		void	initializeMaintenanceArea(void);
		
		/** NVM überprüfen und ggfs. reparieren  */
		void	checkAndRepairNVMConsistency(void);
		
		/************************************************************************/
		/*		New Members for MemeoryManagment Inheritance                     */
		/************************************************************************/
		bool IsConfigurationDataValid(ConfigurationData& cd);
		bool IsConfigurationDataUpdated(void);
		bool IsStateDataValid(HVACState& as);

	private:
		uint32	getUsageTime(uint8 counter) const;
		void	clearUsageTime(uint8 counter);
		void	updateUsageTime(uint8 counter, bool start);
 		
	#ifdef PERSISENT_USAGE_COUNTER_SUPPORT
		uint32	getUsageCounter(uint8 counter) const;
		void	clearUsageCounter(uint8 counter);
		void	updateUsageCounter(uint8 counter, int32 increment);
        void	updateTemperaturCounter(uint8 counter, int32 increment);
	#endif

		bool 	isPersistentAreaValid(uint8 nvm, NVMHeader& adr, uint32 size);
		void	invalidatePersistentArea(uint8 nvm, NVMHeader& adr);
		void	validatePersistentArea(uint8 nvm, NVMHeader& adr, uint32 size);

		/** Area im NVM überprüfen und ggfs. löschen und magic/size setzen */
		void	checkAndRepairNVMArea(uint8 nvm, NVMHeader& adr, uint32 size);

		/** Erase persistent area in NVM */
		void	eraseNVMArea(uint8 nvm, int32 adr, uint32 size);

		/** Area im NVM löschen und magic/size setzen */
		void	initializeNVMArea(uint8 nvm, NVMHeader& adr, uint32 size);

		static tick_t	lastUpdate[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions];	

		uint8	lastValidSaveWorkingStateForGAINIndex;
	};


inline uint16 PersistenceServices::getNSDDataAreaAddress(void) const
	{
	return 0x80;
	}

inline uint16 PersistenceServices::getMaximumNumberOfErrorEntries(void) const
	{
	return kPersistentErrorAreaSize;
	}

inline uint32 PersistenceServices::getUptime(void) const
	{
	return getUsageTime(kUsageTimeUptime);
	}

inline uint32 PersistenceServices::getCateringCycleTime(void) const
	{
	return getUsageTime(kUsageTimeStateON);
	}


inline uint32 PersistenceServices::getCateringCycleTime(uint8 mode) const
	{
	return getUsageTime(kNumberOfUsageTimeCounters + mode);
	}

inline void	PersistenceServices::updateUptime(bool start)
	{
	updateUsageTime(kUsageTimeUptime, start);
	}

inline void	PersistenceServices::updateCateringCycleTime(bool start)
	{
	updateUsageTime(kUsageTimeStateON, start);
	}

inline void	PersistenceServices::updateCateringCycleTime(uint8 mode, bool start)
	{
	updateUsageTime(kUsageTimeStateON, start);
	updateUsageTime(kNumberOfUsageTimeCounters + mode, start);
	}

inline void	PersistenceServices::increaseDoorLatchOpenCounter(void)
	{
	updateUsageCounter(kUsageCounterDoorLatchOpen, 1);
	}

inline void PersistenceServices::increaseEmptyCaven(void)
{
    updateUsageCounter(kUsageCounterEmptyCaven, 1);
}

inline uint32 PersistenceServices::getEmptyCavenCounter(void)
{
    return getUsageCounter(kUsageCounterEmptyCaven);
}

inline void PersistenceServices::clearEmptyCavenCounter(void)
{
    return clearUsageCounter(kUsageCounterEmptyCaven);
}

/*!
 * brief Function updates nvm value for an temperatur error
 * */
inline void	PersistenceServices::increaseTempreturCounter(uint8 tmp)
    {
    updateTemperaturCounter(tmp, 1);
    }
extern PersistenceServices	persistent;

#endif // PERSISTENCE_SERVICES_H
