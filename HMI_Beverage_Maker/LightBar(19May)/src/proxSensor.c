/** \file		proxSensor.c
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

#include "bevMakerHmi.h"
#include "proxSensor.h"
#include "proxSensor_Private.h"
#include "hmiI2c.h"
#include "display.h"
#include "mainTimer.h"


bool gProxTestEnabled;
uint8_t gWriteBuffer[10];
uint8_t gProxResult;
uint8_t gc100msTick_Local;

/** \brief .
 *
 *  \return  Nothing
 */
static void proxWriteDataByte(uint8_t reg, uint8_t val);



/** Initialise the proximity Sensor driver
	\return Nothing
*/
void proximitySensorInit(void)
{
	/* PROX_INT/ */
	/* Proximity Sensor, interrupt line, on PORTE */
	/*          Bit: 7 6 5 4  3 2 1 0 */
	/*    Direction: - - - -  - 0 - -  0=Input, 1=Output */
	/*Initial State: - - - -  - - - - */
	/*	              | | | |  | | | |  bit   */
	/*	              | | | |  | | | +-- b0 : */
	/*	              | | | |  | | +---- b1 : */
	/*	              | | | |  | +------ b2 : PROX_INT/, input with pull-up */
	/*	              | | | |  +-------- b3 : */
	/*	              | | | +----------- b4 : */
	/*	              | | +------------- b5 : */
	/*	              | +--------------- b6 : */
	/*	              +----------------- b7 : */
	PORTE.DIRCLR = 0x04;	/* Clear to input */

	/* configured to pull-up on input */
	PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc;

}


enum ProxTestState
{
	Atime_ProxTestState,
	Wtime_ProxTestState,
	Ppulse_ProxTestState,
	PoffUr_ProxTestState,
	PoffDl_ProxTestState,
	Config1_ProxTestState,
	Pers_ProxTestState,
	Config2_ProxTestState,
	Config3_ProxTestState,
	Control_ProxTestState,
	Enable_ProxTestState,
	Read_ProxTestState,
	Process_ProxTestState,
	Idle_ProxTestState,
};
typedef enum ProxTestState ProxTestState_t;
ProxTestState_t gProxTestState;

void proxTestEnable(bool bEnable)
{
	if((gProxTestEnabled == false) && (bEnable == true))
	{	/* starting the test */
		gProxTestState = Atime_ProxTestState;
				
		dspOn();
	}
	dspErase();
	
	gProxTestEnabled = bEnable;
}



/**
* @brief Configures I2C communications and initializes registers to defaults
*
* @return True if initialized successfully. False otherwise.
*/
void proxManager(void)
{
	if(gProxTestEnabled)
	{
		if(hmiI2cReady())
		{	/* HMI I2C bus is free */

			if(gProxTestState == Atime_ProxTestState)
			{
				proxWriteDataByte(APDS9960_ATIME, DEFAULT_ATIME);
				gProxTestState++;
			}
			else if(gProxTestState == Wtime_ProxTestState)
			{
				proxWriteDataByte(APDS9960_WTIME, DEFAULT_WTIME);
				gProxTestState++;
			}
			else if(gProxTestState == Ppulse_ProxTestState)
			{
				proxWriteDataByte(APDS9960_PPULSE, DEFAULT_PROX_PPULSE);
				gProxTestState++;
			}
			else if(gProxTestState == PoffUr_ProxTestState)
			{
				proxWriteDataByte(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR);
				gProxTestState++;
			}
			else if(gProxTestState == PoffDl_ProxTestState)
			{
				proxWriteDataByte(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL);
				gProxTestState++;
			}
			else if(gProxTestState == Config1_ProxTestState)
			{
				proxWriteDataByte(APDS9960_CONFIG1, DEFAULT_CONFIG1);
				gProxTestState++;
			}
			else if(gProxTestState == Pers_ProxTestState)
			{
				proxWriteDataByte(APDS9960_PERS, DEFAULT_PERS);
				gProxTestState++;
			}
			else if(gProxTestState == Config2_ProxTestState)
			{
				proxWriteDataByte(APDS9960_CONFIG2, DEFAULT_CONFIG2);
				gProxTestState++;
			}
			else if(gProxTestState == Config3_ProxTestState)
			{
				proxWriteDataByte(APDS9960_CONFIG3, DEFAULT_CONFIG3);
				gProxTestState++;
			}
			else if(gProxTestState == Control_ProxTestState)
			{
				/* Set default gain, LED, interrupts, enable power, and enable sensor */
				proxWriteDataByte(APDS9960_CONTROL, (DEFAULT_LDRIVE << 6) | (DEFAULT_PGAIN << 2));
				gProxTestState++;
			}
			else if(gProxTestState == Enable_ProxTestState)
			{
				proxWriteDataByte(APDS9960_ENABLE, (1 << POWER) | (1 << PROXIMITY));
				gProxTestState++;
			}
			else if(gProxTestState == Read_ProxTestState)
			{
				/* Read value from proximity data register */
				gWriteBuffer[0] = APDS9960_PDATA;	/* register to be accessed */
				hmiI2cWriteRead(APDS9960_I2C_ADDR, gWriteBuffer, 1, &gProxResult, 1);
				gProxTestState++;
			}
			else if(gProxTestState == Process_ProxTestState)
			{	/* Process the value read from proximity data register */
				static uint8_t gProxResult_Old = 255;

				if(gProxResult_Old == 255)
				{}
				else
				{
					dspShowVerticalLine(gProxResult_Old, 0);	/* Erase the line at the old position */
				}
				dspShowVerticalLine(gProxResult, 1);	/* draw a line on the OLED corresponding to the prox result */
				gProxResult_Old = gProxResult;

				gProxTestState++;
			}
			else if(gProxTestState == Idle_ProxTestState)
			{
				/* idle interval between reads */
				if(gc100msTick_Local != gc100msTick)
				{
					gc100msTick_Local = gc100msTick;

					gProxTestState = Read_ProxTestState;
				}
			}
			else
			{}
		}
		else
		{	/* HMI I2C bus is busy */
			/* try again next time around the main loop */
		}
	}
}



/**
* @brief Writes a single byte to the I2C device and specified register
*
* @param[in] reg the register in the I2C device to write to
* @param[in] val the 1-byte value to write to the I2C device
* @return Nothing
*/
static void proxWriteDataByte(uint8_t reg, uint8_t val)
{
	/* send data-set to IC */
	gWriteBuffer[0] = reg;	/* register to be written */
	gWriteBuffer[1] = val;	/* value to be written to the register */
	hmiI2cWrite(APDS9960_I2C_ADDR, gWriteBuffer, 2);
}


/*** END of FILE ***/