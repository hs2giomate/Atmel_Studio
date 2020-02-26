/*--------------------------------------------------------------------------

corePorts.c

This file is part of e.Core

Implementation
Atmel SAM ports

$Author: steffen $
$Date: 2017-06-07 11:15:35 +0200 (Mi, 07 Jun 2017) $
$Revision: 1481 $
$HeadURL: https://svn.s2embedded.at/customers/hs2-engineering/trunk/HiLo/Firmware/Libraries/Cortex/e.Core/corePorts.c $

Copyright (c) 2006,2007,2008,2009, 2010, 2014 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#include "sam.h"
#include "corePorts.h"
//#include "coreInterfaceDescription.h"

#define kNumberOfIOPins		2
#define kNumberOfMuxPins	2

#define pinSystemLED	{kPortA, kPortPin27, kOutputHi}
#define pinReedSensor	{kPortB, kPortPin3, kInputPullUp}

#define pinSercom2RX	{kPortA, kPortPin14, kFunctionD}
#define pinSercom2TX	{kPortA, kPortPin15, kFunctionD}

const uint16	eCoreNumberOfMuxPins = kNumberOfMuxPins;
const uint16	eCoreNumberOfIOPins = kNumberOfIOPins;

controllerIOPin ioPins[kNumberOfIOPins] =
	{
	pinSystemLED,
	pinReedSensor
	};

controllerMuxPin muxPins[kNumberOfMuxPins] =
	{
	pinSercom2RX,
	pinSercom2TX
	};

#if defined (__SAMD21G18A__)||defined (__SAMD21J18A__)
volatile uint32*	DIRValue[kNumberOfPorts] = {&PORT->Group[0].DIR.reg ,&PORT->Group[1].DIR.reg};
const volatile uint32*	PINValue[kNumberOfPorts] = {&PORT->Group[0].IN.reg ,&PORT->Group[1].IN.reg};
volatile uint32*	SETValue[kNumberOfPorts] = {&PORT->Group[0].OUTSET.reg ,&PORT->Group[1].OUTSET.reg};
volatile uint32*	CLEARValue[kNumberOfPorts] = {&PORT->Group[0].OUTCLR.reg ,&PORT->Group[1].OUTCLR.reg};
volatile uint32*	TOGGLEValue[kNumberOfPorts] = {&PORT->Group[0].OUTTGL.reg ,&PORT->Group[1].OUTTGL.reg};
#else
#error "Undefined device"
#endif

#define DIR(x)		(*DIRValue[x])
#define PIN(x)		(*PINValue[x])
#define SET(x)		(*SETValue[x])
#define CLEAR(x)	(*CLEARValue[x])
#define TOGGLE(x)	(*TOGGLEValue[x])

#if (SMC_Revision == 1)
void corePortsInitialize(void)
	{
	//	Cortex internal
	//	Flash /CS PA27
	PORT->Group[0].OUTSET.reg = (1UL << 27);
	PORT->Group[0].DIRSET.reg = (1UL << 27);
	//	FRAM /CS PA28
	PORT->Group[0].OUTSET.reg = (1UL << 28);
	PORT->Group[0].DIRSET.reg = (1UL << 28);
/*
	TEMP_SENS_LC	Analogue 	PA02/AIN0/DAC/Y0
	PS1_LC			Analogue 	PA03/AIN1/Y1/REFA/REF_DAC/USB_ID
	PS2_LC			Analogue 	PB08/AIN2/Y14
	DIST_MEAS_LC	Analogue 	PB09/AIN3/Y15

	VREF			Analogue 	PA04/AIN4/Y2/REFB

	ICM_U_POTI		Analogue	PA05/AIN5/Y3
*/
	PORT->Group[0].PINCFG[2].reg = PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[3].reg = PORT_PINCFG_INEN;
	PORT->Group[1].PINCFG[8].reg = PORT_PINCFG_INEN;
	PORT->Group[1].PINCFG[9].reg = PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[4].reg = PORT_PINCFG_INEN;
	//#define kBlockValveDriver  GPIO(GPIO_PORTB, 0)
