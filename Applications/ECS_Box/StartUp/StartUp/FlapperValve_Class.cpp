/* 
* FlapperValve_Class.cpp
*
* Created: 7/31/2020 3:45:11 PM
* Author: GMateusDP
*/


#include "FlapperValve_Class.h"
#include "MCP23017_Class.h"
#include "I2C_Asyn_Class.h"
#include "MemoryManagment_Class.h"
FlapperValve_Class	*ptrFlapperValveClass;
I2C_Asyn_Class		i2cFVs(&I2C_EXPANDER);

static void	Fv1StatusChanged(void){
	
	ptrFlapperValveClass->fv1StatusChanged=true;

}
static void	Fv2StatusChanged(void){
	
	ptrFlapperValveClass->fv2StatusChanged=true;

}

static bool	I2CFlapperValvesInit(void){
	 bool ok= i2cFVs.Init();
	 //ptrFlapperValveClass->isOK=ok;
}

FlapperValve_Class::FlapperValve_Class(uint8_t id)
{	
	valveID=id;
	i2cDescr=&I2C_EXPANDER;
	ptrFlapperValveClass=this;
	isOK=false;
} //FlapperValve_Class

// default constructor
FlapperValve_Class::FlapperValve_Class()
{
	i2cDescr=&I2C_EXPANDER;
	ptrFlapperValveClass=this;
} //FlapperValve_Class

// default destructor
FlapperValve_Class::~FlapperValve_Class()
{
} //~FlapperValve_Class


bool	FlapperValve_Class::Init(void){
	ptrFlapperValveClass=this;
		if (i2cFVs.initiated==false)
		{
				isOK=I2CFlapperValvesInit();
		}
	
		if (i2cFVs.isOK)
		{
			if (valveID==1)
			{
				ext_irq_register(PIN_PA04,FUNC_PTR(Fv1StatusChanged));
			} 
			else if  (valveID==2)
			{
				ext_irq_register(PIN_PA05,FUNC_PTR(Fv2StatusChanged));	
			}
			InitExpanderArray(valveID);
			isOK=Selftest();
		} 
		else
		{
		}
		return isOK;
}
uint8_t FlapperValve_Class::InitExpanderArray(uint8_t fvID){
	
	for (int i = 0; i < FLAPPER_VALVE_EXPANDERS; i++)
	{
		currentExpander=&(expanders[i]);
		currentExpander->Init(i+(fvID-1)*FLAPPER_VALVE_EXPANDERS,&i2cFVs);
	}
}

ControlStatus FlapperValve_Class::ReadControlStatus(void){
	value=expanders[0].ReadGPIORegister();
	controlStatus=FillControlStatus(value);
	return controlStatus;
}
StatusInputs FlapperValve_Class::ReadStatusInputs(void){
	value=expanders[1].ReadGPIORegister();
	statusInputs=FillStatusInputs(value);
	return statusInputs;
}
uint8_t FlapperValve_Class::WriteSetpoint(uint8_t sp){
	setpointPosition= expanders[2].WriteGPIORegister(sp);
	return setpointPosition;
}
uint8_t FlapperValve_Class::ReadSetpoint(){
	setpointPosition= expanders[2].ReadGPIORegister();
	return setpointPosition;
}
uint8_t FlapperValve_Class::ReadActualPosition(void){
	actualPosition= expanders[3].ReadGPIORegister();
	return actualPosition;
}
void FlapperValve_Class::UpdateFlapperValveData(){
	fvData.controlStatus=ReadControlStatus();
	fvData.statusInputs=ReadStatusInputs();
	fvData.actualPosition=ReadActualPosition();
}
bool FlapperValve_Class::Selftest(void){
	UserParameters up;
	memory.ReadFastParameters(up);
//	WriteSetpoint(up.flapperValveOffset);
	//UpdateFlapperValveData();
	//isOK=ReadSetpoint()==up.flapperValveOffset;
	//return isOK;
	return true;
}

FlapperValve_Class::operator bool(){
	return	isOK;
}

 FlapperValve_Class	fv1(1);