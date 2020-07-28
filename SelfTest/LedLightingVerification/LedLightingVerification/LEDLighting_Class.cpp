/* 
* LEDMapping_Class.cpp
*
* Created: 7/15/2020 1:05:39 PM
* Author: GMateusDP
*/


#include "LEDLighting_Class.h"




// default constructor
LEDLighting_Class::LEDLighting_Class()
{
		totalPins=sizeof(PinMappingLeft)+sizeof(PinMappingRight);
		

} //LEDMapping_Class

// default destructor
LEDLighting_Class::~LEDLighting_Class()
{
} //~LEDMapping_Class

void	LEDLighting_Class::InitMapping(void){
	uint8_t	portsleftleft[]={
		0,
		
		0,
		
		0,
		
		1,
		
		1,
		
		0,
		
		0,
		
		0,
		
		0,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		1,
		
		0,
		
		0,
		
		0,
		
		0,
		
		1,
		
		1,
		
		0,
		
		0,
		
		
	};
	for (i = 0; i <(int)sizeof(portsleftleft); i++)
	{
		leftPins.portsLeft[i]=portsleftleft[i];
	}
	

	uint8_t	 pinsLeftLeft[]={
		22,
		
		21,
		
		19,
		
		17,
		
		16,
		
		17,
		
		16,
		
		13,
		
		12,
		
		15,
		
		14,
		
		13,
		
		12,
		
		7,
		
		6,
		
		5,
		
		4,
		
		3,
		
		2,

		1,
		
		0,
		
		7,
		
		6,
		
		5,
		
		4,
		
		9,
		
		8,
		
		3,
		
		2,

	};
	for (i = 0; i < (int)sizeof(pinsLeftLeft); i++)
	{
		leftPins.pinsLeft[i]=pinsLeftLeft[i];
	}
uint8_t	portsRightRight[]={
		0,

		0,

		0,

		0,

		0,

		0,

		0,

		0,

		0,

		0,

		1,

		1,

		0,

		0,

		1,

		1,

		1,

		1,

		0,

		0,

		0,

		0,

		1,

		1,

		1,

		1,

		0,

		0,

		0,

	};
		for (i = 0; i < (int)sizeof(portsRightRight); i++)
		{
			rightPins.portsRight[i]=portsRightRight[i];
		}

uint8_t	pinsRightRight[]={
		25,
		
		24,
		
		23,
		
		03,
		
		19,
		
		18,
		
		15,
		
		14,
		
		21,
		
		20,
		
		11,
		
		10,
		
		23,
		
		22,
		
		15,
		
		14,
		
		13,
		
		12,
		
		19,
		
		18,
		
		17,
		
		16,
		
		23,
		
		22,
		
		31,
		
		30,
		
		8,
		
		31,
		
		30,
	};
			for (i = 0; i <(int)sizeof(pinsRightRight); i++)
			{
				rightPins.pinsRight[i]=pinsRightRight[i];
			}
		totalPins=kTOTALLEFTPINS+kTOTALRIGHTPINS;	
}

void	LEDLighting_Class::SetPin(uint8_t	pt,uint8_t pn){
	gpio_set_pin_level(GPIO(pt,pn),true);
	
}
void	LEDLighting_Class::ClearPin(uint8_t	pt,uint8_t pn){
	gpio_set_pin_level(GPIO(pt,pn),false);
	
}
void LEDLighting_Class::DefineColors(StateLeds&	st){
	
	j=0;
	for (i = 0; i < totalPins; i++){
		if (i<kTOTALLEFTPINS)
		{
			st.color[i]=i%3;
		} 
		else
		{
			st.color[i]=2-((j+1)%3);
			j++;
			
		}
		
	}
	localState=st;
	
}
void	LEDLighting_Class::SetBothSidesPins(BothSidePins& bp){
	pins=bp;
	ConfiguratePins();
}

void	LEDLighting_Class::ConfiguratePins(void){
	for (i = 0; i < totalPins; i++)
	{
		gpio_set_pin_direction(GPIO(pins.allPorts[i],pins.allPins[i]), GPIO_DIRECTION_OUT);
		gpio_set_pin_function(GPIO(pins.allPorts[i],pins.allPins[i]), GPIO_PIN_FUNCTION_OFF);
		gpio_set_pin_level(GPIO(pins.allPorts[i],pins.allPins[i]),false);
	}
}
bool	LEDLighting_Class::IsGreen(uint8_t l){
	return	localState.color[l]==0;
}
bool	LEDLighting_Class::IsRed(uint8_t l){
	return	localState.color[l]==1;
}
bool	LEDLighting_Class::IsBlue(uint8_t l){
	return	localState.color[l]==2;
}




