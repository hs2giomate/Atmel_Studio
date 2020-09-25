/* 
* Maintenance_Tool.h
*
* Created: 9/15/2020 1:29:17 PM
* Author: GMateusDP
*/


#ifndef __MAINTENANCE_TOOL_H__
#define __MAINTENANCE_TOOL_H__
#include "coreTypes.h"
#include "Parameters.h"


#define	kMaintenanceProtocolVersion	0
#define MAINTENANCE_TOOL_KEYWORD 0x61626364
#define MAINTENANCE_TOOL_TIMEOUT	10000
#define MAINTENANCE_TOOL_BUFFER_SIZE 64

typedef struct
{
	uint8	command;
	uint8	selector;
	uint16	dataSize;
} GAINMessageHeader;

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
	kGAINQueryParameters,
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
	kGAINCommandWriteParameters,	       //!< Used to transmit new configuration data
	kGAINCommandReadParameters,	       //!< Used to transmit new configuration data
    kGAINCommandSetCycleDictionary,     //!< Not used in 932
    kGAINCommandSetCycleDescription,    //!< Cycle description are presets that can be set in m.tool (power and time)
    kGAINCommandSetControllerState,     //!< Ports (Pins) can be set or reset (for example: cavity led, fans on off...)
	
    kGAINCommandSetNSDData,             //!< Editing NSD data
    kGAINCommandSetNSDDataBlock,        //!< Editing NSD data in data blocks (see NSD communication: Data blockes are tranmitted to GNC)
    kGAINCommandSetView,                 //!< Enables simplified view or standard view (only 932/933)
	kGAINNumberOfCommands
	};
	
	typedef struct
	{
		uint8	PortValue[7];
		uint8	reserved;
		uint8	PWMCount;
		uint8	PWMState;
		uint16	pwm[8];
	} GAINControllerPortState;

	typedef struct
	{
		uint8	analogueCount;
		uint8	reserved;
		float	analogue[];
	} GAINControllerAnalogueValues;


	typedef struct
	{
		uint16	partNumber;
		uint16	subPartNumber;
		uint16	powerBudget;
		uint16	standbyPowerBudget;
		char	cpuSerialNumber[16];
		char	partNumberString[16];
		char	serialNumberString[16];
		char	softwareVersionString[16];
		char	hardwareVersionString[8];
	} DeviceInformation;

	typedef struct
	{
		tick_t	time;
		uint8	state;
		uint8	flags;

		uint8	deviceID;
		uint8	feederID;
		uint8	deviceFlags;
		uint8	networkState;
	} GAINStateNotification;
class Maintenance_Tool
{
//variables
public:
	volatile uint32_t	ticks;
	volatile	bool	is_MTPC_Beaming;
	uint8_t		localBuffer[MAINTENANCE_TOOL_BUFFER_SIZE];
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
		GAINMessageHeader		header;
		GAINStateNotification	notification;
		DeviceInformation	deviceID;
		bool result,gotAccess,gotCommand,gotTask;
		uint16	checksum, checksumProvided;
		userParameters	parameters;
		uint8	controllerState;
		
		uint8_t		cpuSerial[16];
		uint32_t	xorResult,magic;
		uint8_t	i,j,k;
		
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


	bool	handleGAINCommand();
	void	notifyGAINCommandReceived( GAINMessageHeader& header, bool result);
	
	bool	handleGAINCommandConnect( GAINMessageHeader&);
	bool	handleGAINCommandDisconnect( GAINMessageHeader&);
	bool	handleGAINCommandSetNotificationState(GAINMessageHeader&);
	bool	handleGAINCommandReset( GAINMessageHeader&);
	bool	handleGAINCommandResetNVM( GAINMessageHeader& header);
	bool	handleGAINCommandSetControllerState( GAINMessageHeader&);
	bool	handleGAINCommandSetConfiguration(GAINMessageHeader&);
	bool	handleGAINCommandSetCycleDictionary(GAINMessageHeader&);
	bool	handleGAINCommandSetCycleDescription( GAINMessageHeader&);

	bool 	handleGAINCommandEnableSecondView( GAINMessageHeader&);

	bool	handleGAINQuery(void);
	void	notifyGAINQueryReceived( GAINMessageHeader& header, bool result);

	bool	handleGAINQueryInfo( GAINMessageHeader&);
#if (kMaintenanceProtocolVersion > 0)
	bool	handleGAINQueryProtocolInfo( GAINMessageHeader& header);
#endif
	bool	handleGAINQueryConfiguration( GAINMessageHeader&);
	bool	handleGAINQueryParameters( GAINMessageHeader&);
	bool	handleGAINQueryCycleDictionary( GAINMessageHeader&);
	bool	handleGAINQueryCycleDescription( GAINMessageHeader&);
	bool	handleGAINQueryFaultLog( GAINMessageHeader&);
#if (kMaintenanceProtocolVersion > 0)
	bool	handleGAINQueryFaultLogEntry( GAINMessageHeader&, int16 id);
#endif
#ifdef NSD_SUPPORT
	bool	handleGAINCommandSetNSDData(GAINMessageHeader&);
#endif

	bool ReadParameters(GAINMessageHeader& header);
	bool WriteParameters(GAINMessageHeader& header);
	uint16	calculateChecksum(const GAINMessageHeader& data);
	uint16	calculateChecksum(uint16 checksum, uint16 size, const void* data);
	void GetCPUSerialNumber(uint8_t* buffer);
	

}; //Maintenance_Tool
inline bool Maintenance_Tool::isConnected(void) const
{
	return maintenanceIsConnected;
}

#endif //__MAINTENANCE_TOOL_H__
