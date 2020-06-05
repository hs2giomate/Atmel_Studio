/* 
* I2C_Class.cpp
*
* Created: 4/23/2020 11:29:08 AM
* Author: GMateusDP
*/


#include "I2C_Class.h"

I2C_Class	*ptrI2C;
// default constructor
I2C_Class::I2C_Class()
{
	ptrI2C=this;
} //I2C_Class
I2C_Class::I2C_Class(i2c_m_sync_desc *i2c)
{
	ptrI2C=this;
	I2C=*i2c;
} //I2C_Class

// default destructor
I2C_Class::~I2C_Class()
{
	disable();
} //~I2C_Class

void	I2C_Class::init(void){
	I2C_EEPROM_CLOCK_init();
	i2c_m_sync_init(&I2C, SERCOM5);
	I2C_EEPROM_PORT_init();
	i2c_m_sync_get_io_descriptor(&I2C, &I2C_io);
	i2c_m_sync_enable(&I2C);
	
	
	
}

int32_t	I2C_Class::set_slaveaddr(uint8_t add){

	return i2c_m_sync_set_slaveaddr(&I2C,(uint16_t)add, I2C_M_SEVEN);
}
int32_t	I2C_Class::enable(void){
	return	i2c_m_sync_enable(&I2C);
}
int32_t	I2C_Class::disable(void){
	return	i2c_m_sync_disable(&I2C);
}
int32_t I2C_Class::write(uint8_t* buffer, uint16_t lenght){
	return io_write(I2C_io, buffer, lenght);
}
int32_t I2C_Class::write_ack(uint8_t* buffer, uint16_t lenght){
	int32_t	w=io_write(I2C_io, buffer, lenght);
		while (I2C.device.service.msg.flags & I2C_M_BUSY) {
			;
		}
		return w;
}
int32_t I2C_Class::read(uint8_t* buffer, uint16_t lenght){
	return io_read(I2C_io, buffer, lenght);
}
int32_t I2C_Class::read_ack(uint8_t* buffer, uint16_t lenght){
	int32_t	r=io_read(I2C_io, buffer, lenght);
	
	while (I2C.device.service.msg.flags & I2C_M_BUSY) {
		;
	}
	return r;
}
void	I2C_Class::set_descriptor(i2c_m_sync_desc * i2c_a){
	I2C=*i2c_a;
}