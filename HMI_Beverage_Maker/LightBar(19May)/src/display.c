/** \file		display.c
 *
 *  \brief		OLED display manager.
 *
 *  Functions to drive the display for HALT
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#include "bevMakerHmi.h"
#include "display.h"
#include "display_Private.h"
#include "mainTimer.h"



/** \brief Initialize the registers in the OLED controller.
 *
 *  \return  Nothing
 */
static void initSsd1309(void);

/** \brief Write to one half of the OLED display buffer, so half the display appears on and the other half off.
 *
 *  \return  Nothing
 */
static void dspBlackWhiteHalf(void);

/** \brief Write data to one section of the OLED.
 *
 *  \return  Nothing
 */
static void dspWritePage(uint8_t pixelData, uint8_t page,uint8_t column, uint8_t width);





/* Display Commands */
enum
{
	dspLoNibbleColumnStartAddress_Command		= 0x00,
	dspHiNibbleColumnStartAddress_Command		= 0x10,
	dspMemoryAddressingMode_Command				= 0x20,
	dspStartLine_Command						= 0x40,
	dspContrast_Command							= 0x81,
	dspSegmentRemap_MapCol0ToSeg0_Command  = 0xA0,
	dspSegmentRemap_MapCol127ToSeg0_Command  = 0xA1,
	dspEntireDisplayOn_Command					= 0xA4,
	dspNonInverse_Command						= 0xA6,
	dspInverse_Command							= 0xA7,
	dspMultiplexRatio_Command					= 0xA8,
	dspOn_Command								= 0xAF,
	dspOff_Command								= 0xAE,
	dspPageStartAddress_Command					= 0xB0,
	dspCOMoutputScanDirectionDown_Command       = 0xC8,
	dspOffset_Command							= 0xD3,
	dspDisplayClockDivideRatio_Command			= 0xD5,
	dspPreChargePeriod_Command					= 0xD9,
	dspCOMpins_Command							= 0xDA,
	dspVcomhDeselectLevel_Command				= 0xDB,
};

enum
{
	oledWidth_PixelCount			= 128,
	oledHeight_PixelCount			= 32,
	oled_PixelCountPerByte			= 8,
	oledPageCount					= 4,
};

/**
 * \brief Set the OLED contrast level
 *
 * \param contrast a number between 0 and 0xFF
 *
 * \retval contrast the contrast value written to the OLED controller
 */
static inline void dspSetContrast(uint8_t contrast)
{
	dspWriteCommand(dspContrast_Command);
	dspWriteCommand(contrast);
}

/**
 * \brief Invert all pixels on the device
 *
 * This function will invert all pixels on the OLED
 *
 */
void dspInverseEnable(void)
{
	dspWriteCommand(dspInverse_Command);
}

/**
 * \brief Disable invert of all pixels on the device
 *
 * This function will disable invert on all pixels on the OLED
 *
 */
void dspInverseDisable(void)
{
	dspWriteCommand(dspNonInverse_Command);
}

/**
 * \brief Set the display start draw line address
 *
 * This function will set which line should be the start draw line for the OLED.
 */
static inline void dspSetStartLine(uint8_t cLine)
{
	uint8_t cCommand;
	
	/* address is 6 bits */
	cLine &= 0x3F;
	cCommand = dspStartLine_Command;
	cCommand |= cLine;
	dspWriteCommand(cCommand);
}

/**
 * \brief Write a byte to the display controller RAM
 *
 *
 * \param page Page address
 * \param column Page offset (x coordinate)
 * \param data Data to be written
 */
static void dspWriteByteToDisplay(uint8_t page, uint8_t column, uint8_t data)
{
	dspSetPage(page);
	dspSetColumn(column);

	dspWriteData(data);
}

//! \name Address setup for the OLED
//@{
/**
 * \brief Set current page in display RAM
 *
 * This command is usually followed by the configuration of the column address
 * because this scheme will provide access to all locations in the display
 * RAM.
 *
 * \param address the page address
 */
