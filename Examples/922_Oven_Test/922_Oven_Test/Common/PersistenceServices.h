/*--------------------------------------------------------------------------

PersistenceServices.h

Interface
Zugriff auf persistente Speicherbereiche: software customization area, hardware description area,
	heating program area,...
Persistentes Speichern von Heizzustand, Betriebsstunden,...

Author: Steffen Simon

$Date: 2018-06-07 09:51:15 +0200 (Do, 07. Jun 2018) $
$Revision: 7930 $

--------------------------------------------------------------------------*/
#ifndef PERSISTENCE_SERVICES_H
#define PERSISTENCE_SERVICES_H

#include "error.h"
#include "GAINClass.h"
#include "ConfigurationData.h"
#include "GAINDefinitions.h"

extern const uint32	validMagic;

#ifdef NSD_SUPPORT
class NSDHandler;
#endif

#define	kPersistentErrorAreaSize	0x100
	
#if (PARTNUMBER == 922)
#define PERSISENT_USAGE_COUNTER_SUPPORT
#elif (PARTNUMBER == 932)
#define PERSISENT_USAGE_COUNTER_SUPPORT
#endif

enum
	{
	kUsageTimeUptime = 0,			/**< Kumlierte Betriebszeit GAIN powered */
	kUsageTimeStateON,				/**< Kumlierte Betriebszeit GAIN state ON */
#if PARTNUMBER == 916
#elif PARTNUMBER == 920
	//kUsageTimeHeaterOnTime,			/**< Kumlierte Betriebszeit des Heaters in Sekunden */
	kUsageTimeHeaterOnWithoutMaintenance,	/**< Betriebszeit GAIN state ON seit letzter Maintenance*/
#elif (PARTNUMBER == 921)||(PARTNUMBER == 928)
	kUsageTimeCompressorOnTime,		/**< Kumlierte Betriebszeit des Kompressors in Sekunden */
	kUsageTimeCondenserFanOnTime,	/**< Kumlierte Betriebszeit des Kompressor-Lüfters in Sekunden */
	kUsageTimeCirculationFanOnTime,	/**< Kumlierte Betriebszeit des Lüfters in Sekunden */
#elif PARTNUMBER == 922
	kUsageTimeFanOnTime,			/**< Kumlierte Betriebszeit des Lüfter in Sekunden */
	kUsageTimeHeaterOnTime,			/**< Kumlierte Betriebszeit des Heaters in Sekunden */
	kUsageTimeSteamerOnTime,		/**< Kumlierte Betriebszeit des Steamers in Sekunden */
#elif PARTNUMBER == 932
#endif
	kNumberOfUsageTimeCounters
	};

enum
	{
#if PARTNUMBER == 921
	kUsageCounterDoorOpen = 0,			/**< Door Open Cycles */
#elif PARTNUMBER == 922
	kUsageCounterDoorOpen = 0,			/**< Door Open Cycles */
#elif PARTNUMBER == 932
	kUsageCounterDoorLatchOpen = 0,		/**< Door Latch Open Cycles */
#endif
	kNumberOfUsageCounters
	};

#if PARTNUMBER == 920
enum
	{
	kCycleCounterWithoutCalcification,
	kNumberOfMaintenanceCounters
	};
#endif

/** \brief Beschreibung FlashROM-Speicherbereich: crc, date, magic, size, time.

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
#if PARTNUMBER == 922
	uint32		usageTime[kNumberOfUsageTimeCounters + 3];		/**< Usage time counters */
#else
	uint32		usageTime[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions];	/**< Usage time counters */
#endif
#ifdef PERSISENT_USAGE_COUNTER_SUPPORT
	uint32		usageCounter[kNumberOfUsageCounters];	/**< Usage counters */
#endif
	} persistentUsageDataArea;

