
/** \file		led.h
 *
 *  \brief		LED indicators
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 





/** \brief Initialise the LED module that manages the HALT traffic light LEDs
 *
 *  \return  Nothing
 */
void ledInit(void);

/** \brief Set all LEDs off
 *
 *  \return  Nothing
 */
void ledAllOff(void);

/** \brief Toggle the on/off state of the green LED
 *
 *  \return  Nothing
 */
void ledGreenToggle(void);

/** \brief Toggle the on/off state of the amber LED
 *
 *  \return  Nothing
 */
void ledAmberToggle(void);

/** \brief Toggle the on/off state of the red LED
 *
 *  \return  Nothing
 */
void ledRedToggle(void);

/** \brief Set the green LED on
 *
 *  \return  Nothing
 */
void ledGreenOn(void);

/** \brief Set the green LED off
 *
 *  \return  Nothing
 */
void ledGreenOff(void);

/** \brief Set the amber LED on
 *
 *  \return  Nothing
 */
void ledAmberOn(void);

/** \brief Set the amber LED off
 *
 *  \return  Nothing
 */
void ledAmberOff(void);

/** \brief Set the red LED on
 *
 *  \return  Nothing
 */
void ledRedOn(void);

/** \brief Set the redLED off
 *
 *  \return  Nothing
 */
void ledRedOff(void);




/*** END of FILE ***/