static inline void dspSetPage(uint8_t cPage)
{
	uint8_t cCommand;
	
	// the Page is 3 bits (8 pages)
	cPage &= 0x07;
	
	cCommand = dspPageStartAddress_Command;
	cCommand |= cPage;
	
	dspWriteCommand(cCommand);
}

#define hiNibble(v) (v >> 4)
#define loNibble(v) (v & 0x0F)
/**
 * \brief Set current column in display RAM
 *
 * \param address the column address
 */
static inline void dspSetColumn(uint8_t cColumn)
{
	uint8_t cCommand;
	
	// the cColumn is 7 bits
	cColumn &= 0x7F;
	
	cCommand = dspHiNibbleColumnStartAddress_Command;
	cCommand |= hiNibble(cColumn);
	dspWriteCommand(cCommand);	
	
	cCommand = dspLoNibbleColumnStartAddress_Command;
	cCommand |= loNibble(cColumn);
	dspWriteCommand(cCommand);
}

/** Assert the OLED display reset signal and hold until the display initialisation
	\return Nothing
*/   
void dspResetHold(void)
{
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
	PORTF.OUTCLR = 0x34;	/* Clear output drive to logic 0 */
	PORTF.OUTSET = 0xC8;	/* Set output to logic 1 */
	PORTF.DIRSET = 0xFC;	/* Set outputs */
}

/** Initialise OLED display
	\return Nothing
*/   
void dspInit(void)
{
	/* Note that the display reset line is pulled low early in the initialisation procedure.
		This ensures the LOW reset pulse meets the minimum requirement of 3 us */
	
	/* Configure Port A, for writing to the display (totem-pole) */
	PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;	/* Bit0 */
	PORTA.PIN1CTRL = PORT_OPC_TOTEM_gc;	/* Bit1 */
	PORTA.PIN2CTRL = PORT_OPC_TOTEM_gc;	/* Bit2 */
	PORTA.PIN3CTRL = PORT_OPC_TOTEM_gc;	/* Bit3 */
	PORTA.PIN4CTRL = PORT_OPC_TOTEM_gc;	/* Bit4 */
	PORTA.PIN5CTRL = PORT_OPC_TOTEM_gc;	/* Bit5 */
	PORTA.PIN6CTRL = PORT_OPC_TOTEM_gc;	/* Bit6 */
	PORTA.PIN7CTRL = PORT_OPC_TOTEM_gc;	/* Bit7 */

	PORTA.OUT = 0x00;	/* Set output logic 0 */
	PORTA.DIR = 0xFF;	/* display bus set for writing (output) */

	/* Display control, on PORTF */
	/* Display is configured on the schematic for parallel communication mode, 8-bit 8080 (BS1 = BS2 = 1) */
	/*          Bit:  7 6 5 4  3 2 1 0 */
	/*    Direction:  1 1 1 1  1 1 - -  0=Input, 1=Output */
	/*Initial State:  1 1 0 1  1 0 - - */
	/*	               | | | |  | | | |  bit   */
	/*	               | | | |  | | | +-- b0 : */
	/*	               | | | |  | | +---- b1 : */
	/*	               | | | |  | +------ b2 : DSP-ON, output, Logic 1 = apply 12V5 power to the display OLED (non-logic supply). Initialise with display OFF */
	/*	               | | | |  +-------- b3 : DSP-CS/, output, Logic 0 = Chip-Select, enable comms */
	/*	               | | | +----------- b4 : DSP-RST/, output, Logic 1 = Release Reset */
	/*	               | | +------------- b5 : DSP-DC/, output,  Logic 0 = Access command registers. Logic 1 = Access data registers */
	/*	               | +--------------- b6 : DSP-WR/, output,  Logic 0 = Write enabled */
	/*	               +----------------- b7 : DSP-RD/, output,  Logic 0 = Read enabled */
	PORTF.OUTCLR = 0x24;	/* Clear output drive to logic 0 */
	PORTF.OUTSET = 0xD8;	/* Set output to logic 1 */
	PORTF.DIRSET = 0xFC;	/* Set outputs. GPIO direction for outputs */

	/* All display control bits configured to output, totem-pole */
	PORTF.PIN2CTRL = PORT_OPC_TOTEM_gc;
	PORTF.PIN3CTRL = PORT_OPC_TOTEM_gc;
	PORTF.PIN4CTRL = PORT_OPC_TOTEM_gc;
	PORTF.PIN5CTRL = PORT_OPC_TOTEM_gc;
	PORTF.PIN6CTRL = PORT_OPC_TOTEM_gc;
	PORTF.PIN7CTRL = PORT_OPC_TOTEM_gc;
	


	/* Initialize the low-level display controller. */
	initSsd1309();

	
	/* Display bitmap on right-hand side of display */
	dspBlackWhiteHalf();
}

