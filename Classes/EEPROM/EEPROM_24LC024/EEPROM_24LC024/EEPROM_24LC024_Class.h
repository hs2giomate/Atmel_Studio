/* 
* EEPROM_24LC024_Class.h
*
* Created: 4/29/2020 8:54:46 AM
* Author: GMateusDP
*/


#ifndef __EEPROM_24LC024_CLASS_H__
#define __EEPROM_24LC024_CLASS_H__
#include "driver_init.h"
#include "I2C_Asyn_Class.h"
//#include "I2C_Class.h"
#include "USB_CDC_Class.h"
#include "QuikEval_EEPROM.h"
#include "LT_I2C.h"

class EEPROM_24LC024_Class
{
//variables
public:
	 static I2C_Asyn_Class		i2c;
	//static I2C_Asyn_Class		i2c;
	demo_board_type		exprimental_board;
	i2c_m_async_desc	I2C_A;
	USB_CDC_Class		usb;
protected:
private:
	char ui_buffer[64];
	

//functions
public:
	EEPROM_24LC024_Class();
	EEPROM_24LC024_Class(i2c_m_async_desc *);
	~EEPROM_24LC024_Class();
	void	init();
	uint8_t read_id_string(char *buffer);
	uint8_t  read_buffer_with_terminator(uint8_t i2c_address, char *buffer, uint16_t address_data, char terminator, uint8_t count);
	int8_t eeprom_poll(uint8_t i2c_address);
	int8_t	discover_demo(char *demo_name);
	
protected:
private:
	EEPROM_24LC024_Class( const EEPROM_24LC024_Class &c );
	EEPROM_24LC024_Class& operator=( const EEPROM_24LC024_Class &c );

}; //EEPROM_24LC024_Class

#endif //__EEPROM_24LC024_CLASS_H__
