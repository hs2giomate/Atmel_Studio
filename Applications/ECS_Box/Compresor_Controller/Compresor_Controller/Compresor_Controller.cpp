/* 
* Compresor_Controller.cpp
*
* Created: 12/16/2020 12:17:32 PM
* Author: GMateusDP
*/


#include "Compresor_Controller.h"

// default constructor
Compresor_Controller::Compresor_Controller()
{
} //Compresor_Controller

// default destructor
Compresor_Controller::~Compresor_Controller()
{
} //~Compresor_Controller

bool Compresor_Controller::Init(void){
	isOK=InitExpander();
	if (isOK)
	{
		if (InitCANOpen())
		{
			isOK=true;
		} 
		else
		{
		}
	} 
	else
	{
	}
	return isOK;
}
Compresor_Controller ccu;