/*--------------------------------------------------------------------------

coreNSDStack.c

Implementation
NSD core functionallity

$Author: steffen $
$Date: 2018-06-06 11:13:54 +0200 (Mi, 06. Jun 2018) $
$Revision: 7928 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Network/coreNSDStack.c $

--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT
#ifdef PARTNUMBER
#include "version.h"
#endif
#include "coreNSDStack.h"


/**	Galley Insert FID
*/
#define	kFIDGalleyInserts ((uint8)123)

/**	Logical Communication Channel (LCC) IDs
*/
#define	kLCCNormalOperationChannel ((uint8)2)
#define	kLCCNodeServiceChannel ((uint8)4)

/**	Data transfer service function codes
*/
#define	kServiceFunctionCodeDataUpload ((uint8)2)
#define	kServiceFunctionCodeDataDownload ((uint8)3)

/**	Message identifiers: including LCC, Source FID, RSD, LCL, PVT and message codes
	Refer to NSD part 2 issue 3 NSD_ATA2536_MN008-01
*/
#define	kMessageIdentifier_STA_MGCU ((uint32)0x0BDA0400UL)			/*  01011110110100000010000000000 */
#define	kMessageIdentifier_STA_GAIN ((uint32)0x0BDA0800UL)			/*  01011110110100000100000000000 */
#define	kMessageIdentifier_PBR ((uint32)0x0BDA0C00UL)				/*  01011110110100000110000000000 */
#define	kMessageIdentifier_PBM ((uint32)0x0BDA1000UL)				/*  01011110110100001000000000000 */
#define	kMessageIdentifier_DIAG_FAILURE ((uint32)0x0BDA1400UL)		/*  01011110110100001010000000000 */
#define	kMessageIdentifier_DIAG_MALFUNCTION ((uint32)0x0BDA1600UL)	/*  01011110110100001011000000000 */
#define	kMessageIdentifier_DIAG_WARNING ((uint32)0x0BDA1800UL)		/*  01011110110100001100000000000 */

#define	kMessageIdentifier_SES_MGCU ((uint32)0x0AC03000UL)			/*  01010110000000011000000000000 */
#define	kMessageIdentifier_SES_GAIN ((uint32)0x0BE83200UL)			/*  01011111010000011001000000000 */

#define	kMessageIdentifier_FLIGHT_INFO ((uint32)0x0BDA3C00UL)		/*  01011110110100011110000000000 */
#define	kMessageIdentifier_GAIN_PT ((uint32)0x0BDA4000UL)			/*  01011110110100100000000000000 */
#define	kMessageIdentifier_DIAG_CMD ((uint32)0x0BDA4400UL)			/*  01011110110100100010000000000 */

#define	kMessageIdentifier_PHSM ((uint32)0x1BEEFC00UL)				/*  11011111011101111110000000000 */

/**	Data transfer message identifiers: including LCC, Source FID, RSD, LCL, PVT and message codes
	Refer to NSD part 2 issue 3 NSD_ATA2536_MN008-02
*/
#define	kMessageIdentifier_DT_REQ ((uint32)0x13DEF600UL)			/*  10011110111101111011000000000 */
#define	kMessageIdentifier_DT_RES ((uint32)0x13DAF600UL)			/*  10011110110101111011000000000 */
#define	kMessageIdentifier_DT_DDL ((uint32)0x13DBF600UL)			/*  10011110110111111011000000000 */
#define	kMessageIdentifier_DT_DUL ((uint32)0x13DFF600UL)			/*  10011110111111111011000000000 */


/** STA_GAIN signal accessor definitions
*/
#define	kGAINModeMask			((uint8)0x0F)
#define	kGAINModeByteOffset		((uint8)0)
#define	kGAINModeBitOffset		((uint8)4)

#define	kGAINStateMask				((uint8)0x0F)
#define	kGAINStateByteOffset		((uint8)0)
#define	kGAINStateBitOffset			((uint8)0)

#define	kGAINUsedPowerBudgetMask		((uint8)0xFF)
#define	kGAINUsedPowerBudgetByteOffset	((uint8)1)
#define	kGAINUsedPowerBudgetBitOffset	((uint8)0)

#define	kGAINFallbackPowerBudgetMask		((uint8)0xFF)
#define	kGAINFallbackPowerBudgetByteOffset	((uint8)2)
#define	kGAINFallbackPowerBudgetBitOffset	((uint8)0)

#define	kGAINPBRemainingTimeMask			((uint8)0xFF)
#define	kGAINPBRemainingTimeMSBByteOffset	((uint8)3)
#define	kGAINPBRemainingTimeLSBByteOffset	((uint8)4)
#define	kGAINPBRemainingTimeBitOffset		((uint8)0)

#define	kGAINImplementedFunctionalitiesMask			((uint8)0xFE)
#define	kGAINImplementedFunctionalitiesByteOffset	((uint8)5)
#define	kGAINImplementedFunctionalitiesBitOffset	((uint8)0)

#define	kGAINGAINIsPrimaryByteOffset		((uint8)6)
#define	kGAINGAINIsPrimaryBitOffset			((uint8)5)

#define	kGAINFeederIDMask					((uint8)0x1F)
#define	kGAINFeederIDByteOffset				((uint8)6)
#define	kGAINFeederIDBitOffset				((uint8)0)

#define	kGAINMGCUIsPresentByteOffset		((uint8)7)
#define	kGAINMGCUIsPresentBitOffset			((uint8)7)

/**
	GAIN_PT signal accessor definitions
*/
#define	kGAINPTStandbyPowerThresholdMask			((uint8)0xFF)
#define	kGAINPTStandbyPowerThresholdByteOffset		((uint8)0)
#define	kGAINPTStandbyPowerThresholdBitOffset		((uint8)0)

#define	kGAINPTMaximumPowerThresholdMask			((uint8)0xFF)
#define	kGAINPTMaximumPowerThresholdByteOffset		((uint8)1)
#define	kGAINPTMaximumPowerThresholdBitOffset		((uint8)0)

#define	kGAINPTMinimumPowerThresholdMask			((uint8)0xFF)
#define	kGAINPTMinimumPowerThresholdByteOffset		((uint8)2)
#define	kGAINPTMinimumPowerThresholdBitOffset		((uint8)0)

#define	kGAINPTFallbackPowerThresholdMask			((uint8)0xFF)
#define	kGAINPTFallbackPowerThresholdByteOffset		((uint8)3)
#define	kGAINPTFallbackPowerThresholdBitOffset		((uint8)0)

/**
	PBR signal accessor definitions
*/
#define	kPBRPowerBudgetTypeByteOffset				((uint8)0)
#define	kPBRPowerBudgetTypeBitOffset				((uint8)7)

