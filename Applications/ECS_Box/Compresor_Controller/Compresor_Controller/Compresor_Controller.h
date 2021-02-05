/* 
* Compresor_Controller.h
*
* Created: 12/16/2020 12:17:33 PM
* Author: GMateusDP
*/


#ifndef __COMPRESOR_CONTROLLER_H__
#define __COMPRESOR_CONTROLLER_H__
#include "Compresor_I2C_Interface.h"
#include "Compressor_CANOpen_Interface.h"
#define COMPRESSOR_CONTROLLER_DATA_SIZE 24

class Compresor_Controller: public Compresor_I2C_Interface, public Compressor_CANOpen_Interface
{
//variables
public:
	volatile bool isOK;
	
protected:
private:
	bool  result;
	uint8_t gpio_status;
	uint16_t current_speed, setpoint_speed;
	
	uint8_t	*data_array;
//functions
public:
	Compresor_Controller();
	~Compresor_Controller();
	bool Init(void);
	void GetCompressorStatus(uint8_t*);
protected:
private:
	Compresor_Controller( const Compresor_Controller &c );
	Compresor_Controller& operator=( const Compresor_Controller &c );
	uint8_t Set_Slave_ID (uint8_t sn);

}; //Compresor_Controller

extern Compresor_Controller ccu;

#endif //__COMPRESOR_CONTROLLER_H__
