/** \file		rs485test.c
 *
 *  \brief		test the RS485 hardware with a simple loop-back connection
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
#include "rs485test.h"
#include "usart_driver.h"
#include "display.h"
#include "mainTimer.h"

#define USART USARTD0

bool gRs485testEnabled;

void rs485testInit(void)
{
	/* set TXD as output */
	PORTD.DIRSET = PIN3_bm;

	/* set RXD as input */
	PORTD.DIRCLR = PIN2_bm;
	
	/* Pullup on RxD */
	PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */
	
	
	/* DriverEnable (DE) is active high */
	PORTD.DIRSET = PIN1_bm;
	PORTD.OUTSET = PIN1_bm;

	/* 8 Data bits, No Parity, 1 Stop bit. */
	USART_Format_Set(&USART, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);

	/* Set Baudrate to 9600 bps:
		* Use the default I/O clock frequency that is 8 MHz.
		* Do not use the baudrate scale factor
		*
		* Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
		*                 = 51
		*/
	USART_Baudrate_Set(&USART, 51, 0);

	/* Enable both RX and TX. */
	USART_Rx_Enable(&USART);
	USART_Tx_Enable(&USART);
}

bool gWaitingForLoopBack;
uint8_t gSendData = 255;
uint8_t gLineColour;
uint8_t gc10ms_rs485TestInterval;

void rs485testManager(void)
{
	if(gRs485testEnabled)
	{
		if(gWaitingForLoopBack)
		{
			if(USART_IsRXComplete(&USART))
			{
				gWaitingForLoopBack = false;
				
				uint8_t receivedData = USART_GetChar(&USART);
				if(receivedData == gSendData)
				{	/* The byte Rx matches the byte Tx */
					/* Success */
					
					if((gSendData & 0x7F) == 0)
					{	/* after each scan across the display, invert the colour drawn */
						gLineColour++;
						if(gLineColour > 1)
						{
							gLineColour = 0;
						}
					}
					
					/* Show a vertical line on the OLED display at each x position equating to a successful test with that x value */
					dspShowVerticalLine(gSendData, gLineColour);
				}
				else
				{	/* failure */
					/* leave the OLED blank at this location */
				}
			}
			else if(gc10ms_rs485TestInterval != gc10msTick)
			{	/* a long time since the Tx */
				/* failure */
				/* reset and move on to next Tx */
				gWaitingForLoopBack = false;
			}
			else
			{}
		}
		else if(gc10ms_rs485TestInterval == gc10msTick)
		{}/* idle interval between tests */
		else
		{	/* Transmit next byte */
			if(USART_IsTXDataRegisterEmpty(&USART))
			{	/* Tx register is empty and available for next Tx */
				gSendData++;	/* transmit a different value every time */
				
				USART_PutChar(&USART, gSendData);
			
				gWaitingForLoopBack = true;	/* Indicate waiting for the byte to be Rx via the loop-back connection */
				gc10ms_rs485TestInterval = gc10msTick;	/* Mark time of transmission */
			}
		}
	}
}

void rs485testEnable(bool bEnable)
{
	if((gRs485testEnabled == false) && (bEnable == true))
	{	/* starting the loop back test */
		rs485testInit();
		dspOn();
	}
	dspErase();
	
	gRs485testEnabled = bEnable;
}


/*** END of FILE ***/
