/* 
* FlapperValve_Class.cpp
*
* Created: 7/31/2020 3:45:11 PM
* Author: GMateusDP
*/


#include "FlapperValve_Class.h"

// default constructor
FlapperValve_Class::FlapperValve_Class()
{
} //FlapperValve_Class

// default destructor
FlapperValve_Class::~FlapperValve_Class()
{
} //~FlapperValve_Class
bool	FlapperValve_Class::Init(void){
		return	mcp.Init();
}
FlapperValve_Class	fv1;