//	BV_DRV_LC		Digital Out	PB00/AIN8/Y6
	PORT->Group[1].OUTCLR.reg = (1UL << 0);
	PORT->Group[1].DIRSET.reg = (1UL << 0);
		PORT->Group[1].OUTCLR.reg = (1UL << 0);
		PORT->Group[1].DIRSET.reg = (1UL << 0);
	
//	HOME_SWITCH_LC		Digital In	PB01/AIN9/Y7
	PORT->Group[1].PINCFG[1].reg = PORT_PINCFG_INEN;
//	SPARE_SWITCH_1_LC	Digital In	PB02/AIN10
	PORT->Group[1].PINCFG[2].reg = PORT_PINCFG_INEN;
//	SPARE_SWITCH_2_LC	Digital In	PB03/AIN11
	PORT->Group[1].PINCFG[3].reg = PORT_PINCFG_INEN;
//	SPARE_SWITCH_3_LC	Digital In	PB04/AIN12
	PORT->Group[1].PINCFG[4].reg = PORT_PINCFG_INEN;
//	ROCKER_SWITCH_UP_LC	Digital In	PB05/AIN13
	PORT->Group[1].PINCFG[5].reg = PORT_PINCFG_INEN;
//	ROCKER_SWITCH_DOWN_LC	Digital In	PB06/AIN14
	PORT->Group[1].PINCFG[6].reg = PORT_PINCFG_INEN;

//	EN_+28V_1			Digital Out	PB07/AIN15/Y13
	PORT->Group[1].OUTCLR.reg = (1UL << 7);
	PORT->Group[1].DIRSET.reg = (1UL << 7);
//	STATUS_+28V_1		Digital In	PB12/SPI_SC4_0/TCC0_WO6/X12_1
	PORT->Group[1].PINCFG[12].reg = PORT_PINCFG_INEN;
//	EN_+28V_2			Digital Out	PB13/SPI_SC4_1/TCC0_WO7/X13_1
	PORT->Group[1].OUTCLR.reg = (1UL << 13);
	PORT->Group[1].DIRSET.reg = (1UL << 13);
//	STATUS_+28V_2		Digital In	PB14/SPI_SC4_2/TC5_WO0/X14_1
//	PORT->Group[1].PINCFG[14].reg = PORT_PINCFG_INEN;

//	SYNC_SDA_LC			I2C		PB16/I2C_SC5_0
//	SYNC_SCL_LC			I2C		PB17/I2C_SC5_1

//	ICM_READY_LC		Digital In	PB30/TCC0_WO0
	PORT->Group[1].PINCFG[30].reg = PORT_PINCFG_INEN;
//	ICM_DIGIN1_LC		Digital Out	PB31/TCC0_WO1
	PORT->Group[1].OUTCLR.reg = (1UL << 31);
	PORT->Group[1].DIRSET.reg = (1UL << 31);
//	ICM_DIGIN2_LC		Digital Out	PB22/TC7_WO0
	PORT->Group[1].OUTCLR.reg = (1UL << 22);
	PORT->Group[1].DIRSET.reg = (1UL << 22);
//	ICM_ENABLE_LC		Digital Out	PB23/TC7_WO1
	PORT->Group[1].OUTCLR.reg = (1UL << 23);
	PORT->Group[1].DIRSET.reg = (1UL << 23);

//	ICM_DIR_LC			Digital Out	PA16/I2C_SC1_0/TCC2_WO0/X4_2
	PORT->Group[0].OUTCLR.reg = (1UL << 16);
	PORT->Group[0].DIRSET.reg = (1UL << 16);
//	ICM_MONITOR_N_LC	Digital In	PA17/I2C_SC1_1/TCC2_WO1/X5_2
	PORT->Group[0].PINCFG[7].reg = PORT_PINCFG_INEN;
//	ICM_SPEED_PWM		Digital Out	PA18/UART_SC3_2/TCC0_WO2/X6_1
	PORT->Group[0].OUTCLR.reg = (1UL << 18);
	PORT->Group[0].DIRSET.reg = (1UL << 18);

//	ICM_POT_CS			Digital Out	PB13/SPI_SC4_1/TCC0_WO7/X13_2
	PORT->Group[1].OUTSET.reg = (1UL << 13);
	PORT->Group[1].DIRSET.reg = (1UL << 13);
