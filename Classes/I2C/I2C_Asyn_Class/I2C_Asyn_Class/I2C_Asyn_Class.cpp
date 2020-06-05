/* 
* I2C_Asyn_Class.cpp
*
* Created: 4/29/2020 8:29:55 AM
* Author: GMateusDP
*/


#include "I2C_Asyn_Class.h"


I2C_Asyn_Class	*ptrI2C_Asyn_Class;
static void I2C_tx_complete(struct i2c_m_async_desc *const i2c)
{
	//ptrI2C_Asyn_Class->txReady=true;
	ptrI2C_Asyn_Class->txReady=true;
	//ptrI2C_Asyn_Class->txReady=true;
}
 static void I2C_rx_complete(struct i2c_m_async_desc *const i2c)
{
	ptrI2C_Asyn_Class->
}
 static void I2C_error_transfer(struct i2c_m_async_desc *const i2c)
 {
	// ptrI2C_Asyn_Class->send_stop();
		ptrI2C_Asyn_Class->clear_ack();
		ptrI2C_Asyn_Class->txReady=true;
		ptrI2C_Asyn_Class->rxReady=true;
 }

// default constructor
I2C_Asyn_Class::I2C_Asyn_Class()
{
	I2C_A=I2C_EEPROM;
	ptrI2C_Asyn_Class=this;
} //I2C_Asyn_Class
// default constructor
I2C_Asyn_Class::I2C_Asyn_Class(i2c_m_async_desc *i2c_a)
{
	I2C_A=*i2c_a;
	ptrI2C_Asyn_Class=this;
} //I2C_Asyn_Class

// default destructor
I2C_Asyn_Class::~I2C_Asyn_Class()
{
} //~I2C_Asyn_Class

void	I2C_Asyn_Class::set_descriptor(i2c_m_async_desc * i2c_a){
	I2C_A=*i2c_a;
	ptrI2C_Asyn_Class=this;
}

void	I2C_Asyn_Class::init(void){
		I2C_EEPROM_CLOCK_init();
		i2c_m_async_init(&I2C_A, SERCOM5);
		I2C_EEPROM_PORT_init();

	i2c_m_async_get_io_descriptor(&I2C_A, &I2C_io);
	i2c_m_async_enable(&I2C_A);
	i2c_m_async_register_callback(&I2C_A, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_tx_complete);
	i2c_m_async_register_callback(&I2C_A, I2C_M_ASYNC_RX_COMPLETE, (FUNC_PTR)I2C_rx_complete);
	i2c_m_async_register_callback(&I2C_A, I2C_M_ASYNC_ERROR, (FUNC_PTR)I2C_error_transfer);
	//	i2c_m_async_set_slaveaddr(&I2C_A, 0x12, I2C_M_SEVEN);
	txReady=true;
	rxReady=true;
}

int32_t	I2C_Asyn_Class::set_slaveaddr(uint8_t add){
	return i2c_m_async_set_slaveaddr(&I2C_A,(int16_t)add, I2C_M_SEVEN);
}
int32_t	I2C_Asyn_Class::enable(void){
	return	i2c_m_async_enable(&I2C_A);
}
int32_t	I2C_Asyn_Class::disable(void){
	return	i2c_m_async_disable(&I2C_A);
}
int32_t I2C_Asyn_Class::write_ack(uint8_t* buffer, uint16_t lenght){
	int32_t	w;
	ack=false;
	for(int i=0;i<1024;i++){
		while(!txReady);
		txReady=false;
		w=io_write(I2C_io, buffer, lenght);
		if (ack) break;	
		delay_ms(1);
	}
	return w;
	
}
int32_t I2C_Asyn_Class::write(uint8_t* buffer, uint16_t lenght){
	while(!txReady);
	txReady=false;
	return io_write(I2C_io, buffer, lenght);
}
int32_t I2C_Asyn_Class::write_not_ack(uint8_t* buffer, uint16_t lenght){
	//while(!txReady);
	//txReady=false;
	return io_write(I2C_io, buffer, lenght);
}
int32_t I2C_Asyn_Class::write_cmd(uint8_t cmd, uint8_t value){
	while(!txReady);
	txReady=false;
	localBuffer=cmd;
	localValue=value;
	return  i2c_m_async_cmd_write(&I2C_A, cmd, value);
}
int32_t I2C_Asyn_Class::rewrite_cmd(void){
	//while(!is_tx_ready());
	txReady=false;

	return  i2c_m_async_cmd_write(&I2C_A, localBuffer, localValue);
}
int32_t I2C_Asyn_Class::read_cmd(uint8_t cmd, uint8_t *value){
	while(!rxReady);
	rxReady=false;
	return  i2c_m_async_cmd_read(&I2C_A, cmd, value);
}
int32_t I2C_Asyn_Class::read(uint8_t* buffer, uint16_t lenght){
	
		while(!rxReady);
		rxReady=false;
		return io_read(I2C_io, buffer, lenght);
}

int32_t I2C_Asyn_Class::read_ack(uint8_t* buffer, uint16_t lenght){
		int32_t	r;
		ack=false;
	for(int i=0;i<1024;i++){
		while(!rxReady);
		rxReady=false;
		r=io_read(I2C_io, buffer, lenght);
		if (ack) break;	
		delay_ms(1);
	}
	return r;
}
int32_t I2C_Asyn_Class::read_not_ack(uint8_t* buffer, uint16_t lenght){
	while(!rxReady);
	rxReady=false;
	int32_t	r=io_read(I2C_io, buffer, lenght);
//	while(!rxReady);
	return r;
}
 volatile bool I2C_Asyn_Class::set_tx_ready(void){
	txReady=true;

	 ack=true;
	return txReady;
}
 volatile bool I2C_Asyn_Class::set_rx_ready(void){
	rxReady=true;

		 ack=true;
	return rxReady;
}

volatile bool	I2C_Asyn_Class::is_tx_ready(void){
	return	txReady;
}
volatile bool	I2C_Asyn_Class::is_rx_ready(void){
	return	rxReady;
}
int32_t I2C_Asyn_Class::send_stop(void){
    while(!txReady);
	//txReady=false;
	return i2c_m_async_send_stop(&I2C_A);
}
 volatile bool I2C_Asyn_Class::clear_ack(void){
	 ack=false;
	 return ack;
 }