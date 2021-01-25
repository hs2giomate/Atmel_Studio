/*
 * CO_Definitions.h
 *
 * Created: 8/7/2020 4:17:03 PM
 *  Author: GMateusDP
 */ 


#ifndef CO_DEFINITIONS_H_
#define CO_DEFINITIONS_H_
#include "stdint.h"
#include "compiler.h"
#include "driver_init.h"
#ifdef __cplusplus
extern "C" {
	#endif
	
	
	#define CCU_CANOPEN_NODE 2

/*******************************************************************************
   FEATURES
*******************************************************************************/
   #define CO_NO_SYNC                     1   //Associated objects: 1005, 1006, 1007, 2103, 2104
   #define CO_NO_EMERGENCY                1   //Associated objects: 1014, 1015
   #define CO_NO_SDO_SERVER               1   //Associated objects: 1200
   #define CO_NO_SDO_CLIENT               0   
   #define CO_NO_RPDO                     4   //Associated objects: 1400, 1401, 1402, 1403, 1600, 1601, 1602, 1603
   #define CO_NO_TPDO                     4   //Associated objects: 1800, 1801, 1802, 1803, 1A00, 1A01, 1A02, 1A03
   #if NMT_MASTER_NODE==1
		#define CO_NO_NMT_MASTER               1
		#define CCS_DOWNLOAD_INITIATE           1
		#define CCS_DOWNLOAD_SEGMENT            0
		#define CCS_UPLOAD_INITIATE             2
		#define CCS_UPLOAD_SEGMENT              3
		#define CCS_DOWNLOAD_BLOCK              6
		#define CCS_UPLOAD_BLOCK                5
		#define CCS_ABORT                       4
	#else
	   #define CO_NO_NMT_MASTER               0
	   #define CCS_DOWNLOAD_INITIATE          1U
	   #define CCS_DOWNLOAD_SEGMENT           0U
	   #define CCS_UPLOAD_INITIATE            2U
	   #define CCS_UPLOAD_SEGMENT             3U
	   #define CCS_DOWNLOAD_BLOCK             6U
	   #define CCS_UPLOAD_BLOCK               5U
	   #define CCS_ABORT                      0x80U
	#endif
 
   #define CO_NO_TRACE                    0   


/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
   #define CO_OD_NoOfElements             55

typedef enum{
	CO_ERROR_NO                 =  0,   /**< Operation completed successfully */
	CO_ERROR_ILLEGAL_ARGUMENT   = -1,   /**< Error in function arguments */
	CO_ERROR_OUT_OF_MEMORY      = -2,   /**< Memory allocation failed */
	CO_ERROR_TIMEOUT            = -3,   /**< Function timeout */
	CO_ERROR_ILLEGAL_BAUDRATE   = -4,   /**< Illegal baudrate passed to function CO_CANmodule_init() */
	CO_ERROR_RX_OVERFLOW        = -5,   /**< Previous message was not processed yet */
	CO_ERROR_RX_PDO_OVERFLOW    = -6,   /**< previous PDO was not processed yet */
	CO_ERROR_RX_MSG_LENGTH      = -7,   /**< Wrong receive message length */
	CO_ERROR_RX_PDO_LENGTH      = -8,   /**< Wrong receive PDO length */
	CO_ERROR_TX_OVERFLOW        = -9,   /**< Previous message is still waiting, buffer full */
	CO_ERROR_TX_PDO_WINDOW      = -10,  /**< Synchronous TPDO is outside window */
	CO_ERROR_TX_UNCONFIGURED    = -11,  /**< Transmit buffer was not configured properly */
	CO_ERROR_PARAMETERS         = -12,  /**< Error in function parameters */
	CO_ERROR_DATA_CORRUPT       = -13,  /**< Stored data are corrupt */
	CO_ERROR_CRC                = -14,   /**< CRC does not match */
	CO_ERROR_HAL		     	= -15	/**< HAL error */
}CO_ReturnError_t;


/**
 * Default CANopen identifiers.
 *
 * Default CANopen identifiers for CANopen communication objects. Same as
 * 11-bit addresses of CAN messages. These are default identifiers and
 * can be changed in CANopen. Especially PDO identifiers are confgured
 * in PDO linking phase of the CANopen network configuration.
 */
typedef enum{
     CO_CAN_ID_NMT_SERVICE       = 0x000,   /**< 0x000, Network management */
     CO_CAN_ID_SYNC              = 0x080,   /**< 0x080, Synchronous message */
     CO_CAN_ID_EMERGENCY         = 0x080,   /**< 0x080, Emergency messages (+nodeID) */
     CO_CAN_ID_TIME_STAMP        = 0x100,   /**< 0x100, Time stamp message */
     CO_CAN_ID_TPDO_1            = 0x180,   /**< 0x180, Default TPDO1 (+nodeID) */
     CO_CAN_ID_RPDO_1            = 0x200,   /**< 0x200, Default RPDO1 (+nodeID) */
     CO_CAN_ID_TPDO_2            = 0x280,   /**< 0x280, Default TPDO2 (+nodeID) */
     CO_CAN_ID_RPDO_2            = 0x300,   /**< 0x300, Default RPDO2 (+nodeID) */
     CO_CAN_ID_TPDO_3            = 0x380,   /**< 0x380, Default TPDO3 (+nodeID) */
     CO_CAN_ID_RPDO_3            = 0x400,   /**< 0x400, Default RPDO3 (+nodeID) */
     CO_CAN_ID_TPDO_4            = 0x480,   /**< 0x480, Default TPDO4 (+nodeID) */
     CO_CAN_ID_RPDO_4            = 0x500,   /**< 0x500, Default RPDO5 (+nodeID) */
     CO_CAN_ID_TSDO              = 0x580,   /**< 0x580, SDO response from server (+nodeID) */
     CO_CAN_ID_RSDO              = 0x600,   /**< 0x600, SDO request from client (+nodeID) */
     CO_CAN_ID_HEARTBEAT         = 0x700    /**< 0x700, Heartbeat message */
}CO_Default_CAN_ID_t;


#define CO_LOCK_CAN_SEND()   { 							                  \
		uint32_t PrevPrimask= __get_PRIMASK();   \
		__disable_irq();

#define CO_UNLOCK_CAN_SEND()     __set_PRIMASK(PrevPrimask);              \
		}

