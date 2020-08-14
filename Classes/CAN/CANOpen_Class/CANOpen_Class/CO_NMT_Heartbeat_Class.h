/* 
* CO_NMT_Heartbeat.h
*
* Created: 8/5/2020 2:24:09 PM
* Author: GMateusDP
*/


#ifndef __CO_NMT_HEARTBEAT_CLASS_H__
#define __CO_NMT_HEARTBEAT_CLASS_H__
#include "CO_Definitions.h"
#include "CO_Emergency_Class.h"

    #define LED_FLICKERING(NMT)     (((NMT)->LEDflickering>=0)     ? 1 : 0) /**< 10HZ (100MS INTERVAL) */
    #define LED_BLINKING(NMT)       (((NMT)->LEDblinking>=0)       ? 1 : 0) /**< 2.5HZ (400MS INTERVAL) */
    #define LED_SINGLE_FLASH(NMT)   (((NMT)->LEDsingleFlash>=0)    ? 1 : 0) /**< 200MS ON, 1000MS OFF */
    #define LED_DOUBLE_FLASH(NMT)   (((NMT)->LEDdoubleFlash>=0)    ? 1 : 0) /**< 200MS ON, 200MS OFF, 200MS ON, 1000MS OFF */
    #define LED_TRIPLE_FLASH(NMT)   (((NMT)->LEDtripleFlash>=0)    ? 1 : 0) /**< 200MS ON, 200MS OFF, 200MS ON, 200MS OFF, 200MS ON, 1000MS OFF */
    #define LED_QUADRUPLE_FLASH(NMT)(((NMT)->LEDquadrupleFlash>=0) ? 1 : 0) /**< 200MS ON, 200MS OFF, 200MS ON, 200MS OFF, 200MS ON, 200MS OFF, 200MS ON, 1000MS OFF */
    #define LED_GREEN_RUN(NMT)      (((NMT)->LEDgreenRun>=0)       ? 1 : 0) /**< CANOPEN RUN LED ACCORDING TO CIA DR 303-3 */
    #define LED_RED_ERROR(NMT)      (((NMT)->LEDredError>=0)       ? 1 : 0) /**< CANopen error LED according to CiA DR 303-3 */
/** @} */


/**
 * Internal network state of the CANopen node
 */




class CO_NMT_Heartbeat_Class
{
//variables
public:
protected:
private:
	 CO_NMT_t               *NMT;
//functions
public:
	CO_NMT_Heartbeat_Class();
	~CO_NMT_Heartbeat_Class();
	CO_ReturnError_t NMT_Init(
        CO_NMT_t               *NMT_Init,
        CO_EMpr_t              *emPr,
        uint8_t                 nodeId,
        uint32_t                firstHBTime,
        CO_CANmodule_t         *NMT_CANdev,
        uint32_t                NMT_rxIdx,
        uint32_t                CANidRxNMT,
        CO_CANmodule_t         *HB_CANdev,
        uint32_t                HB_txIdx,
        uint32_t                CANidTxHB);


void CO_NMT_initCallback(
             ptrFuncNMT         ptrState);



void CO_NMT_blinkingProcess50ms(void);


CO_NMT_reset_cmd_t NMT_Process(
         uint32_t                timeDifference_ms,
        uint32_t                HBtime,
        uint32_t                NMTstartup,
        uint8_t                 errorRegister,
        const uint8_t           errorBehavior[],
        uint32_t               *timerNext_ms);



CO_NMT_internalState_t CO_NMT_getInternalState(void);
protected:
private:
	CO_NMT_Heartbeat_Class( const CO_NMT_Heartbeat_Class &c );
	CO_NMT_Heartbeat_Class& operator=( const CO_NMT_Heartbeat_Class &c );

}; //CO_NMT_Heartbeat

#endif //__CO_NMT_HEARTBEAT_H__
