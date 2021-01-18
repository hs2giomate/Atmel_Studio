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
FlapperValveController	*valve1;
FlapperValveController	*valve2;
FlapperValveController	*valve[FLAPPER_VALVE_QUANTITY];
protected:
private:
	bool is_stand_alone;
	bool  boolResult;
//functions
public:
	Dual_Flapper_Valve_Controller();
	~Dual_Flapper_Valve_Controller();
	bool Init(void);
	bool IsPartnerOK(uint8_t partner_id);
	bool IsStandAlone(void);
		
	
	
protected:
private:
	Dual_Flapper_Valve_Controller( const Dual_Flapper_Valve_Controller &c );
	Dual_Flapper_Valve_Controller& operator=( const Dual_Flapper_Valve_Controller &c );

}; //Dual_Flapper_Valve_Controller

extern Dual_Flapper_Valve_Controller flapper;

#endif //__DUAL_FLAPPER_VALVE_CONTROLLER_H__
