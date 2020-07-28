/* 
* LEDMapping_Class.h
*
* Created: 7/15/2020 1:05:40 PM
* Author: GMateusDP
*/


#ifndef __LEDMAPPING_CLASS_H__
#define __LEDMAPPING_CLASS_H__
#include "driver_init.h"
#include "PinMaps.h"


struct BothSidePins
{
	uint8_t	allPorts[kTOTALLEFTPINS+kTOTALRIGHTPINS];
	uint8_t	allPins[kTOTALLEFTPINS+kTOTALRIGHTPINS];
// 	uint8_t	allPorts[16];
// 	uint8_t	allPins[16];

};

typedef	struct BothSidePins	BothSidePins;
struct StateLeds
{
	uint8_t   status[kTOTALLEFTPINS+kTOTALRIGHTPINS];
	uint8_t   color[kTOTALLEFTPINS+kTOTALRIGHTPINS];
	
};
typedef struct	StateLeds	StateLeds;


class LEDLighting_Class
{
//variables
public:
	LeftPins	leftPins;
	RightPins	rightPins;

protected:
private:
	BothSidePins	pins;
	uint8_t		totalPins;
	int i,j,k,m,n;
	StateLeds	localState;

//functions
public:
	LEDLighting_Class();
	~LEDLighting_Class();
	void InitMapping(void);
	void	SetPin(uint8_t, uint8_t);
	void	ClearPin(uint8_t,uint8_t);
	void	SetBothSidesPins(BothSidePins&);
	void	ConfiguratePins(void);
	void	DefineColors(StateLeds&);
	bool	IsGreen(uint8_t);
	bool	IsRed(uint8_t);
	bool	IsBlue(uint8_t);
protected:
private:
	LEDLighting_Class( const LEDLighting_Class &c );
	LEDLighting_Class& operator=( const LEDLighting_Class &c );

}; //LEDMapping_Class

extern	LEDLighting_Class	ledMap;

#endif //__LEDMAPPING_CLASS_H__