//	ICM_POT_U/D			Digital Out	PB12/SPI_SC4_0/TCC0_WO6/X12_2
	PORT->Group[1].OUTCLR.reg = (1UL << 12);
	PORT->Group[1].DIRSET.reg = (1UL << 12);
//	ICM_EN_MOT_UC		Digital In	PB14/SPI_SC4_2/TC5_WO0/X14_2
	PORT->Group[1].PINCFG[14].reg = PORT_PINCFG_INEN;

//	USB_VBUS_LC			Digital In	PA03
//	PORT->Group[0].PINCFG[3].reg = PORT_PINCFG_INEN;
//	USB_DM_LC			USB		PA24/TX/USB_DM
//	USB_DP_LC			USB		PA25/RX/USB_DP	PORT->Group[0].OUTCLR.reg = 0xFFFFFFFF;
	}
#elif (SMC_Revision == 2)
void corePortsInitialize(void)
	{
	//	Cortex internal
	//	Flash /CS PA27
	PORT->Group[0].OUTSET.reg = (1UL << 27);
	PORT->Group[0].DIRSET.reg = (1UL << 27);
	//	FRAM /CS PA28
	PORT->Group[0].OUTSET.reg = (1UL << 28);
	PORT->Group[0].DIRSET.reg = (1UL << 28);
	
	//	OnBoard LED
	PORT->Group[1].OUTSET.reg = (1UL << 15);
	PORT->Group[1].DIRSET.reg = (1UL << 15);

/*
		Analogue 	PA02/AIN0/DAC/Y0
	TEMP_SENS_LC	Analogue 	PA03/AIN1/Y1/REFA/REF_DAC/USB_ID
	PS2_LC			Analogue 	PB08/AIN2/Y14
	DIST_MEAS_LC	Analogue 	PB09/AIN3/Y15

	PS1_LC			Analogue 	PA04/AIN4/Y2/REFB
	ICM_U_POTI		Analogue	PA05/AIN5/Y3
	TEMP_REV		Analogue	PA06/AIN6/Y3
*/
	PORT->Group[0].PINCFG[3].reg = PORT_PINCFG_INEN;
	PORT->Group[1].PINCFG[8].reg = PORT_PINCFG_INEN;
	PORT->Group[1].PINCFG[9].reg = PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[4].reg = PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[5].reg = PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[6].reg = PORT_PINCFG_INEN;
	//#define kBlockValveDriver  GPIO(GPIO_PORTB, 0)
//	BV_DRV_LC		Digital Out	PB00/AIN8/Y6
	PORT->Group[1].OUTCLR.reg = (1UL << 0);
	PORT->Group[1].DIRSET.reg = (1UL << 0);
	PORT->Group[1].OUTSET.reg = (1UL << 0);
	
//	HOME_SWITCH_LC		Digital In	PB01/AIN9/Y7
#ifdef OFFLINE
	PORT->Group[1].PINCFG[1].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	PORT->Group[1].OUTSET.reg = (1UL << 1);
#else
	PORT->Group[1].PINCFG[1].reg = PORT_PINCFG_INEN;
#endif

//	SPARE_SWITCH_1_LC	Digital In	PB02/AIN10
	PORT->Group[1].PINCFG[2].reg = PORT_PINCFG_INEN;
//	SPARE_SWITCH_2_LC	Digital In	PB03/AIN11
	PORT->Group[1].PINCFG[3].reg = PORT_PINCFG_INEN;
//	SPARE_SWITCH_3_LC	Digital In	PB04/AIN12
	PORT->Group[1].PINCFG[4].reg = PORT_PINCFG_INEN;
//	ROCKER_SWITCH_UP_LC	Digital In	PB05/AIN13
#ifdef OFFLINE
	PORT->Group[1].PINCFG[5].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	PORT->Group[1].OUTSET.reg = (1UL << 5);
#else
	PORT->Group[1].PINCFG[5].reg = PORT_PINCFG_INEN;
#endif
//	ROCKER_SWITCH_DOWN_LC	Digital In	PB06/AIN14
#ifdef OFFLINE
	PORT->Group[1].PINCFG[6].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	PORT->Group[1].OUTSET.reg = (1UL << 6);
#else
	PORT->Group[1].PINCFG[6].reg = PORT_PINCFG_INEN;
#endif

//	EN_+28V_1			Digital Out	PB07/AIN15/Y13
	PORT->Group[1].OUTCLR.reg = (1UL << 7);
	PORT->Group[1].DIRSET.reg = (1UL << 7);
//	STATUS_+28V_1		Digital In	PA19/UART_SC3_3/I2S_SD0/TCC0_WO3/X7_1
	PORT->Group[0].PINCFG[19].reg = PORT_PINCFG_INEN;

//	BUG! *****
//	EN_+28V_2			Digital Out	PB13/SPI_SC4_1/TCC0_WO7/X13_1
//	PORT->Group[1].OUTCLR.reg = (1UL << 13);
//	PORT->Group[1].DIRSET.reg = (1UL << 13);
//	STATUS_+28V_2		Digital In	PB12/SPI_SC4_0/TCC0_WO6/X12_1
//	PORT->Group[1].PINCFG[12].reg = PORT_PINCFG_INEN;

//	SYNC_SDA_LC			I2C		PB16/I2C_SC5_0
//	SYNC_SCL_LC			I2C		PB17/I2C_SC5_1
//	ICM_READY_LC		Digital In	PB30/TCC0_WO0
	PORT->Group[1].PINCFG[30].reg = PORT_PINCFG_INEN;
//	ICM_DIGIN1_LC		Digital Out	PB31/TCC0_WO1
	PORT->Group[1].OUTCLR.reg = (1UL << 31);
	PORT->Group[1].DIRSET.reg = (1UL << 31);
//	ICM_DIGIN2_LC		Digital Out	PB22/TC7_WO0
	PORT->Group[1].OUTCLR.reg = (1UL << 22);
	PORT->Group[1].DIRSET.reg = (1UL << 22);
//	ICM_ENABLE_LC		Digital Out	PB23/TC7_WO1
	PORT->Group[1].OUTCLR.reg = (1UL << 23);
	PORT->Group[1].DIRSET.reg = (1UL << 23);

//	ICM_DIR_LC			Digital Out	PA16/I2C_SC1_0/TCC2_WO0/X4_2
	PORT->Group[0].OUTCLR.reg = (1UL << 16);
	PORT->Group[0].DIRSET.reg = (1UL << 16);
//	ICM_MONITOR_N_LC	Digital In	PA17/I2C_SC1_1/TCC2_WO1/X5_2
	PORT->Group[0].PINCFG[7].reg = PORT_PINCFG_INEN;
//	ICM_SPEED_PWM		Digital Out	PA18/UART_SC3_2/TCC0_WO2/X6_1
//	PORT->Group[0].OUTCLR.reg = (1UL << 18);
//	PORT->Group[0].DIRSET.reg = (1UL << 18);

//	ICM_POT_U/D			Digital Out	PB12/SPI_SC4_0/TCC0_WO6/X12_2
	PORT->Group[1].OUTCLR.reg = kPortBit12;
	PORT->Group[1].DIRSET.reg = kPortBit12;
//	ICM_POT_CS			Digital Out	PB13/SPI_SC4_1/TCC0_WO7/X13_2
	PORT->Group[1].OUTCLR.reg = kPortBit13;
	PORT->Group[1].DIRSET.reg = kPortBit13;
//	ICM_EN_MOT_UC		Digital In	PB14/SPI_SC4_2/TC5_WO0/X14_2
	PORT->Group[1].PINCFG[14].reg = PORT_PINCFG_INEN;

//	USB_VBUS_LC			Digital In	PA14/UART_SC2_2
	PORT->Group[0].PINCFG[14].reg = PORT_PINCFG_INEN;
//	USB_DM_LC			USB		PA24/TX/USB_DM
//	USB_DP_LC			USB		PA25/RX/USB_DP	
	
//	PORT->Group[0].OUTCLR.reg = 0xFFFFFFFF;
	}
