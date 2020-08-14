#ifndef _MCP23017_H_
#define _MCP23017_H_

#include "driver_init.h"
#include "utils.h"
#include "I2C_Asyn_Class.h"
#include "Arduino.h"
#define		INPUT		0
#define		OUTPUT		1
#define		LOW		0
#define		HIGH		1
#define		CHANGE		2
#define		FALLING		3
//default device address
#define MCP23017_ADDRESS 0x27

//registers (default IOCON.BANK = 0)
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14


#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

#define MCP23017_INT_ERR 255

class MCP23017_Class
 {
//variables
public:
	volatile bool isReady,hasChanged;
private:
	i2c_m_async_desc *ptrI2CDescr;
	I2C_Asyn_Class	i2ca;
	uint8_t		i2c_addr,registerValue,regAddress,i,controlRegisterA,controlRegisterB,portA,portB;
	bool isOK;
	//Funcitons
public:
	MCP23017_Class();
	MCP23017_Class(i2c_m_async_desc *i2c);
	~MCP23017_Class();
    void Init(i2c_m_async_desc *i2c);
	void Init(uint8_t addr);
    bool Init(void);

    void pinMode(uint8_t p, uint8_t d);
    void digitalWrite(uint8_t p, uint8_t d);
    void pullUp(uint8_t p, uint8_t d);
    uint8_t digitalRead(uint8_t p);

    void writeGPIOAB(uint16_t);
    uint16_t readGPIOAB();
    uint8_t readGPIO(uint8_t b);

    void setupInterrupts(uint8_t mirroring, uint8_t open, uint8_t polarity);
    void setupInterruptPin(uint8_t p, uint8_t mode);
    uint8_t getLastInterruptPin();
    uint8_t getLastInterruptPinValue();
	void	SetPortAInput(void);
	void	SetPortBOutput(void);
	uint8_t	SavePorts(void);
	
private:
    uint8_t i2caddr;

    uint8_t bitForPin(uint8_t pin);
    uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);

    uint8_t readRegister(uint8_t addr);
    void writeRegister(uint8_t addr, uint8_t value);

    void updateRegisterBit(uint8_t p, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);
	bool	SelfTest(void);
};

extern	MCP23017_Class	mcp;



#endif