#define	kPBRNominalPowerBudgetMask					((uint8)0xFF)
#define	kPBRNominalPowerBudgetByteOffset			((uint8)1)
#define	kPBRNominalPowerBudgetBitOffset				((uint8)0)

#define	kPBRNeededTimeWithNominalPBMask				((uint8)0xFF)
#define	kPBRNeededTimeWithNominalPBMSBByteOffset	((uint8)2)
#define	kPBRNeededTimeWithNominalPBLSBByteOffset	((uint8)3)
#define	kPBRNeededTimeWithNominalPBBitOffset		((uint8)0)

#define	kPBRMinimalPowerBudgetMask					((uint8)0xFF)
#define	kPBRMinimalPowerBudgetByteOffset			((uint8)4)
#define	kPBRMinimalPowerBudgetBitOffset				((uint8)0)

#define	kPBRNeededTimeWithMinimalPBMask				((uint8)0xFF)
#define	kPBRNeededTimeWithMinimalPBMSBByteOffset	((uint8)5)
#define	kPBRNeededTimeWithMinimalPBLSBByteOffset	((uint8)6)
#define	kPBRNeededTimeWithMinimalPBBitOffset		((uint8)0)

/** STA_MGCU signal accessor definitions
*/
#define	kMGCUStateMask								((uint8)0x0F)
#define	kMGCUStateByteOffset						((uint8)0)
#define	kMGCUStateBitOffset							((uint8)4)

#define	kMGCUDisinfectionModeByteOffset				((uint8)0)
#define	kMGCUDisinfectionModeBitOffset				((uint8)3)

#define	kMGCUImplementedFunctionalitiesMask			((uint8)0xFE)
#define	kMGCUImplementedFunctionalitiesByteOffset	((uint8)1)
#define	kMGCUImplementedFunctionalitiesBitOffset	((uint8)0)

#define	kMGCUPowerInfoFeederMask					((uint8)0x1F)
#define	kMGCUPowerInfoFeederByteOffset				((uint8)3)
#define	kMGCUPowerInfoFeederBitOffset				((uint8)3)

#define	kMGCUCentralPowerValidOnFeederByteOffset	((uint8)3)
#define	kMGCUCentralPowerValidOnFeederBitOffset		((uint8)2)

#define	kNSDIdentifierMask1Bit ((uint32)0x01)
#define	kNSDIdentifierMask2Bit ((uint32)0x03)
#define	kNSDIdentifierMask3Bit ((uint32)0x07)
#define	kNSDIdentifierMask7Bit ((uint32)0x7F)

#define	kNSDIdentifierBitShiftSourceLCC ((uint8)26)
#define	kNSDIdentifierBitShiftSourceFID ((uint8)19)
#define	kNSDIdentifierBitShiftRSD ((uint8)18)
#define	kNSDIdentifierBitShiftLCL ((uint8)17)
#define	kNSDIdentifierBitShiftPVT ((uint8)16)
#define	kNSDIdentifierBitShiftMessageCode ((uint8)9)
#define	kNSDIdentifierBitShiftDeviceID ((uint8)2)
#define	kNSDIdentifierBitShiftRCI ((uint8)0)

/**
	PBM signal accessor definitions
*/
#define	kPBMPowerBudgetMaxMask				((uint8)0xFF)
#define	kPBMPowerBudgetMaxByteOffset		((uint8)0)
#define	kPowerBudgetMaxBitOffset			((uint8)0)

#define	kPBMTimeMask						((uint8)0xFF)
#define	kPBMTimeMSBByteOffset				((uint8)1)
#define	kPBMTimeLSBByteOffset				((uint8)2)
#define	kPBMTimeBitOffset					((uint8)0)

/*
	NSD350-V25-36-260
	NSD350-V25-36-152
	NSD350-V25-36-153
*/
const GAINDataBlock01 defaultDataBlock01 =
	{
	1, {'N', 'S', 'D', 'I', '0', '5'}, 0
	};

#if (PARTNUMBER == 916)
const GAINDataBlock03 defaultDataBlock03 =
	{25, 25, 25, 10};
const GAINDataBlock04 defaultDataBlock04 =
	{1, {25}};
const GAINDataBlock05 defaultDataBlock05 =
	{1,1,1,1,1,"9160007051-1-95", '\0', "11-03-0103", "S1", "S1", 1};
#elif (PARTNUMBER == 917)
const GAINDataBlock03 defaultDataBlock03 =
	{87, 87, 87, 10};
const GAINDataBlock04 defaultDataBlock04 =
	{1, {87}};
const GAINDataBlock05 defaultDataBlock05 =
	{1,1,1,1,1,"9170007051-1-95", '\0', "11-03-0101", "S2", "S2", 4};
#elif (PARTNUMBER == 920)
const GAINDataBlock03 defaultDataBlock03 =
	{54, 54, 54, 10};
const GAINDataBlock04 defaultDataBlock04 =
	{1, {54}};
const GAINDataBlock05 defaultDataBlock05 =
	{1,1,1,1,1,"9200007051-1-95", '\0', "3489", "S2", "S2", 1};
#elif (PARTNUMBER == 921)
const GAINDataBlock03 defaultDataBlock03 =
	{15, 15, 15, 5};
const GAINDataBlock04 defaultDataBlock04 =
	{1, {15}};
const GAINDataBlock05 defaultDataBlock05 =
	{1,1,1,1,1,"9210007051-1-95", '\0', "3180", "S2", "S2", 1};
#elif (PARTNUMBER == 922)
const GAINDataBlock03 defaultDataBlock03 =
	{64, 64, 64, 5};
const GAINDataBlock04 defaultDataBlock04 =
	{1, {64}};
const GAINDataBlock05 defaultDataBlock05 =
	{1,1,1,1,1,"9220X07051-1-95", '\0', "3354", "S2", "S2", 1};
#elif (PARTNUMBER == 932)
const GAINDataBlock03 defaultDataBlock03 =
	{25, 25, 25, 10};
const GAINDataBlock04 defaultDataBlock04 =
	{1, {25}};
const GAINDataBlock05 defaultDataBlock05 =
	{1,1,1,1,1,"9320007051-1-95", '\0', "11-03-0103", "S1", "S1", 1};
#elif (PARTNUMBER == 0xBABE)
const GAINDataBlock03 defaultDataBlock03 =
	{15, 15, 15, 10};
const GAINDataBlock04 defaultDataBlock04 =
	{1, {15}};
const GAINDataBlock05 defaultDataBlock05 =
	{1,1,1,1,1,"9XX", '\0', "1", "S1", "S1", 1};
#else
#error "Part number not supported!"
#endif

