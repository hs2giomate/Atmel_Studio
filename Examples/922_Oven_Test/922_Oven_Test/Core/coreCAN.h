/*--------------------------------------------------------------------------

coreCAN.h

This file is part of e.Development

Interface
low level interface to the CAN controller

$Date: 2017-09-26 17:14:13 +0200 (Di, 26. Sep 2017) $
$Revision: 7606 $

Copyright (c) 2006,2007, 2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#if defined(MGS_ZST_Revision)||defined(IPECO_PSC_Revision)||defined(E_DEVICE_CAN)

#ifndef CORE_CAN_H
#define CORE_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/**
	Constants for on-chip CAN ports
*/
enum
	{
	kCAN0 = 0,	//!	CAN0
	kNumberOfCANS
	};

/**
	CAN message struct
*/
typedef struct
	{
	uint32	identifier;
	uint8	data[8];
	bool	isPending;
	uint8	dataLengthCode;
	uint32	timestamp;
	} CANMessage;
	
/** Initialize CAN subsystem
*/
int8 coreCANInitialize(void);
/** Open CAN port
*/
int8 coreCANOpen(uint8 can, uint32 bitrate);

/** Close CAN port
*/
int8 coreCANClose(uint8 can);

void coreCANReset(uint8 can);

/**
	Get CAN reset counter
*/
uint16 coreCANGetResetCounter(uint8 can);
/*
	Get CAN bus state ON/off
*/
bool coreCANGetBusOffState(uint8 can);
/*
	Reset CAN bus state ON/off
*/
void coreCANResetBusOffState(uint8 can);
/**
	Get CAN transmit error counter
*/
uint8 coreCANGetTEC(uint8 can);

/**
	Get CAN receive error counter
*/
uint8 coreCANGetREC(uint8 can);

/**
	Get CAN bus state
	\returns true if CAN bus state is NOT Bus OFF, false else.
*/
bool coreCANGetBusState(uint8 can);

uint8 coreCANDataAvailable(uint8 can);

uint16 coreCANDataOverflow(uint8 can);

/**
	CAN message filtering
*/
/**
	Set default message buffer filtering
	Default is no filtering
*/
bool coreCANResetMessageFilters(uint8 can);

/**
	Set message buffer filtering on MOb id
*/
bool coreCANAddMessageFilter(uint8 can, uint32 identifier, uint32 mask);

/**
	Read CAN message
*/
bool coreCANReadMessage(uint8 can, CANMessage* message, tick_t timeout);

/**
	Write CAN message
*/
bool coreCANWriteMessage(uint8 can, CANMessage* message);

#ifdef __cplusplus
	}
#endif
#endif

#endif	// CORE_CAN_SUPPORT
