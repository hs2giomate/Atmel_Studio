/**

* \file Parameters.cpp
*
* \brief Application for the automatic control of the Door Actuator P/N 2400024
*
*	Copyright (c)  2019 hs2-engineering. All rights reserved.
*	
*
User Parameters
*	Created: 10/23/2019 2:41:11 PM
*	Author: Giovanny Mateus
*/ 


#include "Parameters.h"
//#include <math.h>
//#include "applicationClass.h"


const UserParameters	defaultParameters =
{
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	.flapperValveMinimumPosition				=	{0xdf,0xdf},
	.flapperValveStandAloneMinimumPosition				= 	{0xbf,0xbf}
// 	.slowSpeedCurrentLimitStow		=		750,			/**<	Current Limitation mAmp */
// 	.fastSpeedCurrentLimitStow		=		700,			//	Current Limitation mAmp
// 	.parkingSpeedCurrentLimitStow	=		2000,			//	Current Limitation mAmp
// 	.slowSpeedCurrentLimitDeploy	=		750,			//	Current Limitation mAmp
// 	.fastSpeedCurrentLimitDeploy	=		800,			//	Current Limitation mAmp
// 	.parkingSpeedCurrentLimitDeploy =		2000,			//	Current Limitation mAmp
// 	.motorCurrentLimit				=		2270,			//	Current Limitation Amp
// 	.motorCurrentResistorFactor		=		2.2167,		//	Current Limitation Amp
// 	.slowSpeedStow					=		12,			//	Motorvelocity Slow
// 	.fastSpeedStow					=		77,			//	Motorvelocity High
// 	.parkingSpeed					=		6,			//	Motorvelocity High
// 	.slowSpeedDeploy				=		12,			//	Motorvelocity Slow
// 	.fastSpeedDeploy				=		82,			//	Motorvelocity High
// 	.slowSpeedOffsetDeploy			=		140.0,		//	Distance from deploy position for entering slow speed
// 	.slowSpeedOffsetStow			=		130.0,		//	Distance from deploy position for entering slow speed
// 	.distanceCalibrationValue		=	0.24885,		//	Calibration, Motorsignals vs. Distance
// 	.kDeployPosition24inch			=		530.0,		//	Distance from deploy position for entering slow speed
// 	.kDeployPosition22inch			=		530.0,		//	Calibration, Motorsignals vs. Distance
// 	.minSpeedCutOff					=		20.0,			//	Minimal speed to consider Door blocked mm/s
// 	.minSpeedSoftwareCutOff			=		30.0,			 /**< Percentage Speed for activation of Software Cutoff */
// 	.RPMCalibrationfactor			=			1,			//	RPM calibration
// 	.stowParkingLimit				=		2500,		//	Maximal parking distance µm
// 	.deployParkingLimit				=		3000,		//	Maximal parking distance µm
// 	
// 	.stowTimeoutTime				=			8000,		//	timeout for stowing
// 	.deployTimeoutTime				=		    8000,		//  timeout for deplying
// 	.blockTimeDelay					=			8000,		//	pause after software cutoff
// 	.cutoffTimer					=			8000,		//	timer to activate cutoff mode
// 	.TTLtimer						=			5000,		//	pause after software cutoff
// 	.CABtimer						=			5000,		//	timer to activate cutoff mode
// 	.doorMoveDelay					=			100,		//	timer to activate cutoff mode
// 	.parkingDelay					=			100,		//	timer to activate cutoff mode
// 	
// 
// 	
// 	.rawTemperatureMin				=			8,			//	Temperature min operation
// 	.rawTemperatureMax				=			70,			//	Temperature max operation
// 
// 	.doorSide						=					RightDoor,	//	Door side
// 	.doorSize						=					Size22inch,	//	Door size

};

const userPreferences		defaultPreferences=
{
	.privacyLightperiod				=		250,		//	Period of blinking Light ms
	.privacyLightDutyCycle			=		50,		//	DutyCycle on the Light %
};

