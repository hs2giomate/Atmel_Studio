/** \file		buttons.c
 *
 *  \brief		buttons manager
 *
 *  Called from the main loop to scans the buttons, detect changes and report
 *  changes .
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#include "buttons.h"


/* Function prototypes */

/** \brief De-assert all column drive to the keypad.
 *
 *  \return  Nothing
 */


/* Global variables */
uint8_t gcColumnDrvNum;
static const uint8_t gcBtnColumnToPinNumMap_Array[COLUMN_COUNT] = {3, 4};
static const uint8_t gc_BtnRowToPinNumMap_Array[ROW_COUNT] = {6};
ButtonState_t gOnOffButton;
ButtonState_t gButtonStates_Array[MATRIX_BUTTON_COUNT];






/** Initialise the User Interface Buttons driver
	\return Nothing
*/   
void btnInitButtons(void)
{
	/* User-Interface Buttons on PORTB */
	/* SW-PWR */
	/* SW-COL1 */
	/* SW-COL2 */
	/* SW-COL3 */
	/* SW-ROW1 */
	/* SW-ROW2 */
	/*          Bit: 7 6 5 4  3 2 1 0 */
	/*    Direction: 0 0 0 0  0 0 - -  0=Input, 1=Output */
	/*Initial State: - - - -  - - - - */
	/*	              | | | |  | | | |  bit   */
	/*	              | | | |  | | | +-- b0 : */
	/*	              | | | |  | | +---- b1 : */
	/*	              | | | |  | +------ b2 : SW-PWR, input with pull-up */
	/*	              | | | |  +-------- b3 : SW-COL1, initialise to input, high-impedance */
	/*	              | | | +----------- b4 : SW-COL2, initialise to input, high-impedance */
	/*	              | | +------------- b5 : SW-COL3, input with pull-up, reserved for future expansion */
	/*	              | +--------------- b6 : SW-ROW1, input with pull-up  */
	/*	              +----------------- b7 : SW-ROW2, input with pull-up, reserved for future expansion */
	PORTB.DIRCLR = 0xFC;	/* Clear to input */
	
	PORTB.PIN2CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */
	PORTB.PIN3CTRL = PORT_OPC_INPUT_HIGH_IMPEDANCE;	/* input, high-impedance */
	PORTB.PIN4CTRL = PORT_OPC_INPUT_HIGH_IMPEDANCE;	/* input, high-impedance */
	PORTB.PIN5CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */
	PORTB.PIN6CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */
	PORTB.PIN7CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */

	/* User-Interface Buttons row on PORTD */
	/* SW-ROW3, reserved for future expansion */
	PORTD.DIRCLR = 0x01;	/* Clear to input */
	PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */

	/* next column drive */
	btnDriveButtonColumn();
}


/*
	Strategy :
		All Columns pullup to logic 1, by default
		Enable pull-ups on all receive rows.
		Drive one column at a time to logic 0, all other columns MUST be high impedance to avoid a short across the power rails !
			Read the receiver rows,
				If any Rx row is logic 0, the corresponding button is active
		Drive all Columns back to logic 1, briefly, to see-off any capacitance effects,
		Set all Columns as inputs,
			Wait a short interval,
			Read the receiver rows,
				Check all Rx rows are logic 1, indicating all buttons inactive, otherwise report a fault
		Advance to next column and Repeat

*/

static void btnReleaseKeyboardAllColumn(void)
{
	/* All column drive to high impedance  */
	PORTB.DIRCLR = 0x5C;	/* Clear to input */
	
	PORTB.PIN3CTRL = PORT_OPC_INPUT_HIGH_IMPEDANCE;	/* input, high-impedance */
	PORTB.PIN4CTRL = PORT_OPC_INPUT_HIGH_IMPEDANCE;	/* input, high-impedance */
}

static void btnDriveButtonColumn(void)
{
	uint8_t cPinNum;
	uint8_t cPinMask;

	/* All column drive to high impedance first, to avoid short across power rails */
	btnReleaseKeyboardAllColumn();

	/* Drive the target column to logic 0 */
	cPinNum = gcBtnColumnToPinNumMap_Array[gcColumnDrvNum];
	cPinMask = (0x01 << cPinNum);
	
    register8_t* pinControl = &(PORTB.PIN0CTRL);
	pinControl += cPinNum;
	*pinControl = 0;	/* totem-pole output */
	PORTB.DIRSET = cPinMask;		/* Set as output */
	PORTB.OUTCLR = cPinMask;	/* Logic LOW */
}

