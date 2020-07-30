/* 
* AT24MAC_Class.h
*
* Created: 6/3/2020 10:56:30 AM
* Author: GMateusDP
*/


#ifndef __AT24MAC_CLASS_H__
#define __AT24MAC_CLASS_H__
#include "driver_init.h"
#include "utils.h"
#include "I2C_Asyn_Class.h"




#define AT24MAC_SLAVE_ADDRESS      (0x06)
#define AT24MAC_DEVICE_ADDRESS      (0x56)
#define AT24MAC_EEPROM_ADDRESS      (0x0A<<4)
#define AT24MAC_WRITE_CMD			 (0xAC)
#define AT24MAC_READ_CMD			 (0xAD)
#define AT24MAC_RW_PROTECT_ADDRESS  (0x06<<4)
#define AT24MAC_SERIAL_ADDRESS      (0x0B<<4)

/* Known memory blocks */
#define AT24MAC_SERIAL_OFFSET   (0x80)
#define AT24MAC_EUI64_OFFSET    (0x98)
#define AT24MAC_EUI48_OFFSET    (0x9A)

#define SERIAL_LEN 16
#define EUI64_LEN 8
#define EUI48_LEN 6
#define AT24MAC_BUFFER_SIZE 16
#define AT24MAC_MEMORY_SIZE 256



class AT24MAC_Class
{
//variables
public:
	volatile bool isReady;

protected:
private:
	i2c_m_async_desc *ptrI2CDescr;
	
	I2C_Asyn_Class	i2ca;
	uint8_t		i2c_addr;
	uint8_t		byte;
	
	
//functions
public:
	AT24MAC_Class();
	AT24MAC_Class(i2c_m_async_desc *i2c);
	~AT24MAC_Class();
	void Init(void);
	void Init(uint8_t addr);
	void Init(i2c_m_async_desc *);
	int32_t Write_byte(uint8_t addr, uint8_t value);
	int32_t write_page(uint8_t addr, uint8_t *buffer);
	uint8_t	Read_byte(uint8_t );
	bool	IsReady(void);
	uint32_t ReadAddress(uint8_t *p, uint8_t addr, uint8_t size);
	uint32_t ReadAddress(uint8_t *p, uint16_t addr, uint8_t size);
	uint32_t WriteAddress(uint8_t *p, uint8_t addr, uint8_t size);
	uint32_t WriteAddress(uint8_t *p, uint16_t addr, uint8_t size);
	bool	AcknolledgePolling(void);
protected:
	
private:
	AT24MAC_Class( const AT24MAC_Class &c );
	AT24MAC_Class& operator=( const AT24MAC_Class &c );
	bool	GetAcknowledge(void);


}; //AT24MAC_Class

extern	AT24MAC_Class	eeprom;
#endif //__AT24MAC_CLASS_H__