//! \name Display hardware control
//@{
/**
 * \brief Turn the OLED display on
 */
void dspOn(void)
{
	dspPowerOnHighVoltage();
	
	dspWriteCommand(dspOn_Command);
	dspInverseDisable();
}

/** Switch on the high voltage to the OLED (not the logic supply, which is permanently powered).
	See the SSD1309 datasheet for more details.
	\return Nothing
*/   
static void dspPowerOnHighVoltage(void)
{
	/* Switch on the high voltage to the OLED (not the logic supply, which is permanently powered) */
	PORTF.OUTSET = 0x04;	/* Set output to logic 1 */
}

void dspOff(void)
{
	/* Command the display OFF */
	dspWriteCommand(dspOff_Command);
	
	dspPowerOffHighVoltage();
}

/** Switch off the high voltage to the OLED (not the logic supply, which is permanently powered).
	This function should be called when the PSC signals the HMI to switch to low power mode.
	See the SSD1309 datasheet for more details.
	\return Nothing
*/   
static void dspPowerOffHighVoltage(void)
{
	/* Switch off the high voltage to the OLED (not the logic supply, which is permanently powered) */
	PORTF.OUTCLR = 0x04;	/* Set output to logic 0 */
}

static void initSsd1309(void)
{
	// 1/32 Duty (0x0F~0x3F)
	dspWriteCommand(dspMultiplexRatio_Command);
	dspWriteCommand(0x1F);

	// Shift Mapping RAM Counter (0x00~0x3F)
	dspWriteCommand(dspOffset_Command);
	dspWriteCommand(0x00);

	// Set Mapping RAM Display Start Line (0x00~0x3F)
//	dspWriteCommand(dspStartLine_Command(0x00));

	dspWriteCommand(dspMemoryAddressingMode_Command);
	dspWriteCommand(0x00);


	// Set Column Address 0 Mapped to SEG0
	dspWriteCommand(dspSegmentRemap_MapCol0ToSeg0_Command);

	// Set COM/Row Scan Scan from COM63 to 0
	//dspWriteCommand(dspCOMoutputScanDirectionDown_Command);
	dspWriteCommand(0xC0);

	// Set COM Pins hardware configuration
	dspWriteCommand(dspCOMpins_Command);
	dspWriteCommand(0x12);

	dspSetContrast(0xFF);


	// Set Display Clock Divide Ratio / Oscillator Frequency (Default => 0x80)
	dspWriteCommand(dspDisplayClockDivideRatio_Command);
	dspWriteCommand(0x80);

	// Set VCOMH Deselect Level
	dspWriteCommand(dspVcomhDeselectLevel_Command);
	dspWriteCommand(0x40);

	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	dspWriteCommand(dspPreChargePeriod_Command);
	dspWriteCommand(0xF1);


	// Disable Entire display On
	dspWriteCommand(dspEntireDisplayOn_Command);
	dspInverseDisable();
	
/*	dspOn(); leave display off until commanded on by test box */
	/* The OLED datasheet states :
		"After Vcc is stable, send command AFh for display ON. SEG/COM will be ON after 100ms" */
	
	/* Set display to output data from line 0 */
	dspSetStartLine(0);

	uint8_t page;
	uint8_t column;
	/* Clear the contents of the display.
	 */
	for (page = 0; page < oledPageCount; page++) 
	{
		for (column = 0; column < oledWidth_PixelCount; column++) 
		{
			dspWriteByteToDisplay(page, column, 0x00);
		}
	}
}