#define CO_LOCK_EMCY()          CO_LOCK_CAN_SEND()   /**< Lock critical section in CO_errorReport() or CO_errorReset() */
#define CO_UNLOCK_EMCY()        CO_UNLOCK_CAN_SEND() /**< Unlock critical section in CO_errorReport() or CO_errorReset() */

#define CO_LOCK_OD()            CO_LOCK_CAN_SEND()   /**< Lock critical section when accessing Object Dictionary */
#define CO_UNLOCK_OD()          CO_UNLOCK_CAN_SEND() /**< Unlock critical section when accessing Object Dictionary */
/** @} */


/**
 * @defgroup CO_dataTypes Data types
 * @{
 *
 * According to Misra C
 */
/* int8_t to uint64_t are defined in stdint.h */
typedef unsigned char           bool_t;     /**< bool_t */
typedef float                   float32_t;  /**< float32_t */
typedef long double             float64_t;  /**< float64_t */
typedef char                    char_t;     /**< char_t */
typedef unsigned char           oChar_t;    /**< oChar_t */
//typedef unsigned char           domain_t;   /**< domain_t */
typedef	struct can_async_descriptor CAN_Handler;

   typedef uint8_t      UNSIGNED8;
   typedef uint16_t     UNSIGNED16;
   typedef uint32_t     UNSIGNED32;
   typedef uint64_t     UNSIGNED64;
   typedef int8_t       INTEGER8;
   typedef int16_t      INTEGER16;
   typedef int32_t      INTEGER32;
   typedef int64_t      INTEGER64;
   typedef float32_t    REAL32;
   typedef float64_t    REAL64;
   typedef char_t       VISIBLE_STRING;
   typedef oChar_t      OCTET_STRING;
 //  typedef domain_t     DOMAIN;
   typedef unsigned char     DOMAINTT;
/** @} */

/**
 * Return values of some CANopen functions. If function was executed
 * successfully it returns 0 otherwise it returns <0.
 */



/**
 * CAN receive message structure as aligned in CAN module. It is different in
 * different microcontrollers. It usually contains other variables.
 */
typedef struct{
	/** CAN identifier. It must be read through CO_CANrxMsg_readIdent() function. */
	can_message		RxHeader;
	uint32_t            ident;          /* Standard Identifier */
	uint8_t             DLC;            /* Data length code (bits 0...3) */
	uint8_t             data[8];        /**< 8 data bytes */
}CO_CANrxMsg_t;


/**
 * Received message object
 */
typedef struct{
	uint32_t            ident;          /**< Standard CAN Identifier (bits 0..10) + RTR (bit 11) */
	uint32_t            mask;           /**< Standard Identifier mask with same alignment as ident */
	void               *object;         /**< From CO_CANrxBufferInit() */
	void              (*pFunct)(void *object, const CO_CANrxMsg_t *message);  /**< From CO_CANrxBufferInit() */
}CO_CANrx_t;


/**
 * Transmit message object.
 */
typedef struct{
	uint32_t            ident;          /**< CAN identifier as aligned in CAN module */
	uint8_t             DLC ;           /**< Length of CAN message. (DLC may also be part of ident) */
	uint8_t             data[8];        /**< 8 data bytes */
	volatile bool_t     bufferFull;     /**< True if previous message is still in buffer */
	/** Synchronous PDO messages has this flag set. It prevents them to be sent outside the synchronous window */
	volatile bool_t     syncFlag;
}CO_CANtx_t;


/**
 * CAN module object. It may be different in different microcontrollers.
 */
typedef struct{
	can_async_descriptor		*CANBaseDescriptor; /**< From CO_CANmodule_init() */
	CO_CANrx_t          *rxArray;        /**< From CO_CANmodule_init() */
	uint32_t             rxSize;         /**< From CO_CANmodule_init() */
	CO_CANtx_t          *txArray;        /**< From CO_CANmodule_init() */
	uint32_t             txSize;         /**< From CO_CANmodule_init() */
	volatile bool_t      CANnormal;      /**< CAN module is in normal mode */
	/** Value different than zero indicates, that CAN module hardware filters
	 * are used for CAN reception. If there is not enough hardware filters,
	 * they won't be used. In this case will be *all* received CAN messages
	 * processed by software. */
	volatile bool_t      useCANrxFilters;
	/** If flag is true, then message in transmitt buffer is synchronous PDO
	 * message, which will be aborted, if CO_clearPendingSyncPDOs() function
	 * will be called by application. This may be necessary if Synchronous
	 * window time was expired. */
	volatile bool_t      bufferInhibitFlag;
	/** Equal to 1, when the first transmitted message (bootup message) is in CAN TX buffers */
	volatile bool_t      firstCANtxMessage;
	/** Number of messages in transmit buffer, which are waiting to be copied to the CAN module */
	volatile uint32_t    CANtxCount;
	uint32_t             errOld;         /**< Previous state of CAN errors */
	void                *em;             /**< Emergency object */
}CO_CANmodule_t;

#define CO_EM_INTERNAL_BUFFER_SIZE      10
typedef	void (*ptrFunctSignal)(void);
typedef struct{
	uint8_t            *errorStatusBits;/**< From CO_EM_init() */
	uint8_t             errorStatusBitsSize;/**< From CO_EM_init() */
	/** Internal buffer for storing unsent emergency messages.*/
	uint8_t             buf[CO_EM_INTERNAL_BUFFER_SIZE * 8];
	uint8_t            *bufEnd;         /**< End+1 address of the above buffer */
	uint8_t            *bufWritePtr;    /**< Write pointer in the above buffer */
	uint8_t            *bufReadPtr;     /**< Read pointer in the above buffer */
	uint8_t             bufFull;        /**< True if above buffer is full */
	uint8_t             wrongErrorReport;/**< Error in arguments to CO_errorReport() */
	ptrFunctSignal        handler;/**< From CO_EM_initCallback() or NULL */
}CO_EM_t;


