/* 
* I2C_Sync_Class.h
*
* Created: 10/19/2020 1:01:43 PM
* Author: GMateusDP
*/


#ifndef __I2C_SYNC_CLASS_H__
#define __I2C_SYNC_CLASS_H__
#include "driver_init.h"
#define		I2C_SYN_BUFFER_SIZE 64

class I2C_Sync_Class
{
//variables
public:
		uint8_t	*txBuffer;
		uint8_t	*rxBuffer;
		volatile	bool initiated,isOK;
protected:
private:
	io_descriptor *I2C_io;
	i2c_m_sync_desc *ptrI2CDescr;
	uint8_t	enabled;
	
	//I2C_Sync_Class*  i2c;

//functions
public:
	I2C_Sync_Class();
	I2C_Sync_Class(i2c_m_sync_desc *);
	~I2C_Sync_Class();
	void	SetDescriptor(i2c_m_sync_desc *);
	int32_t SetSlaveAddress(uint8_t add);
	bool	Init(uint8_t);
	bool	Init(void);
	int32_t Write(uint8_t* buffer, uint16_t lenght);
	int32_t Read(uint8_t* buffer, uint16_t lenght);
	int32_t ReadCommand(uint8_t cmd,uint8_t* buffer, uint16_t lenght);
	int32_t WriteCommand(uint8_t cmd,uint8_t* buffer, uint16_t lenght);
	
protected:
private:
	I2C_Sync_Class( const I2C_Sync_Class &c );
	I2C_Sync_Class& operator=( const I2C_Sync_Class &c );
	void	Irq_Disable(void);
	void	Irq_Enable(void);

}; //I2C_Sync_Class
static I2C_Sync_Class	i2cSharedStatic(&I2C_HEATERS);

#endif //__I2C_SYNC_CLASS_H__
