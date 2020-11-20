/** \file		display.h
 *
 *  \brief		OLED display manager.
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef __cplusplus
extern "C"{
	#endif
/** \brief Assert the OLED display reset signal and hold until the display initialisation.
 *
 *  \return  Nothing
 */
void dspResetHold(void);

/** \brief Initialise OLED display.
 *
 *  \return  Nothing
 */
void dspInit(void);

/** \brief Switch-on the OLED.
 *
 *  \return  Nothing
 */
void dspOn(void);

/** \brief Switch-off the OLED.
 *
 *  \return  Nothing
 */
void dspOff(void);

/** \brief Show a vertical black or white line at the given position on the OLED.
 *
 *  \return  Nothing
 */
void dspShowVerticalLine(uint8_t x, bool white);

/** \brief Erase the OLED.
 *
 *  \return  Nothing
 */
void dspErase(void);

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_H */
/*** END of FILE ***/