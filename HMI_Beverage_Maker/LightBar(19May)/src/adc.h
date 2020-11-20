/** \file		adc.h
 *
 *  \brief		read various ADC channels
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#ifndef ADC_H
#define ADC_H
#ifdef __cplusplus
extern "C"{
	#endif

/** \brief Initialise the ADC in preparation for readings.
 *
 *  \return  Nothing
 */
void adcInit(void);

/** \brief Manage conversions on various ADC channels.
 *
 *  \return  Nothing
 */
void adcManager(void);

/** \brief Enable/Disabled the ambient light sensor test.
 *
 *  \return  Nothing
 */
void adcAmbientLightTestEnable(bool bEnable);

#ifdef __cplusplus
}
#endif

#endif /* ADC_H */
/*** END of FILE ***/