#if PARTNUMBER == 920
typedef struct
	{
	NVMHeader	header;

	uint32		maintenanceCounter;			/**< Cycle counters since last maintenance */
	uint32		maintenanceTimer;			/**< Time since last maintenance */
	} persistentMaintenanceArea;
#endif

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
	GAINWorkingState	GAINWorkingStateSave;
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

#if (PARTNUMBER == 932)
typedef struct
	{
	NVMHeader			header;
	LRUCycles			lru;
	} PersistentLRUCyclesArea;
#endif

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

class PowerManagement;
	
/** \brief Zugriff auf persistente Speicherbereiche im FlashROM und NVM.

 	Dient dem Zugriff auf software customization area, hardware description area,
	heating program area und dem persistentes Speichern von aktuellem Heizzustand.
*/
class PersistenceServices
	{
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
	#if (PARTNUMBER == 932)
		bool	readLRUCyclesArea(LRUCycles&);
		bool	writeLRUCyclesArea(LRUCycles&);
	#endif

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

		/** Uptime auslesen (in Sekunden) */
		uint32	getUptime(void) const;

		/** CateringCycle time auslesen (in Sekunden)  */
		uint32	getCateringCycleTime(void) const;
		uint32	getCateringCycleTime(uint8 mode) const;

	#if PARTNUMBER == 920
		uint32	getHeaterOnTimeWithoutMaintenance(void) const;
	#elif (PARTNUMBER == 921)||(PARTNUMBER == 928)
		uint32	getCompressorOnTime(void) const;
		uint32	getCondenserFanOnTime(void) const;
		uint32	getCirculationFanOnTime(void) const;
	#elif PARTNUMBER == 922
		uint32	getFanOnTime(void) const;
		uint32	getHeaterOnTime(void) const;
		uint32	getSteamerOnTime(void) const;
	#endif
		/** Uptime updaten */
		void	updateUptime(bool start);

		/** CateringCycle time updaten */
		void	updateCateringCycleTime(bool start);
		void	updateCateringCycleTime(uint8 mode, bool start);

	#if PARTNUMBER == 916
	#elif PARTNUMBER == 920
		void	updateHeaterOnTime(bool start);
	#elif (PARTNUMBER == 921)||(PARTNUMBER == 928)
		void	updateCompressorOnTime(bool start);
		void	updateCondenserFanOnTime(bool start);
		void	updateCirculationFanOnTime(bool start);
	#elif PARTNUMBER == 922
		void	updateFanOnTime(bool start);
		void	updateHeaterOnTime(bool start);
		void	updateSteamerOnTime(bool start);
	#endif
	
	#if PARTNUMBER == 921
		void	increaseDoorOpenCounter(void);
	#elif PARTNUMBER == 922
		void	increaseDoorOpenCounter(void);
	#elif PARTNUMBER == 932
		void	increaseDoorLatchOpenCounter(void);
	#endif

		/** Abfrage, liefert True wenn saveStateIsValid */
		bool	validWorkingStateForGAINAvailable(tick_t timerValue, bool resumeLastStateValid);
		
		void	saveWorkingStateForGAIN(GAINWorkingState& v);
		
		void	restoreWorkingStateForGAIN(GAINWorkingState& v);

		/** Zustand im NVM ungültig erklären mit saveStateIsValid */
		void	clearWorkingStateForGAIN(void);

		/** Error List im NVM löschen  */
		void	initializeErrorArea(void);

		/** Usage area im NVM löschen  */
		void	initializeUsageArea(void);

		/** Maintenance area im NVM löschen  */
		void	initializeMaintenanceArea(void);
		
		/** NVM überprüfen und ggfs. reparieren  */
		void	checkAndRepairNVMConsistency(void);

	private:
		uint32	getUsageTime(uint8 counter) const;
		void	clearUsageTime(uint8 counter);
		void	updateUsageTime(uint8 counter, bool start);
 		
	#ifdef PERSISENT_USAGE_COUNTER_SUPPORT
		uint32	getUsageCounter(uint8 counter) const;
		void	clearUsageCounter(uint8 counter);
		void	updateUsageCounter(uint8 counter, int32 increment);
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

	#if PARTNUMBER == 922
		static tick_t	lastUpdate[kNumberOfUsageTimeCounters + 3];	
	#else
		static tick_t	lastUpdate[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions];	
	#endif
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

#if PARTNUMBER == 920
inline uint32 PersistenceServices::getHeaterOnTimeWithoutMaintenance(void) const
	{
	return getUsageTime(kUsageTimeHeaterOnWithoutMaintenance);
	}
#endif

inline uint32 PersistenceServices::getCateringCycleTime(uint8 mode) const
	{
	return getUsageTime(kNumberOfUsageTimeCounters + mode);
	}

#if PARTNUMBER == 922
inline uint32 PersistenceServices::getFanOnTime(void) const
	{
	return getUsageTime(kUsageTimeFanOnTime);
	}

inline uint32 PersistenceServices::getHeaterOnTime(void) const
	{
	return getUsageTime(kUsageTimeHeaterOnTime);
	}

inline uint32 PersistenceServices::getSteamerOnTime(void) const
	{
	return getUsageTime(kUsageTimeSteamerOnTime);
	}
#elif (PARTNUMBER == 921)||(PARTNUMBER == 928)
inline uint32 PersistenceServices::getCompressorOnTime(void) const
	{
	return getUsageTime(kUsageTimeCompressorOnTime);
	}

inline uint32 PersistenceServices::getCondenserFanOnTime(void) const
	{
	return getUsageTime(kUsageTimeCondenserFanOnTime);
	}

inline uint32 PersistenceServices::getCirculationFanOnTime(void) const
	{
	return getUsageTime(kUsageTimeCirculationFanOnTime);
	}
#endif

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

#if PARTNUMBER == 916
#elif PARTNUMBER == 920
inline void	PersistenceServices::updateHeaterOnTime(bool start)
	{
	updateUsageTime(kUsageTimeHeaterOnWithoutMaintenance, start);
	}
#elif (PARTNUMBER == 921)||(PARTNUMBER == 928)
inline void	PersistenceServices::updateCompressorOnTime(bool start)
	{
	updateUsageTime(kUsageTimeCompressorOnTime, start);
	}

inline void	PersistenceServices::updateCondenserFanOnTime(bool start)
	{
	updateUsageTime(kUsageTimeCondenserFanOnTime, start);
	}

inline void	PersistenceServices::updateCirculationFanOnTime(bool start)
	{
	updateUsageTime(kUsageTimeCirculationFanOnTime, start);
	}
#elif PARTNUMBER == 922
inline void	PersistenceServices::updateFanOnTime(bool start)
	{
	updateUsageTime(kUsageTimeFanOnTime, start);
	}

inline void	PersistenceServices::updateHeaterOnTime(bool start)
	{
	updateUsageTime(kUsageTimeHeaterOnTime, start);
	}

inline void	PersistenceServices::updateSteamerOnTime(bool start)
	{
	updateUsageTime(kUsageTimeSteamerOnTime, start);
	}
#endif

#if (PARTNUMBER == 921)
inline void	PersistenceServices::increaseDoorOpenCounter(void)
	{
	updateUsageCounter(kUsageCounterDoorOpen, 1);
	}
#elif (PARTNUMBER == 922)
inline void	PersistenceServices::increaseDoorOpenCounter(void)
	{
	updateUsageCounter(kUsageCounterDoorOpen, 1);
	}
#elif (PARTNUMBER == 932)
inline void	PersistenceServices::increaseDoorLatchOpenCounter(void)
	{
	updateUsageCounter(kUsageCounterDoorLatchOpen, 1);
	}
#endif


extern PersistenceServices	persistent;

#endif // PERSISTENCE_SERVICES_H

