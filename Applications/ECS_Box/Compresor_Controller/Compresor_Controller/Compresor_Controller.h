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

class Compresor_Controller: public Compresor_I2C_Interface, public Compressor_CANOpen_Interface
{
//variables
public:
	volatile bool isOK;
protected:
private:
	bool  result;
//functions
public:
	Compresor_Controller();
	~Compresor_Controller();
	bool Init(void);
protected:
private:
	Compresor_Controller( const Compresor_Controller &c );
	Compresor_Controller& operator=( const Compresor_Controller &c );

}; //Compresor_Controller

extern Compresor_Controller ccu;

#endif //__COMPRESOR_CONTROLLER_H__
