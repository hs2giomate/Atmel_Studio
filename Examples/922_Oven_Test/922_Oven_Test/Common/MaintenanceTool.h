/*--------------------------------------------------------------------------

MaintenanceTool.h

Interface
Maintenance tool support

Autor: Steffen Simon

$Date: 2017-09-11 15:33:39 +0200 (Mo, 11. Sep 2017) $
$Revision: 7534 $

--------------------------------------------------------------------------*/
#ifndef MAINTENANCE_TOOL_H
#define MAINTENANCE_TOOL_H

#include "coreTypes.h"

class stream;

#ifndef AVR
#pragma pack(2)
#endif

#define	kMaintenanceProtocolVersion	0

typedef struct
	{
	uint8	command;
	uint8	selector;
	uint16	dataSize;
	} GAINMessageHeader;

#ifndef AVR
#pragma pack()
#endif

enum
	{
	kGAINNotifyState = 'a',
	kGAINNotifyControllerState,
	kGAINNotifyAnalogueValues,
	kGAINNotifyUsageData,
	kGAINNotifyCommandReceived,
	kGAINNotifyQueryReceived,
	};

enum
	{
#if (kMaintenanceProtocolVersion > 0)
	kGAINQueryProtocolInfo = '0',
#endif
	kGAINQueryInfo = '1',
	kGAINQueryConfiguration,
	kGAINQueryCycleDictionary,
	kGAINQueryCycleDescription,
	kGAINQueryFaultLog,
	kGAINQueryFaultLogEntry,
	kGAINQueryFaultDescription,
	kGAINQueryNSDData,
	};

enum
	{
	kGAINCommandConnect = 'A',
	kGAINCommandDisconnect,
	kGAINCommandSetNotificationState,
	kGAINCommandReset,
	kGAINCommandResetNVM,
	kGAINCommandSetConfiguration,
	kGAINCommandSetCycleDictionary,
	kGAINCommandSetCycleDescription,
	kGAINCommandSetControllerState,
	kGAINCommandSetNSDData,
	};

/** \brief Maintenance Tool support
 */
class MaintenanceTool
	{
	public:
		MaintenanceTool(void);
		
		bool	isConnected(void) const;
		void	setConnected(bool isConnected);

		bool	handlePeriodicTask(stream& io, uint8 state, uint8 flags);
		bool	handleCommunication(stream& io);
		
		void	updateAnalogueValue(uint8 idx, float value);

	private:
		bool	GAINNotifyState(stream& io, uint8 state, uint8 flags);
		bool	GAINNotifyControllerState(stream& io);
		bool	GAINNotifyAnalogueValues(stream& io);
		bool	GAINNotifyUsageData(stream&);

		bool	handleGAINCommand(stream& io);
		void	notifyGAINCommandReceived(stream& io, GAINMessageHeader& header, bool result);
				
		bool	handleGAINCommandConnect(stream& io, GAINMessageHeader&);
		bool	handleGAINCommandDisconnect(stream& io, GAINMessageHeader&);
		bool	handleGAINCommandSetNotificationState(stream& io, GAINMessageHeader&);
		bool	handleGAINCommandReset(stream& io, GAINMessageHeader&);
		bool	handleGAINCommandResetNVM(stream& io, GAINMessageHeader& header);
		bool	handleGAINCommandSetControllerState(stream& io, GAINMessageHeader&);
		bool	handleGAINCommandSetConfiguration(stream& io, GAINMessageHeader&);
		bool	handleGAINCommandSetCycleDictionary(stream& io, GAINMessageHeader&);
		bool	handleGAINCommandSetCycleDescription(stream& io, GAINMessageHeader&);
	#ifdef NSD_SUPPORT
		bool	handleGAINCommandSetNSDData(stream& io, GAINMessageHeader&);
	#endif

		bool	handleGAINQuery(stream& io);
		void	notifyGAINQueryReceived(stream& io, GAINMessageHeader& header, bool result);

		bool	handleGAINQueryInfo(stream& io, GAINMessageHeader&);
	#if (kMaintenanceProtocolVersion > 0)
		bool	handleGAINQueryProtocolInfo(stream& io, GAINMessageHeader& header);
	#endif
		bool	handleGAINQueryConfiguration(stream& io, GAINMessageHeader&);
		bool	handleGAINQueryCycleDictionary(stream& io, GAINMessageHeader&);
		bool	handleGAINQueryCycleDescription(stream& io, GAINMessageHeader&);
		bool	handleGAINQueryFaultLog(stream& io, GAINMessageHeader&);
	#if (kMaintenanceProtocolVersion > 0)
		bool	handleGAINQueryFaultLogEntry(stream& io, GAINMessageHeader&, int16 id);
	#endif

		void	setConfigurationData(stream& io);
		void	setPartAndSerialNumberData(stream& io);
		
		uint16	calculateChecksum(const GAINMessageHeader& data);
		uint16	calculateChecksum(uint16 checksum, uint16 size, const void* data);
		
		tick_t	nextMaintenanceSyncTime;
		tick_t	nextMaintenanceUsageTimerSyncTime;
		tick_t	lastMaintenanceMessageReceivedTime;

		bool	maintenanceIsConnected;
		bool	bSendNotifications;

		float	analogueValues[16];
	};

inline bool MaintenanceTool::isConnected(void) const
	{
	return maintenanceIsConnected;
	}

#endif // MAINTENANCE_TOOL_H