const GAINDataBlock08	NITDataBlock08 =
	{
	"LNZ",
	"HAM",
	"123456789",
	"12345678901"
	};
	
typedef struct
	{	
	uint32			gainPinCode;
		
	uint8			gainFeederID;
	uint8			gainDeviceID;

	uint8			currentGAINMode;
	uint8			currentGAINState;
		
	bool			flightDataReceived;
	bool			GAINIsPrimary;

	MGCUOperationalState	mgcuState;
    CANMessage				messages[8];
    } coreNSDStruct;


#define	dataBlockHeaderSize	8
#define	dataBlockZeroSize	16

uint16 GAINConfigurationDataBlockOffsetTable[] = {dataBlockHeaderSize, 24, 32, 34, 54, 54, 102, 0xFFFF, 112, 142, 0xFFFF, 0xFFFF, 170};
uint16 GAINConfigurationDataBlockSizeTable[] =   {dataBlockZeroSize,  7,  1,  4, 15, 48, 10,  0,      30,  28,  0,      0,      6};

tick_t	GAINMessageInhibitTimes[kNumberOfGAINMessages];

GAINConfigurationData	configurationData;

//static coreNSDStruct	coreNSD;			
static coreNSDConfigurationStruct	coreNSDConfiguration;

static const uint32	GAINMessageLookup[] =
	{
	kMessageIdentifier_STA_GAIN,
	kMessageIdentifier_PBR,
	kMessageIdentifier_DIAG_FAILURE,
	kMessageIdentifier_DIAG_MALFUNCTION,
	kMessageIdentifier_DIAG_WARNING,
	kMessageIdentifier_SES_GAIN,
	kMessageIdentifier_GAIN_PT,
	kMessageIdentifier_PHSM,
	kMessageIdentifier_DT_RES,
	kMessageIdentifier_DT_DDL
	};

static const uint32	MGCUMessageLookup[] =
	{
	kMessageIdentifier_STA_MGCU,
	kMessageIdentifier_PBM,
	kMessageIdentifier_SES_MGCU,
	kMessageIdentifier_FLIGHT_INFO,
	kMessageIdentifier_DIAG_CMD,
	kMessageIdentifier_DT_REQ,
	kMessageIdentifier_DT_DUL
	};

bool coreNSD_initialize(coreNSDConfigurationStruct* v)
	{
	bool	result = (v != NULL);
	uint8	i;
#ifdef PARTNUMBER
	softwareVersion*	version = (softwareVersion*)&configurationData.block05.SoftwareVersion;
#endif

	if (result)
		{
		result = ((*v).getControllerTicksCallback != NULL) &&
					((*v).CANWriteMessageCallback != NULL) &&
					((*v).readGAINConfigurationDataCallback != NULL) &&
					((*v).writeGAINConfigurationDataCallback != NULL)&&
					((*v).networkStateChangedCallback != NULL) &&
					((*v).powerControlStateChangedCallback != NULL) &&
					((*v).selfTestRequestCallback != NULL);
		}

	if (result)
		{
		uint32	crc32;
		bool	nvmValid = true;
		bool	timerValid = true;
		
		coreNSDConfiguration = *v;
		
		coreNSD_readGAINConfigurationData(0, sizeof(configurationData), &configurationData);
		//crc32 = coreNSD_calculateCRC32(&configurationData.block01, sizeof(configurationData) - 8 - sizeof(configurationData.block00));
		crc32 = 0x1234567;

		if (((*version).build != build) ||
			((*version).major != versionMajor) ||
			((*version).minor != versionMinor) ||
			((*version).bugfix != versionBugfix) ||
			((*version).stage != developmentStage) ||
			((*version).release != release))
			{
			(*version).build = build;
			(*version).major = versionMajor;
			(*version).minor = versionMinor;
			(*version).bugfix = versionBugfix;
			(*version).stage = developmentStage;
			(*version).release = release;
			
			//coreNSDConfiguration.writeGAINConfigurationDataCallback(GAINConfigurationDataBlockOffsetTable[5], GAINConfigurationDataBlockSizeTable[5], &configurationData.block05);

            configurationData.block01 = defaultDataBlock01;
			//coreNSDConfiguration.writeGAINConfigurationDataCallback(GAINConfigurationDataBlockOffsetTable[1], GAINConfigurationDataBlockSizeTable[1], &defaultDataBlock01);

            configurationData.block08 = NITDataBlock08;
			//coreNSDConfiguration.writeGAINConfigurationDataCallback(GAINConfigurationDataBlockOffsetTable[8], GAINConfigurationDataBlockSizeTable[8], &NITDataBlock08);
			
			timerValid = false;
			}

		if (configurationData.block00.partNumber != PARTNUMBER)
			nvmValid = false;

		if (configurationData.magic != 0x1903)
			nvmValid = false;

		if (configurationData.checksum != crc32)
			nvmValid = false;

		if (configurationData.size != sizeof(configurationData))
			nvmValid = false;

		if (!nvmValid)
			{
			configurationData.magic = 0x1903;
			configurationData.size = sizeof(configurationData);
			configurationData.checksum = crc32;
			
			configurationData.block00.partNumber = PARTNUMBER;
			/*
				NSD350-V25-36-261
				NSD350-V25-36-156
			*/
			configurationData.block02.GAINImplementedFunctionalities =
					(kGAIN_CentralizedPowerControl|kGAIN_Diagnostics|kGAIN_FallbackPowerMode|kGAIN_DataTranfer);
			
			configurationData.block03 = defaultDataBlock03;
			configurationData.block04 = defaultDataBlock04;
			configurationData.block05 = defaultDataBlock05;
			}

		if (!timerValid)
			{
			configurationData.block05.DeviceClass = 1;
			configurationData.block05.DeviceGroup = 1;
			
			configurationData.block09.GlobalInhibitTimer = coreNSD_convertToBigEndian16(200);
			configurationData.block09.InhibitTimerDT = coreNSD_convertToBigEndian16(50);
			configurationData.block09.SequenceTimeoutTimer = coreNSD_convertToBigEndian16(5000);
			configurationData.block09.TimerPHSM_GAIN = coreNSD_convertToBigEndian16(10000);
			configurationData.block09.TimerPHSM_MGCU = coreNSD_convertToBigEndian16(10000);
			configurationData.block09.reserved1 = 0;
			configurationData.block09.reserved2 = 0;
			configurationData.block09.TimerSTA_GAIN = coreNSD_convertToBigEndian16(1000);
			configurationData.block09.TimeoutTimerSTA_GAIN = coreNSD_convertToBigEndian16(1200);
			configurationData.block09.TimerSTA_MGCU = coreNSD_convertToBigEndian16(1000);		
			configurationData.block09.TimeoutTimerSTA_MGCU = coreNSD_convertToBigEndian16(1200);		
			configurationData.block09.TimerSES_MGCU = coreNSD_convertToBigEndian16(1000);
			configurationData.block09.TimerSES_GAIN = coreNSD_convertToBigEndian16(1000);		
			configurationData.block09.TimeoutTimerSES = coreNSD_convertToBigEndian16(1200);

			configurationData.block12.MGCUCounterDeltaInc = 1;
			configurationData.block12.MGCUCounterDeltaDec = 1;
			configurationData.block12.MGCUCounterThreshold = 3;
			configurationData.block12.GAINCounterDeltaInc = 1;
			configurationData.block12.GAINCounterDeltaDec = 1;
			configurationData.block12.GAINCounterThreshold = 3;
	
			for (i=0; i<kNumberOfGAINMessages; i++)
				GAINMessageInhibitTimes[i] = coreNSD_convertToSystemEndian16(configurationData.block09.GlobalInhibitTimer);

			GAINMessageInhibitTimes[kDT_RES] = coreNSD_convertToSystemEndian16(configurationData.block09.InhibitTimerDT);	
			GAINMessageInhibitTimes[kDT_DDL] = coreNSD_convertToSystemEndian16(configurationData.block09.InhibitTimerDT);	
			}

		if (!nvmValid || !timerValid)
			coreNSD_writeGAINConfigurationData(0, sizeof(configurationData), &configurationData);
		};

	return result;
	}