typedef struct{
	uint8_t            *errorRegister;  /**< From CO_EM_init() */
	uint32_t           *preDefErr;      /**< From CO_EM_init() */
	uint8_t             preDefErrSize;  /**< From CO_EM_init() */
	uint8_t             preDefErrNoOfErrors;/**< Number of active errors in preDefErr */
	uint32_t            inhibitEmTimer; /**< Internal timer for emergency message */
	CO_EM_t            *em;             /**< CO_EM_t sub object is included here */
	CO_CANmodule_t     *CANdev;         /**< From CO_EM_init() */
	CO_CANtx_t         *CANtxBuff;      /**< CAN transmit buffer */
}CO_EMpr_t;

typedef enum{
	CO_SDO_AB_NONE                  = 0x00000000UL, /**< 0x00000000, No abort */
	CO_SDO_AB_TOGGLE_BIT            = 0x05030000UL, /**< 0x05030000, Toggle bit not altered */
	CO_SDO_AB_TIMEOUT               = 0x05040000UL, /**< 0x05040000, SDO protocol timed out */
	CO_SDO_AB_CMD                   = 0x05040001UL, /**< 0x05040001, Command specifier not valid or unknown */
	CO_SDO_AB_BLOCK_SIZE            = 0x05040002UL, /**< 0x05040002, Invalid block size in block mode */
	CO_SDO_AB_SEQ_NUM               = 0x05040003UL, /**< 0x05040003, Invalid sequence number in block mode */
	CO_SDO_AB_CRC                   = 0x05040004UL, /**< 0x05040004, CRC error (block mode only) */
	CO_SDO_AB_OUT_OF_MEM            = 0x05040005UL, /**< 0x05040005, Out of memory */
	CO_SDO_AB_UNSUPPORTED_ACCESS    = 0x06010000UL, /**< 0x06010000, Unsupported access to an object */
	CO_SDO_AB_WRITEONLY             = 0x06010001UL, /**< 0x06010001, Attempt to read a write only object */
	CO_SDO_AB_READONLY              = 0x06010002UL, /**< 0x06010002, Attempt to write a read only object */
	CO_SDO_AB_NOT_EXIST             = 0x06020000UL, /**< 0x06020000, Object does not exist */
	CO_SDO_AB_NO_MAP                = 0x06040041UL, /**< 0x06040041, Object cannot be mapped to the PDO */
	CO_SDO_AB_MAP_LEN               = 0x06040042UL, /**< 0x06040042, Number and length of object to be mapped exceeds PDO length */
	CO_SDO_AB_PRAM_INCOMPAT         = 0x06040043UL, /**< 0x06040043, General parameter incompatibility reasons */
	CO_SDO_AB_DEVICE_INCOMPAT       = 0x06040047UL, /**< 0x06040047, General internal incompatibility in device */
	CO_SDO_AB_HW                    = 0x06060000UL, /**< 0x06060000, Access failed due to hardware error */
	CO_SDO_AB_TYPE_MISMATCH         = 0x06070010UL, /**< 0x06070010, Data type does not match, length of service parameter does not match */
	CO_SDO_AB_DATA_LONG             = 0x06070012UL, /**< 0x06070012, Data type does not match, length of service parameter too high */
	CO_SDO_AB_DATA_SHORT            = 0x06070013UL, /**< 0x06070013, Data type does not match, length of service parameter too short */
	CO_SDO_AB_SUB_UNKNOWN           = 0x06090011UL, /**< 0x06090011, Sub index does not exist */
	CO_SDO_AB_INVALID_VALUE         = 0x06090030UL, /**< 0x06090030, Invalid value for parameter (download only). */
	CO_SDO_AB_VALUE_HIGH            = 0x06090031UL, /**< 0x06090031, Value range of parameter written too high */
	CO_SDO_AB_VALUE_LOW             = 0x06090032UL, /**< 0x06090032, Value range of parameter written too low */
	CO_SDO_AB_MAX_LESS_MIN          = 0x06090036UL, /**< 0x06090036, Maximum value is less than minimum value. */
	CO_SDO_AB_NO_RESOURCE           = 0x060A0023UL, /**< 0x060A0023, Resource not available: SDO connection */
	CO_SDO_AB_GENERAL               = 0x08000000UL, /**< 0x08000000, General error */
	CO_SDO_AB_DATA_TRANSF           = 0x08000020UL, /**< 0x08000020, Data cannot be transferred or stored to application */
	CO_SDO_AB_DATA_LOC_CTRL         = 0x08000021UL, /**< 0x08000021, Data cannot be transferred or stored to application because of local control */
	CO_SDO_AB_DATA_DEV_STATE        = 0x08000022UL, /**< 0x08000022, Data cannot be transferred or stored to application because of present device state */
	CO_SDO_AB_DATA_OD               = 0x08000023UL, /**< 0x08000023, Object dictionary not present or dynamic generation fails */
	CO_SDO_AB_NO_DATA               = 0x08000024UL  /**< 0x08000024, No data available */
}CO_SDO_abortCode_t;

    #ifndef CO_SDO_BUFFER_SIZE
        #define CO_SDO_BUFFER_SIZE    32
    #endif


/**
 * Object Dictionary attributes. Bit masks for attribute in CO_OD_entry_t.
 */
typedef enum{
    CO_ODA_MEM_ROM          = 0x0001U,  /**< Variable is located in ROM memory */
    CO_ODA_MEM_RAM          = 0x0002U,  /**< Variable is located in RAM memory */
    CO_ODA_MEM_EEPROM       = 0x0003U,  /**< Variable is located in EEPROM memory */
    CO_ODA_READABLE         = 0x0004U,  /**< SDO server may read from the variable */
    CO_ODA_WRITEABLE        = 0x0008U,  /**< SDO server may write to the variable */
    CO_ODA_RPDO_MAPABLE     = 0x0010U,  /**< Variable is mappable for RPDO */
    CO_ODA_TPDO_MAPABLE     = 0x0020U,  /**< Variable is mappable for TPDO */
    CO_ODA_TPDO_DETECT_COS  = 0x0040U,  /**< If variable is mapped to any PDO, then
                                             PDO is automatically send, if variable
                                             changes its value */
    CO_ODA_MB_VALUE         = 0x0080U   /**< True when variable is a multibyte value */
}CO_SDO_OD_attributes_t;


/**
 * Common DS301 object dictionary entries.
 */
