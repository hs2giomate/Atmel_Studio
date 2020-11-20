/** \file		proxSensor.h
 *
 *  \brief		Test the proximity sensor
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef PROXSENSOR_H
#define PROXSENSOR_H
#ifdef __cplusplus
extern "C"{
	#endif

/** \brief Initialise the proximity Sensor driver .
 *
 *  \return  Nothing
 */
void proximitySensorInit(void);

/** \brief Enable the  proximity sensor test.
 *
 *  \return  Nothing
 */
void proxTestEnable(bool bEnable);

/** \brief Manage the proximity sensor test.
 *
 *  \return  Nothing
 */
void proxManager(void);

#ifdef __cplusplus
}
#endif
#endif /* PROXSENSOR_H */
/*** END of FILE ***/