#elif (SMC_Revision == 3)
void corePortsInitialize(void)
	{
	//	Cortex internal
	//	Flash /CS PA27
	PORT->Group[0].OUTSET.reg = (1UL << 27);
	PORT->Group[0].DIRSET.reg = (1UL << 27);
	//	FRAM /CS PA28
	PORT->Group[0].OUTSET.reg = (1UL << 28);
	PORT->Group[0].DIRSET.reg = (1UL << 28);
	
	//	OnBoard LED
	PORT->Group[1].OUTSET.reg = (1UL << 15);
	PORT->Group[1].DIRSET.reg = (1UL << 15);
	PORT->Group[1].OUTSET.reg = (1UL << 0);
	PORT->Group[1].DIRSET.reg = (1UL << 0);
	
	PORT->Group[0].OUTCLR.reg = (1UL << 4);
	PORT->Group[1].OUTSET.reg = (1UL << 9);
	PORT->Group[1].OUTSET.reg = (1UL << 8);
	PORT->Group[0].DIRSET.reg = (1UL << 4);
	PORT->Group[1].DIRSET.reg = (1UL << 9);
	PORT->Group[1].DIRSET.reg = (1UL << 8);
		PORT->Group[0].OUTCLR.reg = (1UL << 7);
		PORT->Group[0].DIRSET.reg = (1UL << 7);

/*
		Analogue 	PA02/AIN0/DAC/Y0
	PS2_LC			Analogue 	PB08/AIN2/Y14
	DIST_MEAS_LC	Analogue 	PB09/AIN3/Y15

	PS1_LC			Analogue 	PA04/AIN4
	ICM_U_POTI		Analogue	PA05/AIN5
	TEMP_REV		Analogue	PA06/AIN6
	TEMP_SENS_LC	Analogue 	PA07/AIN7
*/
	//PORT->Group[1].PINCFG[8].reg = PORT_PINCFG_INEN;
	//PORT->Group[1].PINCFG[9].reg = PORT_PINCFG_INEN;
	//PORT->Group[0].PINCFG[4].reg = PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[8].reg = PORT_PINCFG_INEN|PORT_PINCFG_PULLEN;
	PORT->Group[0].PINCFG[6].reg = PORT_PINCFG_INEN;
	//PORT->Group[0].PINCFG[7].reg = PORT_PINCFG_INEN;
	//#define kBlockValveDriver  GPIO(GPIO_PORTB, 0)
//	BV_DRV_LC		Digital Out	PB00/AIN8/Y6

//	HOME_SWITCH_LC		Digital In	PB01/AIN9/Y7
#ifdef OFFLINE
	PORT->Group[1].PINCFG[1].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	PORT->Group[1].OUTSET.reg = (1UL << 1);
#else
	PORT->Group[1].PINCFG[1].reg = PORT_PINCFG_INEN;
#endif

//	SPARE_SWITCH_1_LC	Digital In	PB02/AIN10
	PORT->Group[1].PINCFG[2].reg = PORT_PINCFG_INEN;
//	SPARE_SWITCH_2_LC	Digital In	PB03/AIN11
	PORT->Group[1].PINCFG[3].reg = PORT_PINCFG_INEN;
//	SPARE_SWITCH_3_LC	Digital In	PB04/AIN12
	PORT->Group[1].PINCFG[4].reg = PORT_PINCFG_INEN;
//	ROCKER_SWITCH_UP_LC	Digital In	PB05/AIN13
#ifdef OFFLINE
	PORT->Group[1].PINCFG[5].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	PORT->Group[1].OUTSET.reg = (1UL << 5);
#else
	PORT->Group[1].PINCFG[5].reg = PORT_PINCFG_INEN;
#endif
//	ROCKER_SWITCH_DOWN_LC	Digital In	PB06/AIN14
#ifdef OFFLINE
	PORT->Group[1].PINCFG[6].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	PORT->Group[1].OUTSET.reg = (1UL << 6);
#else
	PORT->Group[1].PINCFG[6].reg = PORT_PINCFG_INEN;
#endif

//	EN_+28V_1			Digital Out	PB07/AIN15/Y13
	PORT->Group[1].OUTCLR.reg = (1UL << 7);
	PORT->Group[1].DIRSET.reg = (1UL << 7);
//	STATUS_+28V_1		Digital In	PA19/UART_SC3_3/I2S_SD0/TCC0_WO3/X7_1
	PORT->Group[0].PINCFG[19].reg = PORT_PINCFG_INEN;

//	EN_+28V_2			Digital Out	PA20/SERCOM5[2]
	PORT->Group[1].OUTCLR.reg = (1UL << 20);
	PORT->Group[1].DIRSET.reg = (1UL << 20);
//	STATUS_+28V_2		Digital In	PB11/SERCOM4[3]
	PORT->Group[1].PINCFG[11].reg = PORT_PINCFG_INEN;

//	SYNC_SDA_LC			I2C		PB16/I2C_SC5_0
//	SYNC_SCL_LC			I2C		PB17/I2C_SC5_1
//	ICM_READY_LC		Digital In	PB30/TCC0_WO0
	PORT->Group[1].PINCFG[30].reg = PORT_PINCFG_INEN;
//	ICM_DIGIN1_LC		Digital Out	PB31/TCC0_WO1
	PORT->Group[1].OUTCLR.reg = (1UL << 31);
	PORT->Group[1].DIRSET.reg = (1UL << 31);
//	ICM_DIGIN2_LC		Digital Out	PB22/TC7_WO0
	PORT->Group[1].OUTCLR.reg = (1UL << 22);
	PORT->Group[1].DIRSET.reg = (1UL << 22);
//	ICM_ENABLE_LC		Digital Out	PB23/TC7_WO1
	PORT->Group[1].OUTCLR.reg = (1UL << 23);
	PORT->Group[1].DIRSET.reg = (1UL << 23);

//	ICM_DIR_LC			Digital Out	PA16/I2C_SC1_0/TCC2_WO0/X4_2
	PORT->Group[0].OUTCLR.reg = (1UL << 16);
	PORT->Group[0].DIRSET.reg = (1UL << 16);
//	ICM_MONITOR_N_LC	Digital In	PA17/I2C_SC1_1/TCC2_WO1/X5_2
	PORT->Group[0].PINCFG[7].reg = PORT_PINCFG_INEN;
//	ICM_SPEED_PWM		Digital Out	PA18/UART_SC3_2/TCC0_WO2/X6_1
//	PORT->Group[0].OUTCLR.reg = (1UL << 18);
//	PORT->Group[0].DIRSET.reg = (1UL << 18);

//	ICM_POT_U/D			Digital Out	PB12/SPI_SC4_0/TCC0_WO6/X12_2
	PORT->Group[1].OUTCLR.reg = (1UL << 12);
	PORT->Group[1].DIRSET.reg = (1UL << 12);
//	ICM_POT_CS			Digital Out	PB13/SPI_SC4_1/TCC0_WO7/X13_2
	PORT->Group[1].OUTSET.reg = (1UL << 13);
	PORT->Group[1].DIRSET.reg = (1UL << 13);
//	ICM_EN_MOT_UC		Digital In	PB14/SPI_SC4_2/TC5_WO0/X14_2
	PORT->Group[1].PINCFG[14].reg = PORT_PINCFG_INEN;

//	USB_VBUS_LC			Digital In	PA14/UART_SC2_2
	PORT->Group[0].PINCFG[14].reg = PORT_PINCFG_INEN;
//	USB_DM_LC			USB		PA24/TX/USB_DM
//	USB_DP_LC			USB		PA25/RX/USB_DP	PORT->Group[0].OUTCLR.reg = 0xFFFFFFFF;
	}
