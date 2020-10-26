/* 
* I2C_Sync_Class.cpp
*
* Created: 10/19/2020 1:01:42 PM
* Author: GMateusDP
*/


#include "I2C_Sync_Class.h"
static	uint8_t	txI2CStaticBuffer[I2C_SYN_BUFFER_SIZE];
static	uint8_t	rxI2CStaticBuffer[I2C_SYN_BUFFER_SIZE];

// default constructor
I2C_Sync_Class::I2C_Sync_Class()
{
} //I2C_Sync_Class
I2C_Sync_Class::I2C_Sync_Class(i2c_m_sync_desc * des)
{
	ptrI2CDescr=des;
} //I2C_Sync_Class

// default destructor
I2C_Sync_Class::~I2C_Sync_Class()
{
} //~I2C_Sync_Class
void	I2C_Sync_Class::SetDescriptor(i2c_m_sync_desc * i2c_a){
	ptrI2CDescr=i2c_a;
	asm("nop");

}
bool	I2C_Sync_Class::Init(void){
	txBuffer=txI2CStaticBuffer;
	rxBuffer=txI2CStaticBuffer;

	i2c_m_sync_get_io_descriptor(ptrI2CDescr, &I2C_io);
	uint8_t	ena=(uint8_t)i2c_m_sync_enable(ptrI2CDescr);
	isOK=ena==0;
	initiated=isOK;
	return isOK;
	
}

bool	I2C_Sync_Class::Init(uint8_t add){
	isOK=Init();
	i2c_m_sync_set_slaveaddr(ptrI2CDescr,(int16_t)add, I2C_M_SEVEN);

	return isOK;
}

int32_t I2C_Sync_Class::SetSlaveAddress(uint8_t add){
	return i2c_m_sync_set_slaveaddr(ptrI2CDescr,(int16_t)add, I2C_M_SEVEN);
}

int32_t I2C_Sync_Class::Write(uint8_t* buffer, uint16_t lenght){
	//Irq_Disable();
	int32_t w= io_write(I2C_io, buffer, lenght);
//	Irq_Enable();
	return w;
}
int32_t I2C_Sync_Class::Read(uint8_t* buffer, uint16_t lenght){
//	Irq_Disable();
	int32_t r= io_read(I2C_io, buffer, lenght);
//	Irq_Enable();
	return r;
}
int32_t I2C_Sync_Class::ReadCommand(uint8_t cmd,uint8_t* buffer, uint16_t lenght){
	//Irq_Disable();
	int32_t r= i2c_m_sync_cmd_read(ptrI2CDescr,cmd,buffer,lenght);
	//Irq_Enable();
	return r;
}

int32_t I2C_Sync_Class::WriteCommand(uint8_t cmd,uint8_t* buffer, uint16_t lenght){
	Irq_Disable();
	int32_t w= i2c_m_sync_cmd_write(ptrI2CDescr,cmd,buffer,lenght);
	Irq_Enable();
	return w;
}
void	I2C_Sync_Class::Irq_Enable(void){
	__DMB();
	__enable_irq();
}
void	I2C_Sync_Class::Irq_Disable(void){
	__DMB();
	__disable_irq();
}