void coreNSD_handlePeriodicTask(MGCUStateBits* mgcu)
	{
/*
	uint8	i;
	tick_t	now;
	tick_t	deltaT;

	if (coreNSDConfiguration.getControllerTicksCallback == NULL)
		return;

	if (coreNSDConfiguration.networkStateChangedCallback == NULL)
		return;

	for (i=kSTA_GAIN; i<=kPHSM; i++)
		{
		if (coreNSD.messages[i].isPending)
			{
			now = coreNSDConfiguration.getControllerTicksCallback();
			deltaT = now - coreNSD.messages[i].timestamp;

			if (deltaT >= configurationData.block09.GlobalInhibitTimer)
				coreNSD_sendNSDMessage(i);
			}
		}

	now = coreNSDConfiguration.getControllerTicksCallback();
	deltaT = now - coreNSD.mgcuState.lastLifepulse;

	if (deltaT > configurationData.block09.TimeoutTimerSTA_MGCU)
		{
		if (coreNSD.mgcuState.timeoutCounter < configurationData.block12.MGCUCounterThreshold)
			coreNSD.mgcuState.timeoutCounter += configurationData.block12.MGCUCounterDeltaInc;

		coreNSD.mgcuState.lastLifepulse = now;
		}

	if (coreNSD.mgcuState.timeoutCounter >= configurationData.block12.MGCUCounterThreshold)
		{
		//	NSD_ATA2536_NM004-1
		if (coreNSD.mgcuState.bits.isPresent)
			{
			coreNSD_setGAINMGCU_Is_Present(coreNSD.messages[kSTA_GAIN].data, false);

			coreNSD.mgcuState.bits.isPresent = false;
            coreNSD.mgcuState.bits.centralizedPowerAvailable = false;
			coreNSD.mgcuState.timeoutCounter = configurationData.block12.MGCUCounterThreshold;

			//	Callback to Application Code
			coreNSDConfiguration.networkStateChangedCallback(kCentralizedPowerAvailableChanged, false);
			coreNSDConfiguration.networkStateChangedCallback(kMGCUPresentChanged, false);
			}	
		}
	else
		{
		//	NSD_ATA2536_NM004-2
		if (!coreNSD.mgcuState.bits.isPresent && (coreNSD.mgcuState.timeoutCounter <= 0))
			{
			coreNSD_setGAINMGCU_Is_Present(coreNSD.messages[kSTA_GAIN].data, true);

			coreNSD.mgcuState.bits.isPresent = true;
			coreNSD.mgcuState.timeoutCounter = 0;

			//	Callback to Application Code
			coreNSDConfiguration.networkStateChangedCallback(kMGCUPresentChanged, true);	

			//	Centralized Power available!
			if (coreNSD.mgcuState.bits.centralizedPowerAvailable)
				{
				//	Callback to Application Code
				coreNSDConfiguration.networkStateChangedCallback(kCentralizedPowerAvailableChanged, true);	
				}
			}	
		}

	now = coreNSDConfiguration.getControllerTicksCallback();
	deltaT = now - coreNSD.messages[kSTA_GAIN].timestamp;
	//	NSD_ATA2536_NM006-1
	if (deltaT >= configurationData.block09.TimerSTA_GAIN)
		{
		coreNSD_sendNSDMessage(kSTA_GAIN);
		}
	
	*mgcu = coreNSD.mgcuState.bits;
*/
	}

void coreNSD_sendNSDMessage(uint8 idx)
	{
/*
	if (coreNSDConfiguration.getControllerTicksCallback == NULL)
		return;

	if (coreNSDConfiguration.CANWriteMessageCallback == NULL)
		return;

	if (coreNSD.currentGAINState >= kState_Initialize)
		{
		tick_t	now = coreNSDConfiguration.getControllerTicksCallback();
		tick_t	deltaT = (now - coreNSD.messages[idx].timestamp);
	
		if (deltaT < configurationData.block09.GlobalInhibitTimer)
			{
			coreNSD.messages[idx].isPending = true;
			}
		else
			{
			coreNSDConfiguration.CANWriteMessageCallback(&coreNSD.messages[idx]);
			coreNSD.messages[idx].timestamp = now;
			coreNSD.messages[idx].isPending = false;
			}
		}
*/
	}

bool coreNSD_refreshGAINConfigurationDataBlock(int16 block)
	{
	return coreNSD_readGAINConfigurationDataBlock(block, coreNSD_getGAINConfigurationDataBlockAdress(block));
	}

bool coreNSD_readGAINConfigurationDataBlock(int16 block, void* dest)
	{
	bool	result = true;

	if (coreNSDConfiguration.readGAINConfigurationDataCallback)
		{
		coreNSDConfiguration.readGAINConfigurationDataCallback(GAINConfigurationDataBlockOffsetTable[block], GAINConfigurationDataBlockSizeTable[block], dest);
		if (block == 9)
			{
			uint8	i;
		
			for (i=0; i<kNumberOfGAINMessages; i++)
				GAINMessageInhibitTimes[i] = coreNSD_convertToSystemEndian16(configurationData.block09.GlobalInhibitTimer);

			GAINMessageInhibitTimes[kDT_RES] = coreNSD_convertToSystemEndian16(configurationData.block09.InhibitTimerDT);	
			GAINMessageInhibitTimes[kDT_DDL] = coreNSD_convertToSystemEndian16(configurationData.block09.InhibitTimerDT);
			}	
		}
	else
		result = false;

	return result;
	}