#else
#error "Undefined SCM revision"
#endif

void _corePortsInitialize(void)
	{
	corePortsConfigureIOPinFunctions(eCoreNumberOfIOPins, ioPins);
	}

void corePortsConfigurePort(controllerPort port, uint32 portMask, uint32 direction, uint32 pullup, uint32 pulldown)
	{
/*
	uint8	i;
	uint32	pinmode0;
	uint32	pinmode1;
	uint32	inputMask;
	uint32	pinmodeMask0;
	uint32	pinmodeMask1;
	
	DIR(port) = (DIR(port) & ~portMask)|(direction & portMask);
	MASK(port) = 0;
	PIN(port) = 0;

    inputMask = ~direction & portMask;

    pinmodeMask0 = 0;
	for (i=0; i<16; i++)
		{
		if (inputMask & (1 << i))
			pinmodeMask0 |= (0x03 << (i << 1));	
		}

    pinmodeMask1 = 0;
	for (i=0; i<16; i++)
		{
		if (inputMask & (1 << (i + 16)))
			pinmodeMask1 |= (0x03 << (i << 1));	
		}

	pinmode0 = PINMODE(port << 1) & ~pinmodeMask0;
	pinmode1 = PINMODE((port << 1) + 1) & ~pinmodeMask1;
*/
/*
PINMODE
00	P0.00 pin has a pull-up resistor enabled.
01	Reserved. This value should not be used.
10	P0.00 pin has neither pull-up nor pull-down.
11	P0.00 has a pull-down resistor enabled.
*/
/*
	for (i=0; i<16; i++)
		{
		if (inputMask & (1 << i))
			{
			uint8	pinModeValue;
			
			if (pullup & (1 << i))
				pinModeValue = 0x00;
			else if (pulldown & (1 << i))
				pinModeValue = 0x03;
			else
				pinModeValue = 0x02;
				
			pinmode0 |= (pinModeValue << (i << 1));
			}	
		}

	for (i=16; i<32; i++)
		{
		if (inputMask & (1 << i))
			{
			uint8	pinModeValue;
			
			if (pullup & (1 << i))
				pinModeValue = 0x00;
			else if (pulldown & (1 << i))
				pinModeValue = 0x03;
			else
				pinModeValue = 0x02;
				
			pinmode1 |= (pinModeValue << ((i - 16) << 1));
			}	
		}

	PINMODE(port << 1) = pinmode0;
	PINMODE((port << 1) + 1) = pinmode1;
*/
	}

