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

/*!< Note: Never change the order without changing it also in m.tool source code!!!
* Used by unit to transmit data to m.tool for example current state or new analoge value.
* Notifications are transmitted frequentlly by handlePeriodicTask */
enum
	{
	kGAINNotifyState = 'a',
	kGAINNotifyControllerState,
	kGAINNotifyAnalogueValues,
	kGAINNotifyUsageData,
	kGAINNotifyCommandReceived,
	kGAINNotifyQueryReceived,
    kGAINNotifyTemperaturOutOfRangeCounterReceived
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
/*!< This commands are send by Ipeco development Tool.
 * If you want to change order don't forget to update it also in m.tool source code.
 * */
enum
	{
	kGAINCommandConnect = 'A',
	kGAINCommandDisconnect,
    kGAINCommandSetNotificationState,   //!< Only for 919 relevant... implemented by ss
    kGAINCommandReset,                  //!< Resets whole controller
    kGAINCommandResetNVM,               //!< Resets non volitale memory. Depending on command in header.sector working state is also set to initial values
    kGAINCommandSetConfiguration,       //!< Used to transmit new configuration data
    kGAINCommandSetCycleDictionary,     //!< Not used in 932
    kGAINCommandSetCycleDescription,    //!< Cycle description are presets that can be set in m.tool (power and time)
    kGAINCommandSetControllerState,     //!< Ports (Pins) can be set or reset (for example: cavity led, fans on off...)
    kGAINCommandSetNSDData,             //!< Editing NSD data
    kGAINCommandSetNSDDataBlock,        //!< Editing NSD data in data blocks (see NSD communication: Data blockes are tranmitted to GNC)
    kGAINCommandSetView                 //!< Enables simplified view or standard view (only 932/933)
	};


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
        bool	GAINNotifyTemperatureData(stream&);

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
		bool 	handleGAINCommandEnableSecondView(stream& io, GAINMessageHeader&);

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

        float	analogueValues[16]; /*!< Stores analogue values of system which are transmitted to m-tool frequently \n
                                        [00] SMPS temperatur average \n
                                        [01] SMPS current temperatur value \n
                                        [02] Exhaust temperatur average \n
                                        [03] Exhaust current temperatur value \n
                                        [04] Magnetron temperatur average \n
                                        [05] Magnetron current temperatur value \n
                                        [06]\n
                                        [07] 12V power value \n
                                        [08] UDC \n
                                        [09] Target power \n
                                        [10] Current UDC \n
                                        [11] Current IDC \n
                                        [12] Current power \n
                                        [13] Phase value 0 \n
                                        [14] Phase value 1 \n
                                        [15] Phase value 2 \n
                                      */
	};

inline bool MaintenanceTool::isConnected(void) const
	{
	return maintenanceIsConnected;
	}

#endif // MAINTENANCE_TOOL_H