bool coreNSD_copyGAINConfigurationDataBlock(int16 block, void* dest)
	{
	bool	result = true;
 	uint8	i;
 	uint8*	srcPtr = (uint8*)&configurationData;
	uint8*	destPtr = (uint8*)dest;
	
	srcPtr += GAINConfigurationDataBlockOffsetTable[block];
	for (i=0; i<GAINConfigurationDataBlockSizeTable[block]; i++)
		*destPtr++ = *srcPtr++;

	return result;
	}

void* coreNSD_getGAINConfigurationDataBlockAdress(int16 block)
	{
	void*	result = &configurationData;
	
	result += GAINConfigurationDataBlockOffsetTable[block];

	return result;
	}

bool coreNSD_writeGAINConfigurationDataBlock(int16 block, const void* source)
	{
	bool			result = true;
	uint8*			destPtr = (uint8*)&configurationData;
	const uint8*	srcPtr = (const uint8*)source;
	uint16			i;
	
	destPtr += GAINConfigurationDataBlockOffsetTable[block];
	for (i=0; i<GAINConfigurationDataBlockSizeTable[block]; i++)
		*destPtr++ = *srcPtr++;

	if (coreNSDConfiguration.writeGAINConfigurationDataCallback)
		{
		coreNSDConfiguration.writeGAINConfigurationDataCallback(GAINConfigurationDataBlockOffsetTable[block], GAINConfigurationDataBlockSizeTable[block], source);
		}
	else
		result = false;

	return result;
	}

bool coreNSD_flushGAINConfigurationDataBlock(int16 block)
	{
	bool			result = true;

	if (coreNSDConfiguration.writeGAINConfigurationDataCallback)
		{
		uint8*			srcPtr = (uint8*)&configurationData;
	
		srcPtr += GAINConfigurationDataBlockOffsetTable[block];
		coreNSDConfiguration.writeGAINConfigurationDataCallback(GAINConfigurationDataBlockOffsetTable[block], GAINConfigurationDataBlockSizeTable[block], srcPtr);
		}
	else
		result = false;

	return result;
	}

bool coreNSD_readGAINConfigurationData(int16 offset, int16 size, void* dest)
	{
	bool	result = true;
 	uint8	i;

	if (coreNSDConfiguration.readGAINConfigurationDataCallback)
		{
		coreNSDConfiguration.readGAINConfigurationDataCallback(offset, size, dest);

		for (i=0; i<kNumberOfGAINMessages; i++)
			GAINMessageInhibitTimes[i] = coreNSD_convertToSystemEndian16(configurationData.block09.GlobalInhibitTimer);

		GAINMessageInhibitTimes[kDT_RES] = coreNSD_convertToSystemEndian16(configurationData.block09.InhibitTimerDT);	
		GAINMessageInhibitTimes[kDT_DDL] = coreNSD_convertToSystemEndian16(configurationData.block09.InhibitTimerDT);	
		}
	else
		result = false;

	return result;
	}

bool coreNSD_writeGAINConfigurationData(int16 offset, int16 size, const void* source)
	{
	bool			result = true;
	uint8*			destPtr = (uint8*)&configurationData;
	const uint8*	srcPtr = (const uint8*)source;
	uint16			i;
	
	destPtr += offset;
	for (i=0; i<size; i++)
		*destPtr++ = *srcPtr++;

	if (coreNSDConfiguration.writeGAINConfigurationDataCallback)
		{
		coreNSDConfiguration.writeGAINConfigurationDataCallback(offset, size, source);
		}
	else
		result = false;

	return result;
	}

uint16 coreNSD_convertToBigEndian16(uint16 x)
	{
	return ((x >> 8)|(x << 8));
	}

uint16 coreNSD_convertToSystemEndian16(uint16 x)
	{
	return ((x >> 8)|(x << 8));
	}
	
uint32 coreNSD_calculateCRC32(void *buffer, uint16 size)
	{
	uint16	idx, bit;
	uint32	data, crc32;
	uint8*	pData = (uint8*)buffer;
		
	crc32 = 0xFFFFFFFF;
	for (idx = 0; idx < size; idx++)
		{
		data = (uint32)*pData++;
		for (bit = 0; bit<8; bit++)
			{
			if ((crc32 ^ data) & 1)
				crc32 = (crc32 >> 1) ^ 0xEDB88320L;
			else
				crc32 = (crc32 >> 1) ^ 0;
			data >>= 1;
			}
		}
	crc32 ^= 0xFFFFFFFF;

	return crc32;
	}

void coreNSD_setGAINMessageIdentifier(CANMessage* message, GAINMessage msg)
	{
	(*message).identifier = GAINMessageLookup[msg];
	}

void coreNSD_setMGCUMessageIdentifier(CANMessage* message, MGCUMessage msg)
	{
	(*message).identifier = MGCUMessageLookup[msg];
	}

bool coreNSD_checkAndSetCANMessage(uint8 deviceID, CANMessage* message, tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data)
	{
	bool	result;
	
	if (message && coreNSD_isValidIdentifier(identifier, deviceID))
		{
		coreNSD_setCANMessage(message, timestamp, identifier, dataLengthCode, data);
		result = true;
		}
	else
		result = false;
		
	return result;
	}

void coreNSD_setCANMessage(CANMessage* message, tick_t timestamp, uint32 identifier, uint8 dataLengthCode, const byte* data)
	{
	uint8	i;

	if (message)
		{
		(*message).timestamp = timestamp;
		(*message).identifier = identifier;
		(*message).isPending = false;
		
		if (data)
			{
			(*message).dataLengthCode = dataLengthCode;
			}
		else
			{
			(*message).dataLengthCode = 0;
			}
		
		for (i = 0; i<(*message).dataLengthCode; i++)
			{
			(*message).data[i] = data[i];
			}
		
		for (i = (*message).dataLengthCode; i< sizeof((*message).data); i++)
			{
			(*message).data[i] = 0;
			}
		}
	}

/**	Valid source FID?
*/
bool coreNSD_isValidIdentifier(uint32 identifier, uint8 deviceID)
	{
	bool	result;
	
	if (
		(coreNSD_getSourceFID(identifier) == kFIDGalleyInserts) &&
		((coreNSD_getDeviceID(identifier) == deviceID) || (coreNSD_getDeviceID(identifier) == kMulticastAddress)) &&
		((coreNSD_getLCC(identifier) == kLCCNormalOperationChannel) || (coreNSD_getLCC(identifier) == kLCCNodeServiceChannel))
		)
		{		
		result = true;
		}
	else
		{
		result = false;
		}

	return result;
	}

