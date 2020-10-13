/* 
* I2C_Asyn_Class.h
*
* Created: 4/29/2020 8:29:56 AM
* Author: GMateusDP
*/


#ifndef __I2C_ASYN_CLASS_H__
#define __I2C_ASYN_CLASS_H__
#include "driver_init.h"
#include "I2C_ObjectList.h"

class I2C_Asyn_Class: public I2C_ObjectList
{
//variables
public:
	volatile bool  txReady;
	volatile bool  rxReady;
	bool initiated;
	volatile	bool	ack,isOK;
	I2C_Asyn_Class	*ptrClass;
protected:

private:
	io_descriptor *I2C_io;
	i2c_m_async_desc *ptrI2CAsynDescr;
	uint8_t	localBuffer;
	uint8_t	localValue;
	I2CPointerAndDescriptor coupleKey;

//functions
public:
	I2C_Asyn_Class();
	I2C_Asyn_Class(i2c_m_async_desc *);
	~I2C_Asyn_Class();
	bool	Init(void);
	bool	Init(uint8_t);
	bool	Init(i2c_m_async_desc * i2c_a);
	
	void	Set_descriptor(i2c_m_async_desc *);
	int32_t	Set_slaveaddr(uint8_t add);
	int32_t	Enable(void);
	int32_t	Disable(void);
	int32_t Write(uint8_t* buffer, uint16_t lenght);
	int32_t Write_ack(uint8_t* buffer, uint16_t lenght);
	int32_t Write_not_ack(uint8_t* buffer, uint16_t lenght);
	int32_t Read(uint8_t* buffer, uint16_t lenght);
	int32_t Read_ack(uint8_t* buffer, uint16_t lenght);
	int32_t Read_not_ack(uint8_t* buffer, uint16_t lenght);
	volatile	bool	Set_rx_ready(void);
	volatile	bool	Clear_ack(void);
	volatile	bool	Is_tx_ready(void);
	volatile	bool	Set_tx_ready(void);
	volatile	bool	Is_rx_ready(void);
	int32_t		Write_cmd(uint8_t cmd, uint8_t value);
	int32_t		Read_cmd(uint8_t cmd, uint8_t *value);
	int32_t		Rewrite_cmd(void);
	
protected:
private:
	I2C_Asyn_Class( const I2C_Asyn_Class &c );
	I2C_Asyn_Class& operator=( const I2C_Asyn_Class &c );
	int32_t	Send_stop(void);

}; //I2C_Asyn_Class

extern	list_descriptor		i2cList;
extern I2C_Asyn_Class		i2cFVs;

#endif //__I2C_ASYN_CLASS_H__
