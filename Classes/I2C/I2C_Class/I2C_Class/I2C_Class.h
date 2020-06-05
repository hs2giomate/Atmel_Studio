/* 
* I2C_Class.h
*
* Created: 4/23/2020 11:29:08 AM
* Author: GMateusDP
*/


#ifndef __I2C_CLASS_H__
#define __I2C_CLASS_H__
#include "driver_init.h"

class I2C_Class
{
//variables
public:
	io_descriptor *I2C_io;
	i2c_m_sync_desc I2C;
protected:
private:

//functions
public:
	I2C_Class();
	I2C_Class(i2c_m_sync_desc *);
	~I2C_Class();
	void	init();
	int32_t	set_slaveaddr(uint8_t );
	void	set_descriptor(i2c_m_sync_desc *);
	int32_t	enable(void);
	int32_t	disable(void);
	int32_t write(uint8_t* buffer, uint16_t lenght);
	int32_t write_ack(uint8_t* buffer, uint16_t lenght);
	int32_t read(uint8_t* buffer, uint16_t lenght);
	int32_t read_ack(uint8_t* buffer, uint16_t lenght);
	
protected:
private:
	I2C_Class( const I2C_Class &c );
	I2C_Class& operator=( const I2C_Class &c );

}; //I2C_Class

#endif //__I2C_CLASS_H__