void coreNSD_initializeGAINStatusMessage(CANMessage* message)
	{
	coreNSD_setGAINMode((*message).data, kMode_Fallback);
	coreNSD_setGAINState((*message).data, kState_Initialize);
	coreNSD_setGAINImpl_Functionalities((*message).data, kGAIN_FallbackPowerMode);
	}

void coreNSD_setGAINMode(byte* message, GAINMode mode)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINModeMask, kGAINModeByteOffset, kGAINModeBitOffset, mode);
	}

void coreNSD_setGAINState(byte* message, GAINState state)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINStateMask, kGAINStateByteOffset, kGAINStateBitOffset, state);
	}

void coreNSD_setGAINUsed_Power_Budget(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINUsedPowerBudgetMask, kGAINUsedPowerBudgetByteOffset, kGAINUsedPowerBudgetBitOffset, (v+49)/50);
	}

void coreNSD_setGAINFallback_Power_Budget(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINFallbackPowerBudgetMask, kGAINFallbackPowerBudgetByteOffset, kGAINFallbackPowerBudgetBitOffset, (v+49)/50);
	}

void coreNSD_setGAINPB_Remaining_Time(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINPBRemainingTimeMask, kGAINPBRemainingTimeMSBByteOffset, kGAINPBRemainingTimeBitOffset, (v >> 8) & 0xFF);
	coreNSD_setCANMessagePayloadData(message, kGAINPBRemainingTimeMask, kGAINPBRemainingTimeLSBByteOffset, kGAINPBRemainingTimeBitOffset, (v & 0xFF));
	}

void coreNSD_setGAINImpl_Functionalities(byte* message, uint8 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINImplementedFunctionalitiesMask, kGAINImplementedFunctionalitiesByteOffset, kGAINImplementedFunctionalitiesBitOffset, v);
	}

void coreNSD_setGAINGAIN_Is_Primary(byte* message, bool v)
	{
	coreNSD_setCANMessagePayloadDataBit(message, kGAINGAINIsPrimaryByteOffset, kGAINGAINIsPrimaryBitOffset, v);
	}

void coreNSD_setGAINFeeder_ID(byte* message, uint8 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINFeederIDMask, kGAINFeederIDByteOffset, kGAINFeederIDBitOffset, v);
	}

void coreNSD_setGAINMGCU_Is_Present(byte* message, bool v)
	{
	coreNSD_setCANMessagePayloadDataBit(message, kGAINMGCUIsPresentByteOffset, kGAINMGCUIsPresentBitOffset, v);
	}

/**	Read accessors to STA_GAIN payload data
*/
GAINMode coreNSD_getGAINMode(const byte* message)
	{
	return coreNSD_getCANMessagePayloadData(message, kGAINModeMask, kGAINModeByteOffset, kGAINModeBitOffset);
	}

GAINState coreNSD_getGAINState(const byte* message)
	{
	return coreNSD_getCANMessagePayloadData(message, kGAINStateMask, kGAINStateByteOffset, kGAINStateBitOffset);
	}

uint16 coreNSD_getGAINPB_Remaining_Time(const byte* message)
	{
	uint16	result;
	
	result = coreNSD_getCANMessagePayloadData(message, kGAINPBRemainingTimeMask, kGAINPBRemainingTimeMSBByteOffset, kGAINPBRemainingTimeBitOffset);
	result <<= 8;
	result |= coreNSD_getCANMessagePayloadData(message, kGAINPBRemainingTimeMask, kGAINPBRemainingTimeLSBByteOffset, kGAINPBRemainingTimeBitOffset);
	return result;
	}

uint16 coreNSD_getGAINUsed_Power_Budget(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kGAINUsedPowerBudgetMask, kGAINUsedPowerBudgetByteOffset, kGAINUsedPowerBudgetBitOffset);
	}
	
uint16 coreNSD_getGAINFallback_Power_Budget(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kGAINFallbackPowerBudgetMask, kGAINFallbackPowerBudgetByteOffset, kGAINFallbackPowerBudgetBitOffset);
	}

uint8 coreNSD_getGAINImpl_Functionalities(const byte* message)
	{
	return coreNSD_getCANMessagePayloadData(message, kGAINImplementedFunctionalitiesMask, kGAINImplementedFunctionalitiesByteOffset, kGAINImplementedFunctionalitiesBitOffset);
	}

bool coreNSD_getGAINGAIN_Is_Primary(const byte* message)
	{
	return coreNSD_getCANMessagePayloadDataBit(message, kGAINGAINIsPrimaryByteOffset, kGAINGAINIsPrimaryBitOffset);
	}

uint8 coreNSD_getGAINFeeder_ID(const byte* message)
	{
	return coreNSD_getCANMessagePayloadData(message, kGAINFeederIDMask, kGAINFeederIDByteOffset, kGAINFeederIDBitOffset);
	}

bool coreNSD_getGAINMGCU_Is_Present(const byte* message)
	{
	return coreNSD_getCANMessagePayloadDataBit(message, kGAINMGCUIsPresentByteOffset, kGAINMGCUIsPresentBitOffset);
	}

/**	Read accessors for GAIN_PT
*/
uint16 coreNSD_getGAINPTStandbyPowerThreshold(const byte* message)
	{
	return 5 * coreNSD_getCANMessagePayloadData(message, kGAINPTStandbyPowerThresholdMask, kGAINPTStandbyPowerThresholdByteOffset, kGAINPTStandbyPowerThresholdBitOffset);
	}

uint16 coreNSD_getGAINPTMaximumPowerThreshold(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kGAINPTMaximumPowerThresholdMask, kGAINPTMaximumPowerThresholdByteOffset, kGAINPTMaximumPowerThresholdBitOffset);
	}

uint16 coreNSD_getGAINPTMinimumPowerThreshold(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kGAINPTMinimumPowerThresholdMask, kGAINPTMinimumPowerThresholdByteOffset, kGAINPTMinimumPowerThresholdBitOffset);
	}

uint16 coreNSD_getGAINPTFallbackPowerThreshold(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kGAINPTFallbackPowerThresholdMask, kGAINPTFallbackPowerThresholdByteOffset, kGAINPTFallbackPowerThresholdBitOffset);
	}


/**	Write accessors for GAIN_PT
*/
void coreNSD_setGAINPTStandbyPowerThreshold(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINPTStandbyPowerThresholdMask, kGAINPTStandbyPowerThresholdByteOffset, kGAINPTStandbyPowerThresholdBitOffset, (v+4)/5);
	}

void coreNSD_setGAINPTMaximumPowerThreshold(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINPTMaximumPowerThresholdMask, kGAINPTMaximumPowerThresholdByteOffset, kGAINPTMaximumPowerThresholdBitOffset, (v+49)/50);
	}