typedef enum{
    OD_H1000_DEV_TYPE             = 0x1000U,/**< Device type */
    OD_H1001_ERR_REG              = 0x1001U,/**< Error register */
    OD_H1002_MANUF_STATUS_REG     = 0x1002U,/**< Manufacturer status register */
    OD_H1003_PREDEF_ERR_FIELD     = 0x1003U,/**< Predefined error field */
    OD_H1004_RSV                  = 0x1004U,/**< Reserved */
    OD_H1005_COBID_SYNC           = 0x1005U,/**< Sync message cob-id */
    OD_H1006_COMM_CYCL_PERIOD     = 0x1006U,/**< Communication cycle period */
    OD_H1007_SYNC_WINDOW_LEN      = 0x1007U,/**< Sync windows length */
    OD_H1008_MANUF_DEV_NAME       = 0x1008U,/**< Manufacturer device name */
    OD_H1009_MANUF_HW_VERSION     = 0x1009U,/**< Manufacturer hardware version */
    OD_H100A_MANUF_SW_VERSION     = 0x100AU,/**< Manufacturer software version */
    OD_H100B_RSV                  = 0x100BU,/**< Reserved */
    OD_H100C_GUARD_TIME           = 0x100CU,/**< Guard time */
    OD_H100D_LIFETIME_FACTOR      = 0x100DU,/**< Life time factor */
    OD_H100E_RSV                  = 0x100EU,/**< Reserved */
    OD_H100F_RSV                  = 0x100FU,/**< Reserved */
    OD_H1010_STORE_PARAM_FUNC     = 0x1010U,/**< Store parameter in persistent memory function */
    OD_H1011_REST_PARAM_FUNC      = 0x1011U,/**< Restore default parameter function */
    OD_H1012_COBID_TIME           = 0x1012U,/**< Timestamp message cob-id */
    OD_H1013_HIGH_RES_TIMESTAMP   = 0x1013U,/**< High resolution timestamp */
    OD_H1014_COBID_EMERGENCY      = 0x1014U,/**< Emergency message cob-id */
    OD_H1015_INHIBIT_TIME_MSG     = 0x1015U,/**< Inhibit time message */
    OD_H1016_CONSUMER_HB_TIME     = 0x1016U,/**< Consumer heartbeat time */
    OD_H1017_PRODUCER_HB_TIME     = 0x1017U,/**< Producer heartbeat time */
    OD_H1018_IDENTITY_OBJECT      = 0x1018U,/**< Identity object */
    OD_H1019_SYNC_CNT_OVERFLOW    = 0x1019U,/**< Sync counter overflow value */
    OD_H1020_VERIFY_CONFIG        = 0x1020U,/**< Verify configuration */
    OD_H1021_STORE_EDS            = 0x1021U,/**< Store EDS */
    OD_H1022_STORE_FORMAT         = 0x1022U,/**< Store format */
    OD_H1023_OS_CMD               = 0x1023U,/**< OS command */
    OD_H1024_OS_CMD_MODE          = 0x1024U,/**< OS command mode */
    OD_H1025_OS_DBG_INTERFACE     = 0x1025U,/**< OS debug interface */
    OD_H1026_OS_PROMPT            = 0x1026U,/**< OS prompt */
    OD_H1027_MODULE_LIST          = 0x1027U,/**< Module list */
    OD_H1028_EMCY_CONSUMER        = 0x1028U,/**< Emergency consumer object */
    OD_H1029_ERR_BEHAVIOR         = 0x1029U,/**< Error behaviour */
    OD_H1200_SDO_SERVER_PARAM     = 0x1200U,/**< SDO server parameters */
    OD_H1280_SDO_CLIENT_PARAM     = 0x1280U,/**< SDO client parameters */
    OD_H1400_RXPDO_1_PARAM        = 0x1400U,/**< RXPDO communication parameter */
    OD_H1401_RXPDO_2_PARAM        = 0x1401U,/**< RXPDO communication parameter */
    OD_H1402_RXPDO_3_PARAM        = 0x1402U,/**< RXPDO communication parameter */
    OD_H1403_RXPDO_4_PARAM        = 0x1403U,/**< RXPDO communication parameter */
    OD_H1600_RXPDO_1_MAPPING      = 0x1600U,/**< RXPDO mapping parameters */
    OD_H1601_RXPDO_2_MAPPING      = 0x1601U,/**< RXPDO mapping parameters */
    OD_H1602_RXPDO_3_MAPPING      = 0x1602U,/**< RXPDO mapping parameters */
    OD_H1603_RXPDO_4_MAPPING      = 0x1603U,/**< RXPDO mapping parameters */
    OD_H1800_TXPDO_1_PARAM        = 0x1800U,/**< TXPDO communication parameter */
    OD_H1801_TXPDO_2_PARAM        = 0x1801U,/**< TXPDO communication parameter */
    OD_H1802_TXPDO_3_PARAM        = 0x1802U,/**< TXPDO communication parameter */
    OD_H1803_TXPDO_4_PARAM        = 0x1803U,/**< TXPDO communication parameter */
    OD_H1A00_TXPDO_1_MAPPING      = 0x1A00U,/**< TXPDO mapping parameters */
    OD_H1A01_TXPDO_2_MAPPING      = 0x1A01U,/**< TXPDO mapping parameters */
    OD_H1A02_TXPDO_3_MAPPING      = 0x1A02U,/**< TXPDO mapping parameters */
    OD_H1A03_TXPDO_4_MAPPING      = 0x1A03U /**< TXPDO mapping parameters */
}CO_ObjDicId_t;


/**
 * Bit masks for flags associated with variable from @ref CO_SDO_objectDictionary.
 *
 * This additional functionality of any variable in @ref CO_SDO_objectDictionary can be
 * enabled by function CO_OD_configure(). Location of the flag byte can be
 * get from function CO_OD_getFlagsPointer().
 */
