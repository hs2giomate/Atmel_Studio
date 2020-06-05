/* 
* AT24MAC_Class.cpp
*
* Created: 6/3/2020 10:56:30 AM
* Author: GMateusDP
*/


#include "AT24MAC_Class.h"

AT24MAC_Class	*ptrClass;
// default constructor
AT24MAC_Class::AT24MAC_Class()
{
	ptrClass=this;
} //AT24MAC_Class
AT24MAC_Class::AT24MAC_Class(i2c_m_async_desc *i2c)
{
	I2C0=i2c;
	ptrClass=this;
} //AT24MAC_Class

// default destructor
AT24MAC_Class::~AT24MAC_Class()
{
} //~AT24MAC_Class
void AT24MAC_Class::init(void){
	init((uint8_t)AT24MAC_DEVICE_ADDRESS);
}

void AT24MAC_Class::init(uint8_t addr ){
	i2c_addr=addr;
	i2ca.set_descriptor(I2C0);
	i2ca.init();
	i2ca.set_slaveaddr(addr);
	isReady=i2ca.enable()==0;

}


int32_t AT24MAC_Class::write_byte(uint8_t addr, uint8_t value){
	
	//uint8_t cmd=(uint8_t)AT24MAC_WRITE_CMD;
	isReady=false;
	return i2ca.write_cmd(addr,value);
}
int32_t AT24MAC_Class::write_page(uint8_t addr, uint8_t *buffer){
	uint8_t frame[17];
	frame[0]=addr;
	for (int i=1;i<17;i++)
	{
		frame[i]=*buffer;
		buffer++;
	}
	//uint8_t cmd=(uint8_t)AT24MAC_WRITE_CMD;
	return i2ca.write(frame,17);
}

uint8_t AT24MAC_Class::read_byte(uint8_t addr){
	i2ca.enable();
	uint8_t value;
	i2ca.write(&addr,1);
	i2ca.read(&value,1);
	while(!i2ca.rxReady);
	return value;
}

bool AT24MAC_Class::is_EEPROM_ready(void){
	if (i2ca.txReady && i2ca.rxReady)
	{
		isReady=true;
	}
	else
	{
		isReady=false;
	}
	return isReady;	
}