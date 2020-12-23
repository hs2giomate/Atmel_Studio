/* 
* Dual_Flapper_Valve_Controller.h
*
* Created: 12/21/2020 12:38:38 PM
* Author: GMateusDP
*/


#ifndef __DUAL_FLAPPER_VALVE_CONTROLLER_H__
#define __DUAL_FLAPPER_VALVE_CONTROLLER_H__
#include "FlapperValveController.h"



class Dual_Flapper_Valve_Controller: public FlapperValveController
{
//variables
public:
FlapperValveController	*fv1;
FlapperValveController	*fv2;
FlapperValveController	*fvx[FLAPPER_VALVE_QUANTITY];
protected:
private:

	bool  boolResult;
//functions
public:
	Dual_Flapper_Valve_Controller();
	~Dual_Flapper_Valve_Controller();
	bool Init(void);
	bool IsPartnerOK(uint8_t partner_id);
	
protected:
private:
	Dual_Flapper_Valve_Controller( const Dual_Flapper_Valve_Controller &c );
	Dual_Flapper_Valve_Controller& operator=( const Dual_Flapper_Valve_Controller &c );

}; //Dual_Flapper_Valve_Controller

extern Dual_Flapper_Valve_Controller fv;

#endif //__DUAL_FLAPPER_VALVE_CONTROLLER_H__
