/**
*\file Parameters.h
* \brief Application for the automatic control of the Door Actuator P/N 2400024
 *	Copyright (c)  2019 hs2-engineering. All rights reserved. 
 *  Parameters.h
 *
 * Created: 10/23/2019 2:13:31 PM
 *  Author: Giovanny Mateus
 */ 


#ifndef PARAMETERS_H_
#define PARAMETERS_H_


#include "coreTypes.h"
/************************************************************************/
/* User Parameter for Commissioning of the Door                                                                   */
/************************************************************************/

struct userParameters
{
	uint16		slowSpeedCurrentLimitStow;			/**<	Current Limitation mAmp */
	uint16		fastSpeedCurrentLimitStow;		    /**<	fast speed current limitation*/
	uint16		parkingSpeedCurrentLimitStow;		/**<	fast speed current limitation*/
	uint16		slowSpeedCurrentLimitDeploy;		/**<	Current Limitation: Number of Steps for electronic resistor*/
	uint16		fastSpeedCurrentLimitDeploy;		/**<	fast speed current limitation */
	uint16		parkingSpeedCurrentLimitDeploy;		/**<	fast speed current limitation */
	float		motorCurrentLimit;					/**<	Motorvelocity Slow */
	float		motorCurrentResistorFactor;			/**<	Motorvelocity Slow */
	float		slowSpeedStow;						/**<	Motorvelocity Slow */
	float		fastSpeedStow;						/**<	Motorvelocity High */
	float		parkingSpeed;					/**<	Motorvelocity High */
	float		slowSpeedDeploy;						/**<	Motorvelocity Slow */
	float		fastSpeedDeploy;						/**<	Motorvelocity High */
	float		slowSpeedOffsetDeploy;			/**<	Distance from deploy position for entering slow speed */
	float		slowSpeedOffsetStow;			/**<	Distance from deploy position for entering slow speed */
	float		distanceCalibrationValue;		/**<	Calibration, Motorsignals vs. Distance */
	float		kDeployPosition24inch;			/**<	Distance from deploy position for entering slow speed */
	float		kDeployPosition22inch;			/**<	Distance from deploy position for entering slow speed */
	float		minSpeedCutOff;					/**<	Minimal speed to consider Door blocked */
	float		minSpeedSoftwareCutOff;			/**<	Minimal speed to consider Door blocked */
	float		RPMCalibrationfactor;			/**<	Minimal speed to consider Door blocked */
	float		stowParkingLimit;				/**<	Maximal parking distance µm */
	float		deployParkingLimit;				/**<	Maximal parking distance µm */
	
	time_t		stowTimeoutTime;				/**<	timeout for stowing */
	time_t		deployTimeoutTime;				/**<  timeout for deplying */
	time_t		blockTimeDelay;					/**<	pause after software cutoff */
	time_t		cutoffTimer;					/**<	timer to activate cutoff mode */
	time_t		TTLtimer;	      				/**<	timer to activate cutoff mode */
	time_t		CABtimer;	      				/**<	timer to activate cutoff mode */
	time_t		doorMoveDelay;	      			/**<	timer to activate cutoff mode */
	time_t		parkingDelay;	      			/**<	timer to activate cutoff mode */
	
		
	float		rawTemperatureMin;				/**<	Temperature min operation */
	float		rawTemperatureMax;				/**<	Temperature max operation */

	bool		doorSide;						/**<	Door side */
	bool		doorSize;						/**<	Door Size	*/	
};

typedef struct userParameters	userParameters;

struct userPreferences
{
	uint32	privacyLightperiod;					/**<	Period of blinking Light ms */
	uint32	privacyLightDutyCycle;				/**<	DutyCycle on the Light %  */
};

typedef struct userPreferences userPreferences;

enum{
	RightDoor=0,
	LeftDoor
	};
	
enum{
	Size22inch=0,
	Size24inch
};

extern const userParameters		defaultParameters;   /**< user Parameters */
extern const userPreferences	defaultPreferences;	/**< user Preferences */



#endif /* PARAMETERS_H_ */