/** \file		display_Private.h
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


#ifndef DISPLAY_PRIVATE_H
#define DISPLAY_PRIVATE_H

/* Display control, on PORTF */
/* Display is configured on the schematic for parallel communication mode, 8-bit 8080 (BS1 = BS2 = 1) */
/*          Bit: 7 6 5 4  3 2 1 0 */
/*    Direction: 1 1 1 1  1 1 - -  0=Input, 1=Output */
/*Initial State: 1 1 0 0  1 0 - - */
/*	              | | | |  | | | |  bit   */
/*	              | | | |  | | | +-- b0 : */
/*	              | | | |  | | +---- b1 : */
/*	              | | | |  | +------ b2 : DSP-ON, output, Logic 1 = apply 12V5 power to the display OLED (non-logic supply). Initialise with display OFF */
/*	              | | | |  +-------- b3 : DSP-CS/, output, Logic 0 = Chip-Select, enable comms */
/*	              | | | +----------- b4 : DSP-RST/, output, Logic 0 = Reset. Initialise with display logic in reset */
/*	              | | +------------- b5 : DSP-DC/, output,  Logic 0 = Access command registers. Logic 1 = Access data registers */
/*	              | +--------------- b6 : DSP-WR/, output,  Logic 0 = Write enabled */
/*	              +----------------- b7 : DSP-RD/, output,  Logic 0 = Read enabled */

enum
{
	dspHighVoltageOn_PinMask	= PIN2_bm,
	dspChipSelect_PinMask		= PIN3_bm,
	dspReset_PinMask			= PIN4_bm,
	dspDataNotCommand_PinMask	= PIN5_bm,
	dspWriteEnable_PinMask		= PIN6_bm,
	dspReadEnable_PinMask		= PIN7_bm,
};


/* Function prototypes */

/** \brief Sends the inverse command to the OLED.
 *
 *  \return  Nothing
 */
void dspInverseEnable(void);

/** \brief Sends the do not inverse command to the OLED.
 *
 *  \return  Nothing
 */
void dspInverseDisable(void);

/** \brief Write data to the OLED.
 *
 *  \return  Nothing
 */
static void dspWriteCommand(uint8_t command);

/** \brief Set the OLED contrast.
 *
 *  \return  Nothing
 */
static void dspSetContrast(uint8_t contrast);

/** \brief Switch on the high-voltage supply to the OLED.
 *
 *  \return  Nothing
 */
static void dspPowerOnHighVoltage(void);

/** \brief Set the OLED start line command.
 *
 *  \return  Nothing
 */
static inline void dspSetStartLine(uint8_t address);

/** \brief Send a byte to the OLED display buffer.
 *
 *  \return  Nothing
 */
static void dspWriteByteToDisplay(uint8_t page, uint8_t column, uint8_t data);

/** \brief .
 *
 *  \return  Nothing
 */
static inline void dspSetPage(uint8_t address);

/** \brief .
 *
 *  \return  Nothing
 */
static inline void dspSetColumn(uint8_t address);

/** \brief .
 *
 *  \return  Nothing
 */
static inline void dspWriteData(uint8_t data);

/** \brief Switch off the high-voltage supply to the OLED.
 *
 *  \return  Nothing
 */
static void dspPowerOffHighVoltage(void);



#endif /* DISPLAY_PRIVATE_H */
/*** END of FILE ***/