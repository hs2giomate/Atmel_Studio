/* 
* Error_Labelling_Class.cpp
*
* Created: 7/9/2020 5:18:12 PM
* Author: GMateusDP
*/


#include "Error_Labelling_Class.h"

const char* faultDescription[] =
{
	"E- 0801-CCuCAN-X2-CCU",
	"E- 0802-CCuCAN-X2-CCU",
	"E- 0803-CCuCAN-X2-CCU",
	"E- 0804-CCuCAN-X2-CCU",
	"E- 0805-CCuCAN-X2-CCU",
	"E- 0806-CCuCAN-X2-CCU",
	"E- 0807-CCuCAN-X2-CCU",
	"W- 0808-CCuCAN-X2-CCU",
	"E- 0101-Tc1-X3-AT Sensor",
	"E- 0102-Tc2-X3-AT Sensor",
	"E- 0103-Tev1-X3-EVAP1",
	"E- 0104-Tev2-X3-EVAP2",
	"E- 0105-Ta1-X3-EVAP1",
	"E- 0106-Ta2-X3-EVAP2",
	"E- 0107-Theat1-X3-AT Sensor",
	"E- 0108-Theat2-X3-AT Sensor",
	"E- 0211-Heater-X2-Heater1",
	"E- 0212-ECS-X2-Heater1",
	"E- 0213-ECS-X2-Heater1",
	"E- 0214-ECS-X2-Heater1",
	"E- 0215-ECS-X2-Heater1",
	"W- 0216-ECS-X2-Heater1",
	"E- 0221-Heater-X2-Heater2",
	"E- 0222-ECS-X2-Heater2",
	"E- 0223-ECS-X2-Heater2",
	"E- 0224-ECS-X2-Heater2",
	"E- 0225-ECS-X2-Heater2",
	"W- 0226-ECS-X2-Heater2",
	"E- 0311-FAN-X2-Evap Fan1",
	"E- 0312-ECS-X2-Evap Fan1",
	"E- 0313-ECS-X2-Evap Fan1",
	"W- 0314-ECS-X2-Evap Fan1",
	"E- 0321-FAN-X2-Evap Fan2",
	"E- 0322-ECS-X2-Evap Fan2",
	"E- 0323-ECS-X2-Evap Fan2",
	"W- 0324-ECS-X2-Evap Fan2",
	"E- 0331-FAN-X2-Cds Fan",
	"E- 0332-ECS-X2-Cds Fan",
	"E- 0333-ECS-X2-Cds Fan",
	"W- 0334-ECS-X2-Cds Fan",
	"E- 0401-ECS-X2-Scav Fan",
	"E- 0402-ECS-ECS-Scav Fan",
	"E- 0403-ECS-ECS-Scav Fan",
	"W- 0404-ECS-X2-Scav Fan",
	"E- 0901-Tcomp-X3-Rec Pack",
	"E- 0902-LP-X3-Ref Pipes",
	"E- 0903-HP-X3-Rec Pack",
	"E- 0904-Tcd-X3-Rec Pack",
	"W- 0501-Filter-X2-Scav Fan",
	"W- 0502-ScavFan-X2-Scav Fan",
	"W- 0503-ECS-X2-Scav Fan",
	"E- 0611-ECS-X2-FLAPVLVE1",
	"E- 0612-ECS-X2-FLAPVLVE1",
	"E- 0613-ECS-X2-FLAPVLVE1",
	"E- 0621-ECS-X2-FLAPVLVE2",
	"E- 0622-ECS-X2-FLAPVLVE2",
	"E- 0623-ECS-X2-FLAPVLVE2",
	"E- 0701-ECS-X4-ECS",
	"E- 0702-ECS-X4-Line1",
	"E- 0703-ECS-X4-Line2",
	"E- 0704-ECS-X4-ECS",

};

// default constructor
Error_Labelling_Class::Error_Labelling_Class()
{
} //Error_Labelling_Class

// default destructor
Error_Labelling_Class::~Error_Labelling_Class()
{
} //~Error_Labelling_Class