typedef enum{
    /** Variable was written by RPDO. Flag can be cleared by application */
    CO_ODFL_RPDO_WRITTEN        = 0x01U,
    /** Variable is mapped to TPDO */
    CO_ODFL_TPDO_MAPPED         = 0x02U,
    /** Change of state bit, initially copy of attribute from CO_OD_entry_t.
    If set and variable is mapped to TPDO, TPDO will be automatically send,
    if variable changed */
    CO_ODFL_TPDO_COS_ENABLE     = 0x04U,
    /** PDO send bit, can be set by application. If variable is mapped into
    TPDO, TPDO will be send and bit will be cleared. */
    CO_ODFL_TPDO_SEND           = 0x08U,
    /** Variable was accessed by SDO download */
    CO_ODFL_SDO_DOWNLOADED      = 0x10U,
    /** Variable was accessed by SDO upload */
    CO_ODFL_SDO_UPLOADED        = 0x20U,
    /** Reserved */
    CO_ODFL_BIT_6               = 0x40U,
    /** Reserved */
    CO_ODFL_BIT_7               = 0x80U
}CO_SDO_OD_flags_t;


/**
 * Internal states of the SDO server state machine
 */
typedef enum {
    CO_SDO_ST_IDLE                  = 0x00U,
    CO_SDO_ST_DOWNLOAD_INITIATE     = 0x11U,
    CO_SDO_ST_DOWNLOAD_SEGMENTED    = 0x12U,
    CO_SDO_ST_DOWNLOAD_BL_INITIATE  = 0x14U,
    CO_SDO_ST_DOWNLOAD_BL_SUBBLOCK  = 0x15U,
    CO_SDO_ST_DOWNLOAD_BL_SUB_RESP  = 0x16U,
    CO_SDO_ST_DOWNLOAD_BL_END       = 0x17U,
    CO_SDO_ST_UPLOAD_INITIATE       = 0x21U,
    CO_SDO_ST_UPLOAD_SEGMENTED      = 0x22U,
    CO_SDO_ST_UPLOAD_BL_INITIATE    = 0x24U,
    CO_SDO_ST_UPLOAD_BL_INITIATE_2  = 0x25U,
    CO_SDO_ST_UPLOAD_BL_SUBBLOCK    = 0x26U,
    CO_SDO_ST_UPLOAD_BL_END         = 0x27U
} CO_SDO_state_t;


/**
 * Object for one entry with specific index in @ref CO_SDO_objectDictionary.
 */
typedef struct {
    /** The index of Object from 0x1000 to 0xFFFF */
    uint32_t            index;
    /** Number of (sub-objects - 1). If Object Type is variable, then
    maxSubIndex is 0, otherwise maxSubIndex is equal or greater than 1. */
    uint8_t             maxSubIndex;
    /** If Object Type is record, attribute is set to zero. Attribute for
    each member is then set in special array with members of type
    CO_OD_entryRecord_t. If Object Type is Array, attribute is common for
    all array members. See #CO_SDO_OD_attributes_t. */
    uint32_t            attribute;
    /** If Object Type is Variable, length is the length of variable in bytes.
    If Object Type is Array, length is the length of one array member.
    If Object Type is Record, length is zero. Length for each member is
    set in special array with members of type CO_OD_entryRecord_t.
    If Object Type is Domain, length is zero. Length is specified
    by application in @ref CO_SDO_OD_function. */
    uint32_t            length;
    /** If Object Type is Variable, pData is pointer to data.
    If Object Type is Array, pData is pointer to data. Data doesn't
    include Sub-Object 0.
    If object type is Record, pData is pointer to special array
    with members of type CO_OD_entryRecord_t.
    If object type is Domain, pData is null. */
    void               *pData;
}CO_OD_entry_t;


/**
 * Object for record type entry in @ref CO_SDO_objectDictionary.
 *
 * See CO_OD_entry_t.
 */
typedef struct{
    /** See #CO_SDO_OD_attributes_t */
    void               *pData;
    /** Length of variable in bytes. If object type is Domain, length is zero */
    uint32_t            attribute;
    /** Pointer to data. If object type is Domain, pData is null */
    uint32_t            length;
}CO_OD_entryRecord_t;


/**
 * Object contains all information about the object being transferred by SDO server.
 *
 * Object is used as an argument to @ref CO_SDO_OD_function. It is also
 * part of the CO_SDO_t object.
 */
typedef struct{
    /** Informative parameter. It may point to object, which is connected
    with this OD entry. It can be used inside @ref CO_SDO_OD_function, ONLY
    if it was registered by CO_OD_configure() function before. */
    void               *object;
    /** SDO data buffer contains data, which are exchanged in SDO transfer.
    @ref CO_SDO_OD_function may verify or manipulate that data before (after)
    they are written to (read from) Object dictionary. Data have the same
    endianes as processor. Pointer must NOT be changed. (Data up to length
    can be changed.) */
    uint8_t            *data;
    /** Pointer to location in object dictionary, where data are stored.
    (informative reference to old data, read only). Data have the same
    endianes as processor. If data type is Domain, this variable is null. */
    const void         *ODdataStorage;
    /** Length of data in the above buffer. Read only, except for domain. If
    data type is domain see @ref CO_SDO_OD_function for special rules by upload. */
    uint32_t            dataLength;
    /** Attribute of object in Object dictionary (informative, must NOT be changed). */
    uint32_t            attribute;
    /** Pointer to the #CO_SDO_OD_flags_t byte. */
    uint8_t            *pFlags;
    /** Index of object in Object dictionary (informative, must NOT be changed). */
    uint32_t            index;
    /** Subindex of object in Object dictionary (informative, must NOT be changed). */
    uint8_t             subIndex;
    /** True, if SDO upload is in progress, false if SDO download is in progress. */
    bool_t              reading;
    /** Used by domain data type. Indicates the first segment. Variable is informative. */
    bool_t              firstSegment;
    /** Used by domain data type. If false by download, then application will
    receive more segments during SDO communication cycle. If uploading,
    application may set variable to false, so SDO server will call
    @ref CO_SDO_OD_function again for filling the next data. */
    bool_t              lastSegment;
    /** Used by domain data type. By upload @ref CO_SDO_OD_function may write total
    data length, so this information will be send in SDO upload initiate phase. It
    is not necessary to specify this variable. By download this variable contains
    total data size, if size is indicated in SDO download initiate phase */
    uint32_t            dataLengthTotal;
    /** Used by domain data type. In case of multiple segments, this indicates the offset
    into the buffer this segment starts at. */
    uint32_t            offset;
}CO_ODF_arg_t;


