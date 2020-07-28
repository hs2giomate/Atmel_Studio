/* 
* Error_Labelling_Class.h
*
* Created: 7/9/2020 5:18:13 PM
* Author: GMateusDP
*/


#ifndef __ERROR_LABELLING_CLASS_H__
#define __ERROR_LABELLING_CLASS_H__



typedef	enum{
	kE0801CCuCANX2CCU,
	kE0802CCuCANX2CCU,
	kE0803CCuCANX2CCU,
	kE0804CCuCANX2CCU,
	kE0805CCuCANX2CCU,
	kE0806CCuCANX2CCU,
	kE0807CCuCANX2CCU,
	kW0808CCuCANX2CCU,
	kE0101Tc1X3ATSensor,
	kE0102Tc2X3ATSensor,
	kE0103Tev1X3EVAP1,
	kE0104Tev2X3EVAP2,
	kE0105Ta1X3EVAP1,
	kE0106Ta2X3EVAP2,
	kE0107Theat1X3ATSensor,
	kE0108Theat2X3ATSensor,
	kE0211HeaterX2Heater1,
	kE0212ECSX2Heater1,
	kE0213ECSX2Heater1,
	kE0214ECSX2Heater1,
	kE0215ECSX2Heater1,
	kW0216ECSX2Heater1,
	kE0221HeaterX2Heater2,
	kE0222ECSX2Heater2,
	kE0223ECSX2Heater2,
	kE0224ECSX2Heater2,
	kE0225ECSX2Heater2,
	kW0226ECSX2Heater2,
	kE0311FANX2EvapFan1,
	kE0312ECSX2EvapFan1,
	kE0313ECSX2EvapFan1,
	kW0314ECSX2EvapFan1,
	kE0321FANX2EvapFan2,
	kE0322ECSX2EvapFan2,
	kE0323ECSX2EvapFan2,
	kW0324ECSX2EvapFan2,
	kE0331FANX2CdsFan,
	kE0332ECSX2CdsFan,
	kE0333ECSX2CdsFan,
	kW0334ECSX2CdsFan,
	kE0401ECSX2ScavFan,
	kE0402ECSECSScavFan,
	kE0403ECSECSScavFan,
	kW0404ECSX2ScavFan,
	kE0901TcompX3RecPack,
	kE0902LPX3RefPipes,
	kE0903HPX3RecPack,
	kE0904TcdX3RecPack,
	kW0501FilterX2ScavFan,
	kW0502ScavFanX2ScavFan,
	kW0503ECSX2ScavFan,
	kE0611ECSX2FLAPVLVE1,
	kE0612ECSX2FLAPVLVE1,
	kE0613ECSX2FLAPVLVE1,
	kE0621ECSX2FLAPVLVE2,
	kE0622ECSX2FLAPVLVE2,
	kE0623ECSX2FLAPVLVE2,
	kE0701ECSX4ECS,
	kE0702ECSX4Line1,
	kE0703ECSX4Line2,
	kE0704ECSX4ECS,

	}Fault_List;

typedef	enum{
	kARINCINnterfaceError,
	kpBITError,
	}Internal_Fault_List;
	

class Error_Labelling_Class
{
//variables
public:
protected:
private:

//functions
public:
	Error_Labelling_Class();
	~Error_Labelling_Class();
protected:
private:
	Error_Labelling_Class( const Error_Labelling_Class &c );
	Error_Labelling_Class& operator=( const Error_Labelling_Class &c );

}; //Error_Labelling_Class

#endif //__ERROR_LABELLING_CLASS_H__