static void btnCheckWhichButtonsActive(void)
{
	uint8_t cRow;
	uint8_t cButtonNum;
	bool bButtonActive;
	bool bConfirmedStateIsChanged;
	ButtonState_t* pButton;
	
	for(cRow = 0; cRow <= HIGHEST_VALID_ROW_NUM; cRow++)
	{
		bButtonActive = btnIsButtonActiveOnRow(cRow);
		cButtonNum = btnColumnAndRowToBtnNum(gcColumnDrvNum, cRow);
		
		pButton = &(gButtonStates_Array[cButtonNum]);
		bConfirmedStateIsChanged = btnUpdateButtonState(pButton, bButtonActive);
		if(bConfirmedStateIsChanged)
		{
			pscSetPublicButtonState(cButtonNum, pButton->m_bActive_Confirmed);
		}
	}
	
	/* Always check the power button, which is not part of the matrix */
	bButtonActive = btnIsOnOffButtonActive();
	pButton = &gOnOffButton;
	bConfirmedStateIsChanged = btnUpdateButtonState(pButton, bButtonActive);
	if(bConfirmedStateIsChanged)
	{
		pscSetPublicButtonState(BtnLeft, pButton->m_bActive_Confirmed);
	}
}

/*
	Defines the button numbering internal to the HMI
		(Internal means not shared publicly with the PSC)
	The button numbering is unique for each button and allocated from the 
		hardware drive matrix Column and Row.
	The buttonNum is ZERO-based
	The matrix Column & Row are ZERO-based
*/
static uint8_t btnColumnAndRowToBtnNum(uint8_t cColumnNum, uint8_t cRow)
{
	uint8_t cButtonNum;

	cButtonNum = cColumnNum * ROW_COUNT;
	cButtonNum += cRow;

	return(cButtonNum);
}

/*
	returns TRUE if the confirmed button state has been changed
*/
static bool btnUpdateButtonState(ButtonState_t* pButton, bool bButtonActive)
{
	if(pButton->m_bUnconfirmedState_Active != bButtonActive)
	{	/* The button state has changed / is changing */
		/* Update the time-line mark, to track the time of the last change */
		pButton->m_dw_TimeOfLastChange_ms = gdw_msTimeCount;

		pButton->m_bUnconfirmedState_Active = bButtonActive;
	}

	return btnDebounceButton(pButton);
}

static bool btnDebounceButton(ButtonState_t* pButton)
{
	uint32_t dwTimeDiff;
	bool bConfirmedStateIsChanged = false;

	if(pButton->m_bActive_Confirmed != pButton->m_bUnconfirmedState_Active)
	{	/* Recent change to button state, not yet made public */
		/* update the public state only if the de-bounce interval has elapsed since the last change */
		if(pButton->m_dw_TimeOfLastChange_ms > gdw_msTimeCount)
		{	/* The global counter has wrapped since this button changed state */
			/* Start the interval again */
			pButton->m_dw_TimeOfLastChange_ms = gdw_msTimeCount;
		}
		
		dwTimeDiff = gdw_msTimeCount -pButton->m_dw_TimeOfLastChange_ms;
		if(dwTimeDiff >= BUTTON_DEBOUNCE_INTERVAL_MS)
		{	/* de-bounce interval complete */
			bool bButtonActive = pButton->m_bUnconfirmedState_Active;
			/* Update the public / reported state */
			pButton->m_bActive_Confirmed = bButtonActive;

			bConfirmedStateIsChanged = true;	/* Indicate the confirmed button state is changed */
		}
	}

	return bConfirmedStateIsChanged;
}

void btnManageButtonScan(void)
{
	btnCheckWhichButtonsActive();

	/* Release all column drive */
	btnReleaseKeyboardAllColumn();

	/* Advance to next column */
	gcColumnDrvNum++;
	if(gcColumnDrvNum > HIGHEST_VALID_COLUMN_NUM)
	{	/* Matrix scan complete */
		/* Start again at first column */
		gcColumnDrvNum = 0;
	}

	/* next column drive */
	btnDriveButtonColumn();
}

static bool btnIsButtonActiveOnRow(uint8_t cRow)
{
	bool bActive;
	uint8_t cPinNum;
	uint8_t cPinMask;

	bActive = false;
	cPinNum = gc_BtnRowToPinNumMap_Array[cRow];
	cPinMask = (0x01 << cPinNum);

	if((PORTB.IN & cPinMask) == 0)
	{	/* button indicating active */
		bActive = true;
	}

	return(bActive);
}


static bool btnIsOnOffButtonActive(void)
{
	bool bActive;
	uint8_t cPinMask;

	bActive = false;
	cPinMask = PIN2_bm;

	if((PORTB.IN & cPinMask) == 0)
	{	/* button indicating active */
		bActive = true;
	}

	return(bActive);
}

/**** END OF FILE ****/


