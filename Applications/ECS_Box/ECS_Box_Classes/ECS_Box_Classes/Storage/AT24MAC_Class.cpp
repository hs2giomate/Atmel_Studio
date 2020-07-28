/* 
* AT24MAC_Class.cpp
*
* Created: 6/3/2020 10:56:30 AM
* Author: GMateusDP
*/


#include "AT24MAC_Class.h"

AT24MAC_Class	*ptrAT24MACClass;

// default constructor
AT24MAC_Class::AT24MAC_Class()
{
	ptrAT24MACClass=this;
} //AT24MAC_Class
AT24MAC_Class::AT24MAC_Class(i2c_m_async_desc *i2c)
{
	ptrI2CDescr=i2c;
	ptrAT24MACClass=this;
} //AT24MAC_Class

// default destructor
AT24MAC_Class::~AT24MAC_Class()
{
} //~AT24MAC_Class


void AT24MAC_Class::Init(void){
	Init((uint8_t)AT24MAC_DEVICE_ADDRESS);
}
void AT24MAC_Class::Init(i2c_m_async_desc *i2c){
	ptrI2CDescr=i2c;
	Init((uint8_t)AT24MAC_DEVICE_ADDRESS);
}

void AT24MAC_Class::Init(uint8_t addr ){
	i2c_addr=addr;
	i2ca.Set_descriptor(ptrI2CDescr);
	isReady=i2ca.Init(addr)==0;
}


int32_t AT24MAC_Class::write_byte(uint8_t addr, uint8_t value){
	
	//uint8_t cmd=(uint8_t)AT24MAC_WRITE_CMD;
	isReady=false;
	uint8_t array[2];
	array[0]=addr;
	array[1]=value;
	uint32_t w= i2ca.Write(array,2);
	while(!i2ca.txReady);
	return	w;
}
uint32_t AT24MAC_Class::WriteAddress(uint8_t *p, uint8_t addr, uint8_t size){
	
	//uint8_t cmd=(uint8_t)AT24MAC_WRITE_CMD;
	
	uint8_t array[1+AT24MAC_BUFFER_SIZE],value;
	uint32_t w;
	isReady=false;
	array[0]=addr;
	for (int i=1;i<size+1;i++)
	{
				
		value=*p;
		array[i]=value;
	
		p++;
	}
	w= i2ca.Write(array,1+size);
	while(!i2ca.txReady);
	
	return	w;
}
uint32_t AT24MAC_Class::WriteAddress(uint8_t *p, uint16_t addr, uint8_t size){
	
	//uint8_t cmd=(uint8_t)AT24MAC_WRITE_CMD;
	
	uint8_t array[2+AT24MAC_BUFFER_SIZE],value;
	uint32_t w;
	isReady=false;
	array[0]=(uint8_t)((addr&(0xff00))>>8);
	array[1]=(uint8_t)(addr)&(0xff);
	for (int i=2;i<size+2;i++)
	{
		
		value=*p;
		array[i]=value;
		
		p++;
	}
	w= i2ca.Write(array,2+size);
	while(!i2ca.txReady);
	
	return	w;
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
	return i2ca.Write(frame,17);
}

uint8_t AT24MAC_Class::read_byte(uint8_t addr){

	uint8_t value;
	//i2ca.read_cmd(addr,&value);
	i2ca.Write(&addr,1);
	while(!i2ca.txReady);
	i2ca.Read(&value,1);
	while(!i2ca.rxReady);
	return value;
}

uint32_t AT24MAC_Class::ReadAddress(uint8_t *p, uint8_t addr, uint8_t size){
	//i2ca.read_cmd(addr,&value);
	i2ca.Write(&addr,1);
	while(!i2ca.txReady);
	uint32_t r= i2ca.Read(p,size);
	while(!i2ca.rxReady);
	return r;
}
uint32_t AT24MAC_Class::ReadAddress(uint8_t *p, uint16_t addr, uint8_t size){
	//i2ca.read_cmd(addr,&value);
	
	i2ca.Write((uint8_t *)&addr,2);
	while(!i2ca.txReady);
	uint32_t r= i2ca.Read(p,size);
	while(!i2ca.rxReady);
	return r;
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

AT24MAC_Class	eeprom(&I2C_EEPROM);