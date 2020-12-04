/** \file		adc_Private.h
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


#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H




enum
{
	gAdcAvccChannel = 0,
	gAdcTemperatureChannel = 1,
	gHmiSupplyVoltageChannel = 2,
	gAmbientLightChannel = 3,
	gAdcHighestValidChannel = 3,
};


#endif /* ADC_PRIVATE_H */
/*** END of FILE ***/