void corePortsConfigureIOPinFunction(const controllerIOPin pin)
	{
/*
	uint8	pinselRegister = ((pin.port << 1) + (pin.pin / 16));
	uint8	pinselOffset = ((pin.pin<16)?(pin.pin << 1):((pin.pin-16) << 1));
	uint32	pinsel = PINSEL(pinselRegister);
		
	pinsel &= ~(0x03 << pinselOffset);
	pinsel |= (pin.function << pinselOffset);
		
	PINSEL(pinselRegister) = pinsel;
*/
	}

void corePortsConfigureMUXPinFunction(const controllerMuxPin pin)
	{
	}

void corePortsConfigureIOPinFunctions(uint16 count, const controllerIOPin pins[])
	{
	uint32	inmask[kNumberOfPorts];
	uint32	outmask[kNumberOfPorts];
	uint32	upmask[kNumberOfPorts];
	uint32	pullmask[kNumberOfPorts];
	uint8		i, j;

	for (i=0; i<kNumberOfPorts; i++)
		{
		inmask[i] = 0;
		outmask[i] = 0;
		upmask[i] = 0;
		pullmask[i] = 0;
		}
	
	//	Setup I/Os
	for (i=0; i<count; i++)
		{
		switch (pins[i].function)
			{
			case kOutput:
			case kOutputLo:
			case kOutputHi:
				outmask[pins[i].port] |= (1 << pins[i].pin);
				if (pins[i].function == kOutputHi)
					upmask[pins[i].port] |= (1 << pins[i].pin);
				break;
			
			case kInput:
				inmask[pins[i].port] |= (1 << pins[i].pin);
				break;
				
			case kInputPullUp:
			case kInputPullDown:
				inmask[pins[i].port] |= (1 << pins[i].pin);
				pullmask[pins[i].port] |= (1 << pins[i].pin);
				if (pins[i].function == kInputPullUp)
					upmask[pins[i].port] |= (1 << pins[i].pin);
				break;
				
			default:
				break;
			}
		}

	for (i=0; i<kNumberOfPorts; i++)
		{
		PORT->Group[i].DIRCLR.reg = ~outmask[i];
		PORT->Group[i].DIRSET.reg = outmask[i];

		//	Clear outputs
		PORT->Group[i].OUTCLR.reg = (~upmask[i] & ~(outmask[i]|pullmask[i]));

		//	Set outputs
		PORT->Group[i].OUTSET.reg = (upmask[i] & outmask[i]);
		
		//	Setup WREG
		for (j=0; j<2; j++)
			{
			//	WREG halfwords
			uint16	pinMask;
			uint16	pinPullMask;
			uint32	WRCONFIG;
			
			pinMask = (inmask[i] >> (j << 3)) & 0xFFFF;
			pinPullMask = (pullmask[i] >> (j << 3)) & 0xFFFF;
			
			//	Configure pull down/pull up	inputs
			WRCONFIG = (uint32)(pinMask & pinPullMask);
			if (WRCONFIG)
				{
				WRCONFIG |= (j>0?(1UL << 31):0);	//	HWSEL
				WRCONFIG |= (1UL << 30);			//	WRPINCFG	
				WRCONFIG |= (1UL << 18);			//	PULLEN	
				WRCONFIG |= (1UL << 17);			//	INEN	

				PORT->Group[i].WRCONFIG.reg = WRCONFIG;
				//	Set pullups
				PORT->Group[i].OUTSET.reg = (upmask[i] & pullmask[i]);
				}
			
			//	Configure remaining inputs
			WRCONFIG = (uint32)(pinMask & ~pinPullMask);
			if (WRCONFIG)
				{
				WRCONFIG |= (j>0?(1UL << 31):0);	//	HWSEL
				WRCONFIG |= (1UL << 30);			//	WRPINCFG	
				WRCONFIG |= (1UL << 17);			//	INEN	

				PORT->Group[i].WRCONFIG.reg = WRCONFIG;
				}
			}
		}
	}