/**
 * \brief Write data to the display controller
 *
 * This functions sets the pin D/C# before writing to the controller. Different
 * data write function is called based on the selected interface.
 *
 * \param cData the data to write
 */
static inline void dspWriteData(uint8_t cData)
{
	/* Set the Data Command/ pin for Data */
	PORTF.OUTSET = dspDataNotCommand_PinMask;	/* Logic HIGH */

	PORTF.OUTCLR = dspChipSelect_PinMask | dspWriteEnable_PinMask;	/* Logic LOW */
	
	/* Set the byte to be written on the parallel bus */
	PORTA.OUT = cData;
	
	/* The data is latched into the display on the rising edge of the Write line */
	PORTF.OUTSET = dspWriteEnable_PinMask;
	
	PORTF.OUTSET = dspChipSelect_PinMask;
}

/**
 * \brief Writes a command to the display controller
 *
 * This functions pull pin D/C# low before writing to the controller. Different
 * data write function is called based on the selected interface.
 *
 * \param cCommand the command to write
 */
static void dspWriteCommand(uint8_t cCommand)
{
	/* Set the Data Command/ pin for Command */
	PORTF.OUTCLR = dspDataNotCommand_PinMask;	/* Logic LOW */

	PORTF.OUTCLR = dspChipSelect_PinMask | dspWriteEnable_PinMask;	/* Logic LOW */
	
	/* Set the byte to be written on the parallel bus */
	PORTA.OUT = cCommand;
	
	/* The data is latched into the display on the rising edge of the Write line */
	PORTF.OUTSET = dspWriteEnable_PinMask;
	
	PORTF.OUTSET = dspChipSelect_PinMask;
}


uint32_t gdwSeconds_DisplayUpdate;
bool gbLeftOn;


static void dspBlackWhiteHalf(void)
{
	uint8_t x = 64;
	uint8_t y = 0;
	uint8_t pixelData = 0xFF;
	uint8_t num_pages = 4;
	uint8_t page = y / 8;
	uint8_t i;
	uint8_t width = 64;

	for (i = 0; i < num_pages; i++)
	{
		dspWritePage(pixelData, page + i, x, width);
	}
}


void dspErase(void)
{
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t pixelData = 0x00;
	uint8_t num_pages = 4;
	uint8_t page = y / 8;
	uint8_t i;
	uint8_t width = 128;

	for (i = 0; i < num_pages; i++)
	{
		dspWritePage(pixelData, page + i, x, width);
	}
}

void dspShowVerticalLine(uint8_t xPos, bool white)
{
	uint8_t x = (xPos & 0x7F);		/* valid x is 0 to 127 */
	uint8_t y = 0;
	uint8_t num_pages = 4;
	uint8_t page = y / 8;
	uint8_t i;
	uint8_t width = 1;
	uint8_t pixelData;

	if(white)
	{
		pixelData = 0xFF;		
	}
	else
	{
		pixelData = 0x00;
	}
	
	for (i = 0; i < num_pages; i++)
	{
		dspWritePage(pixelData, page + i, x, width);
	}
}

static void dspWritePage(uint8_t pixelData, uint8_t page,uint8_t column, uint8_t width)
{
	dspSetPage(page);
	dspSetColumn(column);

	do 
	{
		dspWriteData(pixelData);
	} while (--width);
}


/*** END of FILE ***/