/**
 * Object is used as array inside CO_SDO_t, parallel to @ref CO_SDO_objectDictionary.
 *
 * Object is generated by function CO_OD_configure(). It is then used as
 * extension to Object dictionary entry at specific index.
 */
typedef struct{
    /** Pointer to @ref CO_SDO_OD_function */
    CO_SDO_abortCode_t (*pODFunc)(CO_ODF_arg_t *ODF_arg);
    /** Pointer to object, which will be passed to @ref CO_SDO_OD_function */
    void               *object;
    /** Pointer to #CO_SDO_OD_flags_t. If object type is array or record, this
    variable points to array with length equal to number of subindexes. */
    uint8_t            *flags;
}CO_OD_extension_t;


/**
 * SDO server object.
 */
typedef struct{
    /** 8 data bytes of the received message. */
    uint8_t             CANrxData[8];
    /** SDO data buffer of size #CO_SDO_BUFFER_SIZE. */
    uint8_t             databuffer[CO_SDO_BUFFER_SIZE];
    /** Internal flag indicates, that this object has own OD */
    bool_t              ownOD;
    /** Pointer to the @ref CO_SDO_objectDictionary (array) */
    const CO_OD_entry_t *OD;
    /** Size of the @ref CO_SDO_objectDictionary */
    uint32_t            ODSize;
    /** Pointer to array of CO_OD_extension_t objects. Size of the array is
    equal to ODSize. */
    CO_OD_extension_t  *ODExtensions;
    /** Offset in buffer of next data segment being read/written */
    uint32_t            bufferOffset;
    /** Sequence number of OD entry as returned from CO_OD_find() */
    uint32_t            entryNo;
    /** CO_ODF_arg_t object with additional variables. Reference to this object
    is passed to @ref CO_SDO_OD_function */
    CO_ODF_arg_t        ODF_arg;
    /** From CO_SDO_init() */
    uint8_t             nodeId;
    /** Current internal state of the SDO server state machine #CO_SDO_state_t */
    CO_SDO_state_t      state;
    /** Toggle bit in segmented transfer or block sequence in block transfer */
    uint8_t             sequence;
    /** Timeout timer for SDO communication */
    uint32_t            timeoutTimer;
    /** Number of segments per block with 1 <= blksize <= 127 */
    uint8_t             blksize;
    /** True, if CRC calculation by block transfer is enabled */
    bool_t              crcEnabled;
    /** Calculated CRC code */
    uint32_t            crc;
    /** Length of data in the last segment in block upload */
    uint8_t             lastLen;
    /** Indication end of block transfer */
    bool_t              endOfTransfer;
    /** Variable indicates, if new SDO message received from CAN bus */
    bool_t              CANrxNew;
    /** From CO_SDO_initCallback() or NULL */
    void              (*ptrFunctSignal)(void);
    /** From CO_SDO_init() */
    CO_CANmodule_t     *CANdevTx;
    /** CAN transmit buffer inside CANdev for CAN tx message */
    CO_CANtx_t         *CANtxBuff;
}CO_SDO_t;

typedef enum{
	CO_NMT_INITIALIZING             = 0,    /**< Device is initializing */
	CO_NMT_PRE_OPERATIONAL          = 127,  /**< Device is in pre-operational state */
	CO_NMT_OPERATIONAL              = 5,    /**< Device is in operational state */
	CO_NMT_STOPPED                  = 4     /**< Device is stopped */
}CO_NMT_internalState_t;

typedef struct{
    CO_EM_t            *em;             /**< From CO_SYNC_init() */
    uint8_t            *operatingState; /**< From CO_SYNC_init() */
    /** True, if device is SYNC producer. Calculated from _COB ID SYNC Message_
    variable from Object dictionary (index 0x1005). */
    bool_t              isProducer;
    /** COB_ID of SYNC message. Calculated from _COB ID SYNC Message_
    variable from Object dictionary (index 0x1005). */
    uint32_t            COB_ID;
    /** Sync period time in [microseconds]. Calculated from _Communication cycle period_
    variable from Object dictionary (index 0x1006). */
    uint32_t            periodTime;
    /** Sync period timeout time in [microseconds].
    (periodTimeoutTime = periodTime * 1,5) */
    uint32_t            periodTimeoutTime;
    /** Value from _Synchronous counter overflow value_ variable from Object
    dictionary (index 0x1019) */
    uint8_t             counterOverflowValue;
    /** True, if current time is inside synchronous window.
    In this case synchronous PDO may be sent. */
    bool_t              curentSyncTimeIsInsideWindow;
    /** Variable indicates, if new SYNC message received from CAN bus */
    bool_t              CANrxNew;
    /** Variable toggles, if new SYNC message received from CAN bus */
    bool_t              CANrxToggle;
    /** Counter of the SYNC message if counterOverflowValue is different than zero */
    uint8_t             counter;
    /** Timer for the SYNC message in [microseconds].
    Set to zero after received or transmitted SYNC message */
    uint32_t            timer;
    /** Set to nonzero value, if SYNC with wrong data length is received from CAN */
    uint32_t            receiveError;
    CO_CANmodule_t     *CANdevRx;       /**< From CO_SYNC_init() */
    uint32_t            CANdevRxIdx;    /**< From CO_SYNC_init() */
    CO_CANmodule_t     *CANdevTx;       /**< From CO_SYNC_init() */
    CO_CANtx_t         *CANtxBuff;      /**< CAN transmit buffer inside CANdevTx */
    uint32_t            CANdevTxIdx;    /**< From CO_SYNC_init() */
}CO_SYNC_t;

typedef		void (*ptrFuncNMT)(CO_NMT_internalState_t state);

/**
 * Commands from NMT master.
 */
typedef enum{
    CO_NMT_ENTER_OPERATIONAL        = 1,    /**< Start device */
    CO_NMT_ENTER_STOPPED            = 2,    /**< Stop device */
    CO_NMT_ENTER_PRE_OPERATIONAL    = 128,  /**< Put device into pre-operational */
    CO_NMT_RESET_NODE               = 129,  /**< Reset device */
    CO_NMT_RESET_COMMUNICATION      = 130   /**< Reset CANopen communication on device */
}CO_NMT_command_t;


/**
 * Return code for CO_NMT_process() that tells application code what to
 * reset.
 */