void corePortsConfigureMUXPinFunctions(uint16 count, const controllerMuxPin pins[])
	{
	}

/** Get I/O port.
*/
uint32 corePortsGetPort(controllerPort port)
	{
	return PIN(port);
	}

uint32 corePortsGetDirection(controllerPort port)
	{
	return DIR(port);
	}

uint32 corePortsGetPortMasked(controllerPort port, uint32 mask)
	{
	return PIN(port) & mask;
	}

/** Set I/O port.
*/
void corePortsSetPort(controllerPort port, uint32 value)
	{
	SET(port) = value;
	}

void corePortsSetPortMasked(controllerPort port, uint32 mask, uint32 value)
	{
	SET(port) = (value & mask);
	}

void corePortsORPort(controllerPort port, uint32 value)
	{
	SET(port) = PIN(port) | value;
	}

void corePortsORPortMasked(controllerPort port, uint32 mask, uint32 value)
	{
	SET(port) = PIN(port) | (value & mask);
	}

void corePortsANDPort(controllerPort port, uint32 value)
	{
	SET(port) = PIN(port) & value;
	}

void corePortsANDPortMasked(controllerPort port, uint32 mask, uint32 value)
	{
	SET(port) = PIN(port) & value & mask;
	}

/** Logical XOR I/O port.
*/
void corePortsXORPort(controllerPort port, uint32 value)
	{
	SET(port) = PIN(port) ^ value;
	}

