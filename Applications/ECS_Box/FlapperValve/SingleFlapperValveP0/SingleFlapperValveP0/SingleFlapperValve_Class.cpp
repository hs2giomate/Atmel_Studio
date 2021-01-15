/* 
* FlapperValve_Class.cpp
*
* Created: 7/31/2020 3:45:11 PM
* Author: GMateusDP
*/


#include "SingleFlapperValve_Class.h"
//#include "MCP23017_Class.h"
#include "I2C_Sync_Class.h"
//#include "MemoryManagment_Class.h"
#include "Timer_Class.h"
#include "Parameters.h"

SingleFlapperValve_Class	*ptrSingleFlapperValveClass;
static I2C_Sync_Class		i2cStatic(&I2C_FLAPPER_VALVE);
//static MCP23017_Class staticExpander(&I2C_EXPANDER);

static	MCP23008_Class expandersStatic[FLAPPER_VALVE_QUANTITY][FLAPPER_VALVE_EXPANDERS];

static void	Fv1StatusChanged(void){
	
	ptrSingleFlapperValveClass->fv1StatusChanged=true;

}


static void	RegulatorTimeout(const struct timer_task *const timer_task){
	ptrSingleFlapperValveClass->regulatorTimeout=true;
	asm("nop");
}

SingleFlapperValve_Class::SingleFlapperValve_Class(uint8_t _iD)
{	
	valveID=_iD;
	i2c=&i2cStatic;
	fv1StatusChanged=false;
	ptrSingleFlapperValveClass=this;
	isOK=false;
} //FlapperValve_Class




// default constructor
SingleFlapperValve_Class::SingleFlapperValve_Class()
{
	valveID=0;
	i2c=&i2cStatic;
	fv1StatusChanged=false;
	ptrSingleFlapperValveClass=this;
	isOK=false;
} //FlapperValve_Class

// default destructor
SingleFlapperValve_Class::~SingleFlapperValve_Class()
{
} //~FlapperValve_Class


bool	SingleFlapperValve_Class::Init(void){
	if (i2c->i2c_initiated)
	{
	} 
	else
	{
		i2c->Init();
	}
		

		if (i2c->isOK)
		{
		//	ext_irq_register(PIN_PA04,FUNC_PTR(Fv1StatusChanged));
			InitExpanderArray(valveID);
			expanders[0]->SetPortInput();
			expanders[1]->SetPortInput(0x82);
			expanders[2]->SetPortOutput();
			expanders[3]->SetPortInput();
							
		}
		isOK=i2c->isOK;
		
		return isOK;


}

bool	SingleFlapperValve_Class::Init(uint8_t valve_id){
	valveID=valve_id;
	return Init();
}
uint8_t SingleFlapperValve_Class::InitExpanderArray(uint8_t fvID){
	
	for (int i = 0; i < FLAPPER_VALVE_EXPANDERS; i++)
	{
		currentExpander=&(expandersStatic[valveID][i]);
		
		currentExpander->Init(MCP23008_ADDRESS+i+(fvID)*FLAPPER_VALVE_EXPANDERS,i2c);
		expanders[i]=currentExpander;
	}
	return 0;
}

ControlStatus SingleFlapperValve_Class::ReadControlStatus(void){
	value=expanders[1]->ReadGPIORegister();
	controlOutputs=FillControlOutputs(value);
	return controlOutputs;
}
InputStatus SingleFlapperValve_Class::ReadStatusInputs(void){
	value=expanders[0]->ReadGPIORegister();
	statusInputs=FillInputStatus(value);
	return statusInputs;
}
uint8_t SingleFlapperValve_Class::WriteControlStatus(uint8_t controlByte){
	value=expanders[1]->WriteGPIORegister(controlByte);
	return value;
}