typedef enum{
    CO_RESET_NOT  = 0,/**< Normal return, no action */
    CO_RESET_COMM = 1,/**< Application must provide communication reset. */
    CO_RESET_APP  = 2,/**< Application must provide complete device reset */
    CO_RESET_QUIT = 3 /**< Application must quit, no reset of microcontroller (command is not requested by the stack.) */
}CO_NMT_reset_cmd_t;


/**
 * NMT consumer and Heartbeat producer object. It includes also variables for
 * @ref CO_NMT_statusLEDdiodes. Object is initialized by CO_NMT_init().
 */
typedef struct{
    uint8_t             operatingState; /**< See @ref CO_NMT_internalState_t */
    int8_t              LEDflickering;  /**< See @ref CO_NMT_statusLEDdiodes */
    int8_t              LEDblinking;    /**< See @ref CO_NMT_statusLEDdiodes */
    int8_t              LEDsingleFlash; /**< See @ref CO_NMT_statusLEDdiodes */
    int8_t              LEDdoubleFlash; /**< See @ref CO_NMT_statusLEDdiodes */
    int8_t              LEDtripleFlash; /**< See @ref CO_NMT_statusLEDdiodes */
    int8_t              LEDquadrupleFlash; /**< See @ref CO_NMT_statusLEDdiodes */
    int8_t              LEDgreenRun;    /**< See @ref CO_NMT_statusLEDdiodes */
    int8_t              LEDredError;    /**< See @ref CO_NMT_statusLEDdiodes */

    uint8_t             resetCommand;   /**< If different than zero, device will reset */
    uint8_t             nodeId;         /**< CANopen Node ID of this device */
    uint32_t            HBproducerTimer;/**< Internal timer for HB producer */
    uint32_t            firstHBTime;    /**< From CO_NMT_init() */
    CO_EMpr_t          *emPr;           /**< From CO_NMT_init() */
    CO_CANmodule_t     *HB_CANdev;      /**< From CO_NMT_init() */
    ptrFuncNMT         ptrState; /**< From CO_NMT_initCallback() or NULL */
    CO_CANtx_t         *HB_TXbuff;      /**< CAN transmit buffer */
}CO_NMT_t;
typedef struct{
    uint8_t             maxSubIndex;    /**< Equal to 2 */
    /** Communication object identifier for message received. Meaning of the specific bits:
        - Bit  0-10: COB-ID for PDO, to change it bit 31 must be set.
        - Bit 11-29: set to 0 for 11 bit COB-ID.
        - Bit 30:    If true, rtr are NOT allowed for PDO.
        - Bit 31:    If true, node does NOT use the PDO. */
    uint32_t            COB_IDUsedByRPDO;
    /** Transmission type. Values:
        - 0-240:   Reciving is synchronous, process after next reception of the SYNC object.
        - 241-253: Not used.
        - 254:     Manufacturer specific.
        - 255:     Asynchronous. */
    uint8_t             transmissionType;
}CO_RPDOCommPar_t;


/**
 * RPDO mapping parameter. The same as record from Object dictionary (index 0x1600+).
 */
typedef struct{
    /** Actual number of mapped objects from 0 to 8. To change mapped object,
    this value must be 0. */
    uint8_t             numberOfMappedObjects;
    /** Location and size of the mapped object. Bit meanings `0xIIIISSLL`:
        - Bit  0-7:  Data Length in bits.
        - Bit 8-15:  Subindex from object distionary.
        - Bit 16-31: Index from object distionary. */
    uint32_t            mappedObject1;
    uint32_t            mappedObject2;  /**< Same */
    uint32_t            mappedObject3;  /**< Same */
    uint32_t            mappedObject4;  /**< Same */
    uint32_t            mappedObject5;  /**< Same */
    uint32_t            mappedObject6;  /**< Same */
    uint32_t            mappedObject7;  /**< Same */
    uint32_t            mappedObject8;  /**< Same */
}CO_RPDOMapPar_t;


/**
 * TPDO communication parameter. The same as record from Object dictionary (index 0x1800+).
 */
typedef struct{
    uint8_t             maxSubIndex;    /**< Equal to 6 */
    /** Communication object identifier for transmitting message. Meaning of the specific bits:
        - Bit  0-10: COB-ID for PDO, to change it bit 31 must be set.
        - Bit 11-29: set to 0 for 11 bit COB-ID.
        - Bit 30:    If true, rtr are NOT allowed for PDO.
        - Bit 31:    If true, node does NOT use the PDO. */
    uint32_t            COB_IDUsedByTPDO;
    /** Transmission type. Values:
        - 0:       Transmiting is synchronous, specification in device profile.
        - 1-240:   Transmiting is synchronous after every N-th SYNC object.
        - 241-251: Not used.
        - 252-253: Transmited only on reception of Remote Transmission Request.
        - 254:     Manufacturer specific.
        - 255:     Asinchronous, specification in device profile. */
    uint8_t             transmissionType;
    /** Minimum time between transmissions of the PDO in 100micro seconds.
    Zero disables functionality. */
    uint32_t            inhibitTime;
    /** Not used */
    uint8_t             compatibilityEntry;
    /** Time between periodic transmissions of the PDO in milliseconds.
    Zero disables functionality. */
    uint32_t            eventTimer;
    /** Used with numbered SYNC messages. Values:
        - 0:       Counter of the SYNC message shall not be processed.
        - 1-240:   The SYNC message with the counter value equal to this value
                   shall be regarded as the first received SYNC message. */
    uint8_t             SYNCStartValue;
}CO_TPDOCommPar_t;


/**
 * TPDO mapping parameter. The same as record from Object dictionary (index 0x1A00+).
 */
typedef struct{
    /** Actual number of mapped objects from 0 to 8. To change mapped object,
    this value must be 0. */
    uint8_t             numberOfMappedObjects;
    /** Location and size of the mapped object. Bit meanings `0xIIIISSLL`:
        - Bit  0-7:  Data Length in bits.
        - Bit 8-15:  Subindex from object distionary.
        - Bit 16-31: Index from object distionary. */
    uint32_t            mappedObject1;
    uint32_t            mappedObject2;  /**< Same */
    uint32_t            mappedObject3;  /**< Same */
    uint32_t            mappedObject4;  /**< Same */
    uint32_t            mappedObject5;  /**< Same */
    uint32_t            mappedObject6;  /**< Same */
    uint32_t            mappedObject7;  /**< Same */
    uint32_t            mappedObject8;  /**< Same */
}CO_TPDOMapPar_t;


