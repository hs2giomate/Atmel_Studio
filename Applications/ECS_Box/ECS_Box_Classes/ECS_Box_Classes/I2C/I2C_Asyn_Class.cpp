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

	ptrI2C_Asyn_Class->txReady=true;

}
 static void I2C_rx_complete(struct i2c_m_async_desc *const i2c)
{
	ptrI2C_Asyn_Class->rxReady=true;
}
 static void I2C_error_transfer(struct i2c_m_async_desc *const i2c)
 {

		ptrI2C_Asyn_Class->Clear_ack();
		ptrI2C_Asyn_Class->txReady=true;
		ptrI2C_Asyn_Class->rxReady=true;
 }

// default constructor
I2C_Asyn_Class::I2C_Asyn_Class()
{
	ptrI2CAsynDescr=&I2C_EEPROM;
	ptrI2C_Asyn_Class=this;
} //I2C_Asyn_Class
// default constructor
I2C_Asyn_Class::I2C_Asyn_Class(i2c_m_async_desc *i2c_a)
{
	ptrI2CAsynDescr=i2c_a;
	ptrI2C_Asyn_Class=this;
} //I2C_Asyn_Class

// default destructor
I2C_Asyn_Class::~I2C_Asyn_Class()
{
} //~I2C_Asyn_Class

void	I2C_Asyn_Class::Set_descriptor(i2c_m_async_desc * i2c_a){
	ptrI2CAsynDescr=i2c_a;
	ptrI2C_Asyn_Class=this;
}

uint8_t	I2C_Asyn_Class::Init(void){
	

	i2c_m_async_get_io_descriptor(ptrI2CAsynDescr, &I2C_io);
  	uint8_t	ena=(uint8_t)i2c_m_async_enable(ptrI2CAsynDescr);
	i2c_m_async_register_callback(ptrI2CAsynDescr, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_tx_complete);
	i2c_m_async_register_callback(ptrI2CAsynDescr, I2C_M_ASYNC_RX_COMPLETE, (FUNC_PTR)I2C_rx_complete);
	i2c_m_async_register_callback(ptrI2CAsynDescr, I2C_M_ASYNC_ERROR, (FUNC_PTR)I2C_error_transfer);
	//	i2c_m_async_set_slaveaddr(&I2C_A, 0x12, I2C_M_SEVEN);
	txReady=true;
	rxReady=true;
	return ena;
	
}
uint8_t	I2C_Asyn_Class::Init(uint8_t add){
	uint8_t ena=Init();
	i2c_m_async_set_slaveaddr(ptrI2CAsynDescr,(int16_t)add, I2C_M_SEVEN);
	txReady=true;
	rxReady=true;
	return ena;
}

int32_t	I2C_Asyn_Class::Set_slaveaddr(uint8_t add){
	return i2c_m_async_set_slaveaddr(ptrI2CAsynDescr,(int16_t)add, I2C_M_SEVEN);
}
int32_t	I2C_Asyn_Class::Enable(void){
	return	i2c_m_async_enable(ptrI2CAsynDescr);
}
int32_t	I2C_Asyn_Class::Disable(void){
	return	i2c_m_async_disable(ptrI2CAsynDescr);
}
int32_t I2C_Asyn_Class::Write_ack(uint8_t* buffer, uint16_t lenght){
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
int32_t I2C_Asyn_Class::Write(uint8_t* buffer, uint16_t lenght){
	while(!txReady);
	txReady=false;
	return io_write(I2C_io, buffer, lenght);
}
int32_t I2C_Asyn_Class::Write_not_ack(uint8_t* buffer, uint16_t lenght){
	//while(!txReady);
	//txReady=false;
	return io_write(I2C_io, buffer, lenght);
}
int32_t I2C_Asyn_Class::Write_cmd(uint8_t cmd, uint8_t value){
	while(!txReady);
	txReady=false;
	localBuffer=cmd;
	localValue=value;
	return  i2c_m_async_cmd_write(ptrI2CAsynDescr, cmd, value);
}
int32_t I2C_Asyn_Class::Rewrite_cmd(void){
	//while(!is_tx_ready());
	txReady=false;

	return  i2c_m_async_cmd_write(ptrI2CAsynDescr, localBuffer, localValue);
}
int32_t I2C_Asyn_Class::Read_cmd(uint8_t cmd, uint8_t *value){
	while(!rxReady);
	rxReady=false;
	int32_t r= i2c_m_async_cmd_read(ptrI2CAsynDescr, cmd, value);
	while(!rxReady);
	return r;
}
int32_t I2C_Asyn_Class::Read(uint8_t* buffer, uint16_t lenght){
	
		while(!rxReady);
		rxReady=false;
		return io_read(I2C_io, buffer, lenght);
}

int32_t I2C_Asyn_Class::Read_ack(uint8_t* buffer, uint16_t lenght){
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
int32_t I2C_Asyn_Class::Read_not_ack(uint8_t* buffer, uint16_t lenght){
	while(!rxReady);
	rxReady=false;
	int32_t	r=io_read(I2C_io, buffer, lenght);
//	while(!rxReady);
	return r;
}
 volatile bool I2C_Asyn_Class::Set_tx_ready(void){
	txReady=true;

	 ack=true;
	return txReady;
}
 volatile bool I2C_Asyn_Class::Set_rx_ready(void){
	rxReady=true;

		 ack=true;
	return rxReady;
}

volatile bool	I2C_Asyn_Class::Is_tx_ready(void){
	return	txReady;
}
volatile bool	I2C_Asyn_Class::Is_rx_ready(void){
	return	rxReady;
}
int32_t I2C_Asyn_Class::Send_stop(void){
    while(!txReady){};
	//txReady=false;
	return i2c_m_async_send_stop(ptrI2CAsynDescr);
}
 volatile bool I2C_Asyn_Class::Clear_ack(void){
	 ack=false;
	 return ack;
 }