uint8_t SingleFlapperValve_Class::SetEnable(bool b){
	value=expanders[1]->ReadGPIORegister();
	value=b?value|0x01:value&0xfe;
	controlOutputs.niAlcFvMotorEnable=b;
	value=expanders[1]->WriteGPIORegister(value);
	return value;
}
uint8_t SingleFlapperValve_Class::SetDirection(bool b){
	value=expanders[1]->ReadGPIORegister();
	value=b?value|0x20:value&0xdf;
	value=expanders[1]->WriteGPIORegister(value);
	return value;
}
uint8_t SingleFlapperValve_Class::ClearMoveFault(bool b){
	value=expanders[1]->ReadGPIORegister();
	value=b?value|0x40:value&0xbf;
	value=expanders[1]->WriteGPIORegister(value);
	return value;
}

uint8_t SingleFlapperValve_Class::SetInvalidPosition(bool b){
	value=expanders[1]->ReadGPIORegister();
	value=b?value|0x04:value&0xfb;
	value=expanders[1]->WriteGPIORegister(value);
	return value;
}

uint8_t SingleFlapperValve_Class::WriteSetpoint(uint8_t sp){
	setpointPosition= expanders[2]->WriteGPIORegister(sp);
//	setpointPosition= simpleExpander->WriteRegisterB(sp);
	return setpointPosition;
}
uint8_t SingleFlapperValve_Class::SetOffsetPosition(void){
		WriteSetpoint(offset);
		regulatorTimeout=false;
		hvacTimer.Start_oneShot_task(FUNC_PTR(RegulatorTimeout),1000*60*1);
		while ((!regulatorTimeout)&&(abs(ReadActualPosition()-setpointPosition)>3)){
		//	arinc.SaveFlapperValveAngle();
			delay_ms(50);
		//	arinc.TransmitTXBuffer();
			
		}
		hvacTimer.Remove_task(FUNC_PTR(RegulatorTimeout));
		return actualPosition;
		
}
uint8_t SingleFlapperValve_Class::SetVentilatePosition(void){
	WriteSetpoint(0xff-offset);
	regulatorTimeout=false;
	hvacTimer.Start_oneShot_task(FUNC_PTR(RegulatorTimeout),1000*60);
	while ((!regulatorTimeout)&&(abs(ReadActualPosition()-setpointPosition)>3)){
		//	arinc.SaveFlapperValveAngle();
			delay_ms(50);
		//	arinc.TransmitTXBuffer();
	}
	hvacTimer.Remove_task(FUNC_PTR(RegulatorTimeout));
	return actualPosition;
	
}
uint8_t SingleFlapperValve_Class::ReadSetpoint(){
	setpointPosition= expanders[2]->ReadGPIORegister();
	return setpointPosition;
}
uint8_t SingleFlapperValve_Class::GetCurrentAngle(void){
	return actualPosition;
}
uint8_t SingleFlapperValve_Class::ReadActualPosition(void){

	lastPosition=actualPosition;
//	actualPosition= simpleExpander->ReadRegister(MCP23017_GPIOA);
	actualPosition= expanders[3]->ReadGPIORegister();
	if (actualPosition>FLAPPER_VALVE_MINIMUM_AIR)
	{
		statusFlapperValve=NBC_MODE;
	}else if (actualPosition>FLAPPER_VALVE_MINIMUM_AIR/2)
	{
		statusFlapperValve=RECYCLE_MODE;
	}else if (actualPosition>FLAPPER_VALVE_MINIMUM_AIR/4)
	{
		statusFlapperValve=INTERM_MODE;
	} 
	else
	{
		statusFlapperValve=FRESHAIR_MODE;
	}
	return actualPosition;
}
void SingleFlapperValve_Class::UpdateFlapperValveData(){
	fvData.controlOutputs=ReadControlStatus();
	fvData.inputStatus=ReadStatusInputs();
	fvData.actualPosition=ReadActualPosition();
}
bool SingleFlapperValve_Class::IsPositionChanged(void){
	if (lastPosition==ReadActualPosition())
	{
		return false;
	} 
	else
	{
		return true;
	}
	return true;
}
bool SingleFlapperValve_Class::Selftest(void){

	SetOffsetPosition();
	//UpdateFlapperValveData();
	isOK=abs(ReadActualPosition()-setpointPosition)<4;
	//return isOK;
	return true;
}

SingleFlapperValve_Class::operator bool(){
	return	isOK;
}

 //SingleFlapperValve_Class	fv1;