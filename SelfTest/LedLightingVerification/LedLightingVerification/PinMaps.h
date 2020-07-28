/*
 * PinMaps.h
 *
 * Created: 7/15/2020 1:22:59 PM
 *  Author: GMateusDP
 */ 


#ifndef PINMAPS_H_
#define PINMAPS_H_
#include "compiler.h"
	enum PinMappingLeft
{
kPA22R,

kPA21R,

kPA19RB,

kPB17R,

kPB16R,

kPA17R,

kPA16R,

kPA13R,

kPA12R,

kPB15R,

kPB14R,

kPB13R,

kPB12R,

kPB07R,

kPB06R,

kPB05R,

kPB04R,

kPB03R,

kPB02R,

kPB01R,

kPB00R,

kPA07R,

kPA06R,

kPA05R,

kPA04R,

kPB09R,

kPB08R,

kPA03R,

kPA02R,
kTOTALLEFTPINS,

};

	enum PinMappingRight
{
kPA25R,

kPA24R,

kPA23RA,

kPA03RA,

kPA19R,

kPA18RA,

kPA15R,

kPA14R,

kPA21RA,

kPA20R,

kPB11R,

kPB10R,

kPA23R,

kPA22RA,

kPB15RA,

kPB14RA,

kPB13RA,

kPB12RA,

kPA19RC,

kPA18R,

kPA17RA,

kPA16RA,

kPB23R,

kPB22R,

kPB31R,

kPB30R,

kPA08R,

kPA31R,

kPA30R,
kTOTALRIGHTPINS,


};

struct LeftPins
	 {
	uint8_t	portsLeft[kTOTALLEFTPINS];
	uint8_t	pinsLeft[kTOTALLEFTPINS];
	};

typedef struct	LeftPins	LeftPins;

struct RightPins
 {
	uint8_t	portsRight[kTOTALRIGHTPINS];
	uint8_t	pinsRight[kTOTALRIGHTPINS];
};

typedef struct	RightPins	RightPins;

#endif /* PINMAPS_H_ */