void coreNSD_setGAINPTMinimumPowerThreshold(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINPTMinimumPowerThresholdMask, kGAINPTMinimumPowerThresholdByteOffset, kGAINPTMinimumPowerThresholdBitOffset, (v+49)/50);
	}

void coreNSD_setGAINPTFallbackPowerThreshold(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kGAINPTFallbackPowerThresholdMask, kGAINPTFallbackPowerThresholdByteOffset, kGAINPTFallbackPowerThresholdBitOffset, (v+49)/50);
	}


/**	Read accessors for PBR
*/
bool coreNSD_getPBRPowerBudgetType(byte* message)
	{
	return coreNSD_getCANMessagePayloadDataBit(message, kPBRPowerBudgetTypeByteOffset, kPBRPowerBudgetTypeBitOffset);
	}

uint16 coreNSD_getPBRNominalPowerBudget(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kPBRNominalPowerBudgetMask, kPBRNominalPowerBudgetByteOffset, kPBRNominalPowerBudgetBitOffset);
	}

uint16 coreNSD_getPBRMinimalPowerBudget(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kPBRMinimalPowerBudgetMask, kPBRMinimalPowerBudgetByteOffset, kPBRMinimalPowerBudgetBitOffset);
	}

uint16 coreNSD_getPBRNeededTimeWithNominalPB(const byte* message)
	{
	uint16	result;
	
	result = coreNSD_getCANMessagePayloadData(message, kPBRNeededTimeWithNominalPBMask, kPBRNeededTimeWithNominalPBMSBByteOffset, kPBRNeededTimeWithNominalPBBitOffset);
	result <<= 8;
	result |= coreNSD_getCANMessagePayloadData(message, kPBRNeededTimeWithNominalPBMask, kPBRNeededTimeWithNominalPBLSBByteOffset, kPBRNeededTimeWithNominalPBBitOffset);
	return result;
	}

uint16 coreNSD_getPBRNeededTimeWithMinimalPB(const byte* message)
	{
	uint16	result;
	
	result = coreNSD_getCANMessagePayloadData(message, kPBRNeededTimeWithMinimalPBMask, kPBRNeededTimeWithMinimalPBMSBByteOffset, kPBRNeededTimeWithMinimalPBBitOffset);
	result <<= 8;
	result |= coreNSD_getCANMessagePayloadData(message, kPBRNeededTimeWithMinimalPBMask, kPBRNeededTimeWithMinimalPBLSBByteOffset, kPBRNeededTimeWithMinimalPBBitOffset);
	return result;
	}

/**	Write accessors for PBR
*/
void coreNSD_setPBRPowerBudgetType(byte* message, bool v)
	{
	coreNSD_setCANMessagePayloadDataBit(message, kPBRPowerBudgetTypeByteOffset, kPBRPowerBudgetTypeBitOffset, v);
	}

void coreNSD_setPBRNominalPowerBudget(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kPBRNominalPowerBudgetMask, kPBRNominalPowerBudgetByteOffset, kPBRNominalPowerBudgetBitOffset, (v+49)/50);
	}

void coreNSD_setPBRMinimalPowerBudget(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kPBRMinimalPowerBudgetMask, kPBRMinimalPowerBudgetByteOffset, kPBRMinimalPowerBudgetBitOffset, (v+49)/50);
	}

void coreNSD_setPBRNeededTimeWithNominalPB(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kPBRNeededTimeWithNominalPBMask, kPBRNeededTimeWithNominalPBMSBByteOffset, kPBRNeededTimeWithNominalPBBitOffset, (v >> 8) & 0xFF);
	coreNSD_setCANMessagePayloadData(message, kPBRNeededTimeWithNominalPBMask, kPBRNeededTimeWithNominalPBLSBByteOffset, kPBRNeededTimeWithNominalPBBitOffset, (v & 0xFF));
	}

void coreNSD_setPBRNeededTimeWithMinimalPB(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kPBRNeededTimeWithMinimalPBMask, kPBRNeededTimeWithMinimalPBMSBByteOffset, kPBRNeededTimeWithMinimalPBBitOffset, (v >> 8) & 0xFF);
	coreNSD_setCANMessagePayloadData(message, kPBRNeededTimeWithMinimalPBMask, kPBRNeededTimeWithMinimalPBLSBByteOffset, kPBRNeededTimeWithMinimalPBBitOffset, (v & 0xFF));
	}

	
/**	Read accessors to STA_MGCU payload data
*/
MGCUState coreNSD_getMGCUState(const byte* message)
	{
	return coreNSD_getCANMessagePayloadData(message, kMGCUStateMask, kMGCUStateByteOffset, kMGCUStateBitOffset);
	}

bool coreNSD_getMGCUDisinfection_Mode(const byte* message)
	{
	return coreNSD_getCANMessagePayloadDataBit(message, kMGCUDisinfectionModeByteOffset, kMGCUDisinfectionModeBitOffset);
	}

uint8 coreNSD_getMGCUImplemented_Functionalities(const byte* message)
	{
	return coreNSD_getCANMessagePayloadData(message, kMGCUImplementedFunctionalitiesMask, kMGCUImplementedFunctionalitiesByteOffset, kMGCUImplementedFunctionalitiesBitOffset);
	}

uint8 coreNSD_getMGCUPower_Info_Feeder(const byte* message)
	{
	return coreNSD_getCANMessagePayloadData(message, kMGCUPowerInfoFeederMask, kMGCUPowerInfoFeederByteOffset, kMGCUPowerInfoFeederBitOffset);
	}

bool coreNSD_getMGCUCentral_Power_Valid_On_Feeder(const byte* message)
	{
	return coreNSD_getCANMessagePayloadDataBit(message, kMGCUCentralPowerValidOnFeederByteOffset, kMGCUCentralPowerValidOnFeederBitOffset);
	}

/**	Write accessors for STA_MGCU
*/
void coreNSD_setMGCUState(byte* message, MGCUState v)
	{
	coreNSD_setCANMessagePayloadData(message, kMGCUStateMask, kMGCUStateByteOffset, kMGCUStateBitOffset, v);
	}

void coreNSD_setMGCUDisinfection_Mode(byte* message, bool v)
	{
	coreNSD_setCANMessagePayloadDataBit(message, kMGCUDisinfectionModeByteOffset, kMGCUDisinfectionModeBitOffset, v);
	}

void coreNSD_setMGCUImplemented_Functionalities(byte* message, uint8 v)
	{
	coreNSD_setCANMessagePayloadData(message, kMGCUImplementedFunctionalitiesMask, kMGCUImplementedFunctionalitiesByteOffset, kMGCUImplementedFunctionalitiesBitOffset, v);
	}

