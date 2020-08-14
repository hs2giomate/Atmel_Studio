/* 
* AT24MAC_Class.cpp
*
* Created: 6/3/2020 10:56:30 AM
* Author: GMateusDP
*/


#include "AT24MAC_Class.h"
#include "stdlib.h"

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


bool AT24MAC_Class::Init(void){
	Init((uint8_t)AT24MAC_DEVICE_ADDRESS);
	return SelfTest();
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


int32_t AT24MAC_Class::Write_byte(uint8_t addr, uint8_t value){
	
	isReady=false;
	uint8_t array[2];
	array[0]=addr;
	array[1]=value;
	uint32_t w= i2ca.Write(array,2);
	while(!i2ca.txReady);
	return	w;
}
uint32_t AT24MAC_Class::WriteAddress(uint8_t *p, uint8_t addr, uint8_t size){
	uint32_t w;
	for (int i = 0; i < size; i++)
	{
		while(!IsReady());
		w=Write_byte(addr+i,*p);
		p++;
		delay_ms(2);
		while(!AcknolledgePolling());
	}
	
//	uint8_t array[1+AT24MAC_BUFFER_SIZE],value;
//	
// 	isReady=false;
// 	array[0]=addr;
// 	for (int i=1;i<size+1;i++)
// 	{				
// 		value=*p;
// 		array[i]=value;
// 		p++;
// 	}
// 	w= i2ca.Write(array,1+size);
// 	while(!i2ca.txReady);
	return	w;
}
uint32_t AT24MAC_Class::WriteAddress(uint8_t *p, uint16_t addr, uint8_t size){
	

	uint8_t add=(uint8_t)(0xff&addr);
	return	WriteAddress(p,add,size);
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

uint8_t AT24MAC_Class::Read_byte(uint8_t addr){

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
	uint8_t add=(uint8_t)(0xff&addr);
	
	return ReadAddress(p,add,size);
}
bool	AT24MAC_Class::GetAcknowledge(void){
	byte=Read_byte(0);
	return	byte>0;
}
bool AT24MAC_Class::AcknolledgePolling(void){
	do 
	{
		GetAcknowledge();
	} while (!IsReady());
	return isReady;
}

bool	AT24MAC_Class::SelfTest(void){
	currentAddress=AT24MAC_MEMORY_SIZE-AT24MAC_BUFFER_SIZE;
	for (int i = 0; i < AT24MAC_BUFFER_SIZE ; i++) {
		tx_buffer[i] = (uint8_t)rand();
		rx_buffer[i] = (uint8_t)(AT24MAC_BUFFER_SIZE-i);
	}

	while(!IsReady());
	WriteAddress(tx_buffer,currentAddress,AT24MAC_BUFFER_SIZE);
	while(!IsReady());
	ReadAddress(rx_buffer,currentAddress,AT24MAC_BUFFER_SIZE);
	isOK = true;
	for (int i = 0; i < AT24MAC_BUFFER_SIZE; i++) {
		if (tx_buffer[i] != rx_buffer[i]) {
			isOK = false;
// 			usb.print("EEPROM verification failed. Address: ");
// 			usb.print(addr,HEX);
// 			usb<<" bit :"<<i<<NEWLINE;
			//flashAddress=0;
			
			break;
		}
		
	}
	return isOK;
}


bool AT24MAC_Class::IsReady(void){

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