/* 
* MaintenanceDataHandler.h
*
* Created: 11/17/2020 10:50:08 AM
* Author: GMateusDP
*/


#ifndef __MAINTENANCEDATAHANDLER_H__
#define __MAINTENANCEDATAHANDLER_H__

#include "coreTypes.h"
#include "Parameters.h"


#define	kMaintenanceProtocolVersion	0
#define MAINTENANCE_TOOL_KEYWORD 0x61626364
#define MAINTENANCE_TOOL_TIMEOUT	10000
#define MAINTENANCE_TOOL_BUFFER_SIZE 64



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
	kHVACCommandNotTask=0,	
	kGAINCommandConnect = 'A',
	kGAINCommandDisconnect,
    kGAINCommandSetNotificationState,   //!< Only for 919 relevant... implemented by ss
    kGAINCommandReset,                  //!< Resets whole controller
    kGAINCommandResetNVM,               //!< Resets non volitale memory. Depending on command in header.sector working state is also set to initial values
    kGAINCommandSetConfiguration,       //!< Used to transmit new configuration data
	kGAINCommandWriteParameters,	       //!< Used to transmit new configuration data
	kGAINCommandReadParameters,	       //!< Used to transmit new configuration data
	kHVACCommandSetHeaters,	       //!< Used to transmit new configuration data
	kHVACCommandReadHeaterStatus,	       //!< Used to transmit new configuration data
	kHVACCommandSetEnableFans,	       //!< Used to transmit new configuration data
	kHVACCommandSetPWMFans,	       //!< Used to transmit new configuration data
	kHVACCommandReadTemperatures,	       //!< Used to transmit new configuration data
	kHVACReadPositionFlapperValve,	       //!< Used to transmit new configuration data
	kHVACCommandFlapperValve,	       //!< Used to transmit new configuration data
	kHVACWriteSetpointFlapperValve,	       //!< Used to transmit new configuration data
	kHVACCommandRedStatusFans,	       //!< Used to transmit new configuration data
	
	kHVACCommandReadScavengeStatus,	       //!< Used to transmit new configuration data
	kHVACCommandSetScavenge,	       //!< Used to transmit new configuration data
	
    kGAINCommandSetCycleDictionary,     //!< Not used in 932
    kGAINCommandSetCycleDescription,    //!< Cycle description are presets that can be set in m.tool (power and time)
    kGAINCommandSetControllerState,     //!< Ports (Pins) can be set or reset (for example: cavity led, fans on off...)
	
    kGAINCommandSetNSDData,             //!< Editing NSD data
    kGAINCommandSetNSDDataBlock,        //!< Editing NSD data in data blocks (see NSD communication: Data blockes are tranmitted to GNC)
    kGAINCommandSetView,                 //!< Enables simplified view or standard view (only 932/933)
	kGAINNumberOfCommands
	};
 struct __attribute__((__packed__)) HVACMessageHeader
 {
	 uint32	magic=MAINTENANCE_TOOL_KEYWORD;
	 char	command;
	 uint8_t	task=kHVACCommandNotTask;
 } ;
 struct __attribute__((__packed__)) SingleTaskMessage
 {
	 HVACMessageHeader	header;
	 uint8_t		description;
 };
  struct __attribute__((__packed__)) FullBufferMessage
  {
	  HVACMessageHeader	header;
	  uint8_t		content[MAINTENANCE_TOOL_BUFFER_SIZE-sizeof(HVACMessageHeader)];
  };
  #define FULL_MESSAGE_SIZE MAINTENANCE_TOOL_BUFFER_SIZE-sizeof(HVACMessageHeader)
	
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



class MaintenanceDataHandler
{
//variables
public:
	FullBufferMessage	fullBufferMessage;
protected:
private:
	uint8_t	sizeFullBufferMessage;

//functions
public:
	MaintenanceDataHandler();
	~MaintenanceDataHandler();
	uint8_t CreateFullBufferMessage(uint8_t *localBuff,  uint8_t *data);
protected:
private:
	MaintenanceDataHandler( const MaintenanceDataHandler &c );
	MaintenanceDataHandler& operator=( const MaintenanceDataHandler &c );

}; //MaintenanceDataHandler

#endif //__MAINTENANCEDATAHANDLER_H__
