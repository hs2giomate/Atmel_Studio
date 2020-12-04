/** \file		buttons.h
 *
 *  \brief		buttons manager
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef BUTTONS_H
#define BUTTONS_H
	#include "buttons_Private.h"
	#include "mainTimer.h"
	#include "pscEvent.h"
	#include "bevMakerHmi.h"

#ifdef __cplusplus
extern "C"{
	#endif


extern ButtonState_t gButtonStates_Array[];
extern ButtonState_t gOnOffButton;
static void btnReleaseKeyboardAllColumn(void);

/** \brief Setup the drive on one column of the keypad.
 *
 *  \return  Nothing
 */
static void btnDriveButtonColumn(void);

/** \brief Determine which buttons are pressed for the currently active column.
 *
 *  \return  Nothing
 */
static void btnCheckWhichButtonsActive(void);

/** \brief translate a column and row to the corresponding button number.
 *
 *  \return  Nothing
 */
static uint8_t btnColumnAndRowToBtnNum(uint8_t cColumnNum, uint8_t cRow);

/** \brief Handle state changes at a specific button.
 *
 *  \return  Nothing
 */
static bool btnUpdateButtonState(ButtonState_t* pButton, bool bButtonActive);

/** \brief Determine if a button is pressed on a specific row of the keypad, while a column drive is active.
 *
 *  \return  Nothing
 */
static bool btnIsButtonActiveOnRow(uint8_t cRow);

/** \brief Determine if the on/off button is pressed.
 *
 *  \return  Nothing
 */
static bool btnIsOnOffButtonActive(void);

/** \brief handle de-bounce for the given button.
 *
 *  \return  Nothing
 */
static bool btnDebounceButton(ButtonState_t* pButton);

/** \brief Initialise the User Interface Buttons driver.
 *
 *  \return  Nothing
 */
void btnInitButtons(void);

/** \brief Manage scanning of the keypad.
 *
 *  \return  Nothing
 */
void btnManageButtonScan(void);

enum
{
	BtnCentre = 0,
	BtnRight	= 1,
	BtnLeft	= 100,	/* This button is not part of the matrix */
};
#ifdef __cplusplus
}
#endif
#endif /* BUTTONS_H */
/*** END of FILE ***/