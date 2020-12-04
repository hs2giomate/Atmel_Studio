/* 
* Maintenance_Tool.h
*
* Created: 9/15/2020 1:29:17 PM
* Author: GMateusDP
*/


#ifndef __MAINTENANCE_TOOL_H__
#define __MAINTENANCE_TOOL_H__

#include "CDC_Class.h"
#include "Commands_Handler.h"

class Maintenance_Tool: private virtual MaintenanceDataHandler, private Commands_Handler
{
//variables
public:
	volatile uint32_t	ticks;
	volatile	bool	is_MTPC_Beaming;
	
	
protected:
private:
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
		HVACMessageHeader		header;
		
		GAINStateNotification	notification;
		DeviceInformation	deviceID;
		bool result,gotAccess,gotCommand,gotTask;
		uint16	checksum, checksumProvided;
		UserParameters	parameters;
		uint8	controllerState;
		
		uint8_t		cpuSerial[16];
		uint32_t	xorResult,magic;
		uint8_t	 lastEnableHeaters,enableHeaters;
		SingleTaskMessage	singleTaskMessage;
		uint8_t		*localBuffer;
		
//functions
public:
	Maintenance_Tool();
	~Maintenance_Tool();
		bool	Init(void);
		bool	isConnected(void) const;
		void	setConnected(bool );
		bool	handlePeriodicTask(uint8 state, uint8 flags);
		bool	handleCommunication(void);
		void	updateAnalogueValue(uint8 idx, float value);
		bool	NotifyConnectionAcknowledge(void);
		uint32_t	SendPeriodicNotifications(void);
		bool	IsAppConnected(void);
		uint8_t	ClearLocalBuffer(void);
protected:
private:

	Maintenance_Tool( const Maintenance_Tool &c );
	Maintenance_Tool& operator=( const Maintenance_Tool &c );
	bool	GAINNotifyState( uint8 state, uint8 flags);
	bool	GAINNotifyControllerState();
	bool	GAINNotifyAnalogueValues();
	bool	GAINNotifyUsageData();
	bool	GAINNotifyTemperatureData();


	bool	handleHVACTask();
	void	notifyGAINCommandReceived( HVACMessageHeader& header, bool result);
	
	bool	handleGAINCommandConnect( HVACMessageHeader&);
	bool	handleGAINCommandDisconnect( HVACMessageHeader&);
	bool	handleGAINCommandSetNotificationState(HVACMessageHeader&);
	bool	handleGAINCommandReset( HVACMessageHeader&);
	bool	handleGAINCommandResetNVM( HVACMessageHeader& header);
	bool	handleGAINCommandSetControllerState( HVACMessageHeader&);
	bool	handleGAINCommandSetConfiguration(HVACMessageHeader&);
	bool	handleGAINCommandSetCycleDictionary(HVACMessageHeader&);
	bool	handleGAINCommandSetCycleDescription( HVACMessageHeader&);

	bool 	handleGAINCommandEnableSecondView( HVACMessageHeader&);

	bool	handleGAINQuery(void);
	void	notifyGAINQueryReceived( HVACMessageHeader& header, bool result);

	bool	handleGAINQueryInfo( HVACMessageHeader&);
#if (kMaintenanceProtocolVersion > 0)
	bool	handleGAINQueryProtocolInfo( HVACMessageHeader& header);
#endif
	bool	handleGAINQueryConfiguration( HVACMessageHeader&);
	bool	handleGAINQueryParameters( HVACMessageHeader&);
	bool	handleGAINQueryCycleDictionary( HVACMessageHeader&);
	bool	handleGAINQueryCycleDescription( HVACMessageHeader&);
	bool	handleGAINQueryFaultLog( HVACMessageHeader&);
#if (kMaintenanceProtocolVersion > 0)
	bool	handleGAINQueryFaultLogEntry( HVACMessageHeader&, int16 id);
#endif
#ifdef NSD_SUPPORT
	bool	handleGAINCommandSetNSDData(HVACMessageHeader&);
#endif

	bool	CommandReadParameters();
	bool	CommandWriteParameters(void);
	uint16	calculateChecksum(const HVACMessageHeader& data);
	uint16	calculateChecksum(uint16 checksum, uint16 size, const void* data);
	void	 GetCPUSerialNumber(uint8_t* buffer);
	bool	CommandSetHeaters(void);	
	bool	CommandReadHeaterStatus();

	bool	CommandReadTemmperatures(void);
	bool	CommandSetFlapperValve(void);
	bool	CommandReadFlapperData();
	bool	CommandSetFlapperPosition(void);
	

}; //Maintenance_Tool

extern   Maintenance_Tool toolApp;


inline bool Maintenance_Tool::isConnected(void) const
{
	return maintenanceIsConnected;
}

#endif //__MAINTENANCE_TOOL_H__
