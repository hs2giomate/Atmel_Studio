/* 
* AMMC_Class.cpp
*
* Created: 6/19/2020 4:38:41 PM
* Author: GMateusDP
*/


#include "AMMC_Class.h"

// default constructor
AMMC_Class::AMMC_Class()
{
} //AMMC_Class

// default destructor
AMMC_Class::~AMMC_Class()
{
} //~AMMC_Class

bool	AMMC_Class::IsSelected(void){
	if (selected)
	{
		return true;
	} 
	else
	{
		return false;
	}
	
}

bool	AMMC_Class::GetSelected(void){

		selected= true;
	return	selected;
}

AMMC_Class	ammc1;
AMMC_Class	ammc2;
AMMC_Class	*ammc;