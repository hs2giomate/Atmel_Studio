/** \file		pscEvent.h
 *
 *  \brief		Provide a collection point for HMI data to be passed to the HALT test box
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 
#ifndef PSC_EVENT_H
#define PSC_EVENT_H


/* types */
#ifdef __cplusplus
extern "C"{
	#endif

/* function prototypes */

/** \brief Update the on/off state of the given button in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscSetPublicButtonState(uint8_t cButton, bool bActive);

/** \brief Update the sequence count in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscSetSequenceCount(uint32_t sequenceCount);

/** \brief Update the results of the test on the SPI external flash memory in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscOnExternalFlashResult(bool bSuccess, uint32_t externalFlashMemSuccessCount, uint32_t externalFlashMemFailedCount);

/** \brief Update the information about the micro-controller reset in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscSetLastReasonForReset(uint8_t lastReasonForReset);

/** \brief Update the count of seconds since reset in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscSetSecondsAlive(uint32_t secondsAlive);

/** \brief Update the ADC result for temperature in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscSetTemperatureAdcResult(uint16_t temperatureAdcResult);

/** \brief Update the ADC result for the micro-controller Vcc in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscSetVccAdcResult(uint16_t vccAdcResult);

/** \brief Update the ADC result for the HMI supply voltage in the buffer used to return information to the test-box.
 *
 *  \return  Nothing
 */
void pscSetHmiSupplyVoltageAdcResult(uint16_t temperatureAdcResult);

#ifdef __cplusplus
}
#endif
#endif
/*** END of FILE ***/