void coreNSD_setMGCUPower_Info_Feeder(byte* message, uint8 v)
	{
	coreNSD_setCANMessagePayloadData(message, kMGCUPowerInfoFeederMask, kMGCUPowerInfoFeederByteOffset, kMGCUPowerInfoFeederBitOffset, v);
	}

void coreNSD_setMGCUCentral_Power_Valid_On_Feeder(byte* message, bool v)
	{
	coreNSD_setCANMessagePayloadDataBit(message, kMGCUCentralPowerValidOnFeederByteOffset, kMGCUCentralPowerValidOnFeederBitOffset, v);
	}

/**	Read accessors for PBM
*/
uint16 coreNSD_getPBMPowerBudgetMax(const byte* message)
	{
	return 50 * coreNSD_getCANMessagePayloadData(message, kPBMPowerBudgetMaxMask, kPBMPowerBudgetMaxByteOffset, kPowerBudgetMaxBitOffset);
	}
	
uint16 coreNSD_getPBMTime(const byte* message)
	{
	uint16	result;
	
	result = coreNSD_getCANMessagePayloadData(message, kPBMTimeMask, kPBMTimeMSBByteOffset, kPBMTimeBitOffset);
	result <<= 8;
	result |= coreNSD_getCANMessagePayloadData(message, kPBMTimeMask, kPBMTimeLSBByteOffset, kPBMTimeBitOffset);
	return result;
	}


/**	Write accessors for PBM
*/
void coreNSD_setPBMPowerBudgetMax(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kPBMPowerBudgetMaxMask, kPBMPowerBudgetMaxByteOffset, kPowerBudgetMaxBitOffset, (v+49)/50);
	}
	
void coreNSD_setPBMTime(byte* message, uint16 v)
	{
	coreNSD_setCANMessagePayloadData(message, kPBMTimeMask, kPBMTimeMSBByteOffset, kPBMTimeBitOffset, (v >> 8) & 0xFF);
	coreNSD_setCANMessagePayloadData(message, kPBMTimeMask, kPBMTimeLSBByteOffset, kPBMTimeBitOffset, (v & 0xFF));
	}


/**	Write accessors for CAN message
*/
void coreNSD_setCANMessagePayloadData(byte* message, uint8 mask, uint8 byteOffset, uint8 bitOffset, uint8 value)
	{
	if (message && (byteOffset < 8))
		{
		message[byteOffset] &= ~(mask << bitOffset);
		if (value)
			{
			message[byteOffset] |= ((value << bitOffset) & (mask << bitOffset));
			}
		}
	}

void coreNSD_setCANMessagePayloadDataBit(byte* message, uint8 byteOffset, uint8 bitOffset, bool value)
	{
	if (message && (byteOffset < 8))
		{
		message[byteOffset] &= ~(0x01 << bitOffset);
		if (value)
			{
			message[byteOffset] |= ((value << bitOffset) & (0x01 << bitOffset));
			}
		}
	}

/**	Read accessors for CAN message
*/
uint8	coreNSD_getCANMessagePayloadData(const byte* message, uint8 mask, uint8 byteOffset, uint8 bitOffset)
	{
	uint8	result = 0;
	
	if (message && (byteOffset < 8))
		{
		result = (message[byteOffset] >> bitOffset) & mask;
		}
	
	return result;
	}

bool	coreNSD_getCANMessagePayloadDataBit(const byte* message, uint8 byteOffset, uint8 bitOffset)
	{
	uint8	value = 0;
	bool	result = 0;
	
	if (message && (byteOffset < 8))
		{
		value = (message[byteOffset] >> bitOffset) & 0x01;
		}
	
	if (value)
		{
		result = true;
		}
	else
		{
		result = false;
		}
	
	return result;
	}

uint8	coreNSD_getLCC(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftSourceLCC) & kNSDIdentifierMask3Bit);
	
	return result;
	}

uint8	coreNSD_getSourceFID(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftSourceFID) & kNSDIdentifierMask7Bit);
	
	return result;
	}

uint8	coreNSD_getRSD(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftRSD) & kNSDIdentifierMask1Bit);
	
	return result;
	}

uint8	coreNSD_getLCL(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftLCL) & kNSDIdentifierMask1Bit);
	
	return result;
	}

uint8	coreNSD_getPVT(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftPVT) & kNSDIdentifierMask1Bit);
	
	return result;
	}

uint8	coreNSD_getMessageCode(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftMessageCode) & kNSDIdentifierMask7Bit);
	
	return result;
	}

uint8	coreNSD_getDeviceID(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftDeviceID) & kNSDIdentifierMask7Bit);
	
	return result;
	}

uint8	coreNSD_getRCI(uint32 identifier)
	{
	uint8	result;
	
	result = (uint8)((identifier >> kNSDIdentifierBitShiftRCI) & kNSDIdentifierMask2Bit);
	
	return result;
	}


void coreNSD_setLCC(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask3Bit << kNSDIdentifierBitShiftSourceLCC);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask3Bit) << kNSDIdentifierBitShiftSourceLCC);
		}
	}

void coreNSD_setSourceFID(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask7Bit << kNSDIdentifierBitShiftSourceFID);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask7Bit) << kNSDIdentifierBitShiftSourceFID);
		}
	}

void coreNSD_setRSD(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask1Bit << kNSDIdentifierBitShiftRSD);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask1Bit) << kNSDIdentifierBitShiftRSD);
		}
	}

void coreNSD_setLCL(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask1Bit << kNSDIdentifierBitShiftLCL);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask1Bit) << kNSDIdentifierBitShiftLCL);
		}
	}

void coreNSD_setPVT(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask1Bit << kNSDIdentifierBitShiftPVT);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask1Bit) << kNSDIdentifierBitShiftPVT);
		}
	}

void coreNSD_setMessageCode(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask7Bit << kNSDIdentifierBitShiftMessageCode);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask7Bit) << kNSDIdentifierBitShiftMessageCode);
		}
	}

void coreNSD_setDeviceID(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask7Bit << kNSDIdentifierBitShiftDeviceID);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask7Bit) << kNSDIdentifierBitShiftDeviceID);
		}
	}

void coreNSD_setRCI(uint32* identifier, uint8 value)
	{
	if (identifier)
		{
		//	Clear bits in identifier
		*identifier &= ~(kNSDIdentifierMask2Bit << kNSDIdentifierBitShiftRCI);
		//	Set new bits value in identifier
		*identifier |= ((((uint32)value) & kNSDIdentifierMask2Bit) << kNSDIdentifierBitShiftRCI);
		}
	}
#endif	//	NSD_SUPPORT