/** Logical XOR I/O port masked.
*/
void corePortsXORPortMasked(controllerPort port, uint32 mask, uint32 value)
	{
	SET(port) = PIN(port) ^ (value & mask);
	}

/** Get one bit.
*/
bool corePortsGetPortPin(controllerPort port, controllerPortPin pin)
	{
	bool	result;

	result = (pin < kNumberOfPortPins) && (PIN(port) & (1UL << pin)) != 0;
	return result;
	}

void corePortsSetPortPin(controllerPort port, controllerPortPin pin)
	{
	if (pin < kNumberOfPortPins)
		SET(port) = (1UL << pin);
	}

void corePortsClearPortPin(controllerPort port, controllerPortPin pin)
	{
	if (pin < kNumberOfPortPins)
		CLEAR(port) = (1UL << pin);
	}

void corePortsTogglePortPin(controllerPort port, controllerPortPin pin)
	{
	if (pin < kNumberOfPortPins)
		TOGGLE(port) = (1UL << pin);
	}

/** Get one bit.
*/
bool corePortsGetPortBit(controllerPort port, controllerPortBit bit)
	{
	bool	result;

	result = (PIN(port) & bit) != 0;
	return result;
	}

void corePortsSetPortBit(controllerPort port, controllerPortBit bit)
	{
	SET(port) = bit;
	}

void corePortsClearPortBit(controllerPort port, controllerPortBit bit)
	{
	CLEAR(port) = bit;
	}

void corePortsTogglePortBit(controllerPort port, controllerPortBit bit)
	{
	if (PIN(port) & bit)
		CLEAR(port) = bit;
	else
		SET(port) = bit;
	}

