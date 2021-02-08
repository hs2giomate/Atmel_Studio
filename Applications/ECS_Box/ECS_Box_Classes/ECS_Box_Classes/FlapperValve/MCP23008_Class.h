/* 
* MCP23008_Class.h
*
* Created: 10/2/2020 6:08:22 PM
* Author: GMateusDP
*/


#ifndef __MCP23008_CLASS_H__
#define __MCP23008_CLASS_H__
//#include "I2C_Asyn_Class.h"
#include "I2C_Sync_Class.h"
#include "Arduino.h"

#define		MCP23008_INPUT		1
#define		MCP23008_OUTPUT		0
#define		MCP23008_LOW		0
#define		MCP23008_HIGH		1
#define		MCP23008_CHANGE		2
#define		MCP23008_FALLING		3
//default device address
#define MCP23008_ADDRESS 0x20

//registers (default IOCON.BANK = 0)
#define MCP23008_IODIRA 0x00
#define MCP23008_IPOLA 0x01
#define MCP23008_GPINTENA 0x02
#define MCP23008_DEFVALA 0x03
#define MCP23008_INTCONA 0x04
#define MCP23008_IOCONA 0x05
#define MCP23008_GPPUA 0x06
#define MCP23008_INTFA 0x07
#define MCP23008_INTCAPA 0x8
#define MCP23008_GPIOA 0x9
#define MCP23008_OLATA 0xa
#define MCP23008_INT_ERR 255
#define MCP23008_NUMBER_PINS 8

class MCP23008_Class
{
//variables
public:
	
	uint8_t		registerValue,regAddress,controlRegisterA,controlRegisterB,portA,portB;
	volatile bool  isOK,hasChanged;
protected:
private:
	uint8_t	i2cAddress;
	I2C_Sync_Class* i2cClass;
	 uint8_t	*txBuffer;
	 uint8_t	*rxBuffer;
	 i2c_m_sync_desc *ptrI2CDescr;
//functions
public:
	MCP23008_Class();
	MCP23008_Class(uint8_t);
	~MCP23008_Class();
	    void Init(i2c_m_sync_desc *i2c);
	    void Init(uint8_t addr);
	    bool Init(void);
		//bool Init(uint8_t addr,I2C_Asyn_Class *i2c);
		bool Init(uint8_t addr,I2C_Sync_Class *i2c);

	    void pinMode(uint8_t p, uint8_t d);
	    void digitalWrite(uint8_t p, uint8_t d);
	    void pullUp(uint8_t p, uint8_t d);
	    uint8_t digitalRead(uint8_t p);

	

	    void setupConfigurationRegister(uint8_t mirroring, uint8_t open, uint8_t polarity);
	    void setupInterruptPin(uint8_t p, uint8_t mode);
	    uint8_t getLastInterruptPin();
	    uint8_t getLastInterruptPinValue();
	    void	SetPortInput(void);
	    void	SetPortOutput(void);
	    uint8_t	SavePorts(void);
	    void SetChangeInterruptAllPins();
		uint8_t ReadGPIORegister(uint8_t add);
		uint8_t ReadGPIORegister(void);
		uint8_t WriteGPIORegister(uint8_t v);
		bool	ReadDigit(uint8_t pin);
		bool	WriteDigit(uint8_t pin, bool b);
		void	SetPortInput(uint8_t inputs);
		void	SetPortOutput(uint8_t out);
		void	SetChangeInterruptPins(uint8_t mask);
protected:
private:
	MCP23008_Class( const MCP23008_Class &c );
	MCP23008_Class& operator=( const MCP23008_Class &c );
	   

	    uint8_t bitForPin(uint8_t pin);
	    uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);

	    uint8_t readRegister(uint8_t addr);
	    uint32_t writeRegister(uint8_t addr, uint8_t value);

	    void updateRegisterBit(uint8_t p, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);
	    bool	SelfTest(void);
	    void ClearIntRegisters(void);

}; //MCP23008_Class

#endif //__MCP23008_CLASS_H__