/**
 * RPDO object.
 */
typedef struct{
    CO_EM_t            *em;             /**< From CO_RPDO_init() */
    CO_SDO_t           *SDO;            /**< From CO_RPDO_init() */
    CO_SYNC_t          *SYNC;           /**< From CO_RPDO_init() */
    const CO_RPDOCommPar_t *RPDOCommPar;/**< From CO_RPDO_init() */
    const CO_RPDOMapPar_t  *RPDOMapPar; /**< From CO_RPDO_init() */
    uint8_t            *operatingState; /**< From CO_RPDO_init() */
    uint8_t             nodeId;         /**< From CO_RPDO_init() */
    uint32_t            defaultCOB_ID;  /**< From CO_RPDO_init() */
    uint8_t             restrictionFlags;/**< From CO_RPDO_init() */
    /** True, if PDO is enabled and valid */
    bool_t              valid;
    /** True, if PDO synchronous (transmissionType <= 240) */
    bool_t              synchronous;
    /** Data length of the received PDO message. Calculated from mapping */
    uint8_t             dataLength;
    /** Pointers to 8 data objects, where PDO will be copied */
    uint8_t            *mapPointer[8];
    /** Variable indicates, if new PDO message received from CAN bus. */
    volatile bool_t     CANrxNew[2];
    /** 8 data bytes of the received message. */
    uint8_t             CANrxData[2][8];
    CO_CANmodule_t     *CANdevRx;       /**< From CO_RPDO_init() */
    uint32_t            CANdevRxIdx;    /**< From CO_RPDO_init() */
}CO_RPDO_t;


/**
 * TPDO object.
 */
typedef struct{
    CO_EM_t            *em;             /**< From CO_TPDO_init() */
    CO_SDO_t           *SDO;            /**< From CO_TPDO_init() */
    const CO_TPDOCommPar_t *TPDOCommPar;/**< From CO_TPDO_init() */
    const CO_TPDOMapPar_t  *TPDOMapPar; /**< From CO_TPDO_init() */
    uint8_t            *operatingState; /**< From CO_TPDO_init() */
    uint8_t             nodeId;         /**< From CO_TPDO_init() */
    uint32_t            defaultCOB_ID;  /**< From CO_TPDO_init() */
    uint8_t             restrictionFlags;/**< From CO_TPDO_init() */
    bool_t              valid;          /**< True, if PDO is enabled and valid */
    /** Data length of the transmitting PDO message. Calculated from mapping */
    uint8_t             dataLength;
    /** If application set this flag, PDO will be later sent by
    function CO_TPDO_process(). Depends on transmission type. */
    uint8_t             sendRequest;
    /** Pointers to 8 data objects, where PDO will be copied */
    uint8_t            *mapPointer[8];
    /** Each flag bit is connected with one mapPointer. If flag bit
    is true, CO_TPDO_process() functiuon will send PDO if
    Change of State is detected on value pointed by that mapPointer */
    uint8_t             sendIfCOSFlags;
    /** SYNC counter used for PDO sending */
    uint8_t             syncCounter;
    /** Inhibit timer used for inhibit PDO sending translated to microseconds */
    uint32_t            inhibitTimer;
    /** Event timer used for PDO sending translated to microseconds */
    uint32_t            eventTimer;
    CO_CANmodule_t     *CANdevTx;       /**< From CO_TPDO_init() */
    CO_CANtx_t         *CANtxBuff;      /**< CAN transmit buffer inside CANdev */
    uint32_t            CANdevTxIdx;    /**< From CO_TPDO_init() */
}CO_TPDO_t;
typedef struct{
    uint8_t             NMTstate;       /**< Of the remote node */
    bool_t              monStarted;     /**< True after reception of the first Heartbeat mesage */
    uint32_t            timeoutTimer;   /**< Time since last heartbeat received */
    uint32_t            time;           /**< Consumer heartbeat time from OD */
    bool_t              CANrxNew;       /**< True if new Heartbeat message received from the CAN bus */
}CO_HBconsNode_t;



typedef struct{
    CO_EM_t            *em;             /**< From CO_HBconsumer_init() */
    const uint32_t     *HBconsTime;     /**< From CO_HBconsumer_init() */
    CO_HBconsNode_t    *monitoredNodes; /**< From CO_HBconsumer_init() */
    uint8_t             numberOfMonitoredNodes; /**< From CO_HBconsumer_init() */
    /** True, if all monitored nodes are NMT operational or no node is
        monitored. Can be read by the application */
    uint8_t             allMonitoredOperational;
    CO_CANmodule_t     *CANdevRx;       /**< From CO_HBconsumer_init() */
    uint32_t            CANdevRxIdxStart; /**< From CO_HBconsumer_init() */
}CO_HBconsumer_t;

typedef struct{
	CO_CANmodule_t     *CANmodule[1];   /**< CAN module objects */
	CO_SDO_t           *SDO[CO_NO_SDO_SERVER]; /**< SDO object */
	CO_EM_t            *em;             /**< Emergency report object */
	CO_EMpr_t          *emPr;           /**< Emergency process object */
	CO_NMT_t           *NMT;            /**< NMT object */
	CO_SYNC_t          *SYNC;           /**< SYNC object */
	CO_RPDO_t          *RPDO[CO_NO_RPDO];/**< RPDO objects */
	CO_TPDO_t          *TPDO[CO_NO_TPDO];/**< TPDO objects */
	CO_HBconsumer_t    *HBcons;         /**<  Heartbeat consumer object*/
	#if CO_NO_SDO_CLIENT == 1
		CO_SDOclient_t     *SDOclient;      /**< SDO client object */
	#endif
	#if CO_NO_TRACE > 0
		CO_trace_t         *trace[CO_NO_TRACE]; /**< Trace object for monitoring variables */
	#endif
}CO_t;

#ifdef __cplusplus
}
#endif

#endif /* CO_DEFINITIONS_H_ */