/* 
* I2C_Asyn_Class.h
*
* Created: 4/29/2020 8:29:56 AM
* Author: GMateusDP
*/


#ifndef __I2C_ASYN_CLASS_H__
#define __I2C_ASYN_CLASS_H__
#include "driver_init.h"

class I2C_Asyn_Class
{
//variables
public:
	volatile bool  txReady;
	volatile bool  rxReady;
	io_descriptor *I2C_io;
	i2c_m_async_desc I2C_A;
	volatile	bool	ack;
protected:
private:
	uint8_t	localBuffer;
	uint8_t	localValue;

//functions
public:
	I2C_Asyn_Class();
	I2C_Asyn_Class(i2c_m_async_desc *);
	~I2C_Asyn_Class();
	void	init(void);
	int32_t	send_stop(void);
	void	set_descriptor(i2c_m_async_desc *);
	int32_t	set_slaveaddr(uint8_t add);
	int32_t	enable(void);
	int32_t	disable(void);
	int32_t write(uint8_t* buffer, uint16_t lenght);
	int32_t write_ack(uint8_t* buffer, uint16_t lenght);
	int32_t write_not_ack(uint8_t* buffer, uint16_t lenght);
	int32_t read(uint8_t* buffer, uint16_t lenght);
	int32_t read_ack(uint8_t* buffer, uint16_t lenght);
	int32_t read_not_ack(uint8_t* buffer, uint16_t lenght);
	volatile	bool	set_rx_ready(void);
	volatile	bool	clear_ack(void);
	volatile	bool	is_tx_ready(void);
	volatile	bool	set_tx_ready(void);
	volatile	bool	is_rx_ready(void);
	int32_t		write_cmd(uint8_t cmd, uint8_t value);
	int32_t		read_cmd(uint8_t cmd, uint8_t *value);
	int32_t		rewrite_cmd(void);
	
protected:
private:
	I2C_Asyn_Class( const I2C_Asyn_Class &c );
	I2C_Asyn_Class& operator=( const I2C_Asyn_Class &c );

}; //I2C_Asyn_Class

#endif //__I2C_ASYN_CLASS_H__
