

#include "MCP23017_Class.h"



MCP23017_Class	*ptrMCP23017Class;

MCP23017_Class::MCP23017_Class()
{
	ptrMCP23017Class=this;
} //AT24MAC_Class
MCP23017_Class::MCP23017_Class(i2c_m_async_desc *i2c)
{
	ptrI2CDescr=i2c;
	ptrMCP23017Class=this;
} //AT24MAC_Class

// default destructor
MCP23017_Class::~MCP23017_Class()
{
} //~AT24MAC_Class

void MCP23017_Class::Init(uint8_t addr) {
	i2c_addr=addr;
	i2ca.Set_descriptor(ptrI2CDescr);
	isReady=i2ca.Init(addr)==0;
}
void MCP23017_Class::Init(i2c_m_async_desc *i2c){
	ptrI2CDescr=i2c;
	Init((uint8_t)MCP23017_ADDRESS);
}

/**
 * Initializes the default MCP23017, with 000 for the configurable part of the address
 */
bool MCP23017_Class::Init(void) {
    Init((uint8_t)MCP23017_ADDRESS);
	return	SelfTest();	
}
/**
 * Bit number associated to a give Pin
 */
uint8_t MCP23017_Class::bitForPin(uint8_t pin){
    return pin%8;
}


/**
 * Register address, port dependent, for a given PIN
 */
uint8_t MCP23017_Class::regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
    return(pin<8) ?portAaddr:portBaddr;
}


/**
 * Reads a given register
 */
uint8_t MCP23017_Class::readRegister(uint8_t addr){
	// read the current GPINTEN
	//i2ca.read_cmd(addr,&value);
	i2ca.Write(&addr,1);
	while(!i2ca.txReady);
	i2ca.Read(&registerValue,1);
	while(!i2ca.rxReady);
	return registerValue;
}


/**
 * Writes a given register
 */
void MCP23017_Class::writeRegister(uint8_t addr, uint8_t value){
	// Write the register
	isReady=false;
	uint8_t array[2];
	array[0]=addr;
	array[1]=value;
	i2ca.Write(array,2);
	while(!i2ca.txReady);
}

void	MCP23017_Class::SetPortAInput(void){
	for (i=0;i<8;i++)
	{
		pinMode(i,INPUT);
		pullUp(i,HIGH);
	}
}
void	MCP23017_Class::SetPortBOutput(void){
	for (i=8;i<16;i++)
	{
		pinMode(i,OUTPUT);
	}
}
/**
 * Helper to update a single bit of an A/B register.
 * - Reads the current register value
 * - Writes the new register value
 */
void MCP23017_Class::updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
	uint8_t regValue;
	uint8_t regAddr=regForPin(pin,portAaddr,portBaddr);
	uint8_t bit=bitForPin(pin);
	regValue = readRegister(regAddr);

	// set the value for the particular bit
	bitWrite(regValue,bit,pValue);

	writeRegister(regAddr,regValue);
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the MCP23017 given its HW selected address, see datasheet for Address selection.
 * the addr is from 0 to 7
 * soldering the A0 to A2 to change the addr
 */


/**
 * Sets the pin mode to either INPUT or OUTPUT
 */
void MCP23017_Class::pinMode(uint8_t p, uint8_t d) {
    updateRegisterBit(p,(d==INPUT),MCP23017_IODIRA,MCP23017_IODIRB);
}

/**
 * Reads all 16 pins (port A and B) into a single 16 bits variable.
 */
uint16_t MCP23017_Class::readGPIOAB() {
	uint16_t ba = 0;
	regAddress=MCP23017_GPIOA;
	i2ca.Write(&regAddress,1);
	while(!i2ca.txReady);
	uint32_t r= i2ca.Read((uint8_t*)&ba,2);
	while(!i2ca.rxReady);
	return r;

	return ba;
}

/**
 * Read a single port, A or B, and return its current 8 bit value.
 * Parameter b should be 0 for GPIOA, and 1 for GPIOB.
 */
uint8_t MCP23017_Class::readGPIO(uint8_t b) {

	if (b == 0){
		regAddress=MCP23017_GPIOA;
		i2ca.Write(&regAddress,1);
	}else {
		regAddress=MCP23017_GPIOB;
		i2ca.Write(&regAddress,1);
	}


		while(!i2ca.txReady);
		i2ca.Read(&registerValue,1);
		while(!i2ca.rxReady);
		return	registerValue;
}

/**
 * Writes all the pins in one go. This method is very useful if you are implementing a multiplexed matrix and want to get a decent refresh rate.
 */
void MCP23017_Class::writeGPIOAB(uint16_t ba) {
	uint8_t array[3];
	isReady=false;
	array[0]=(uint8_t)MCP23017_GPIOA;
	array[1]=(uint8_t)((ba&(0xff00))>>8);
	array[2]=(uint8_t)(ba)&(0xff);
	i2ca.Write(array,3);
	//while(!i2ca.txReady);
}

void MCP23017_Class::digitalWrite(uint8_t pin, uint8_t d) {
	uint8_t gpio;
	uint8_t bit=bitForPin(pin);


	// read the current GPIO output latches
	uint8_t regAddr=regForPin(pin,MCP23017_OLATA,MCP23017_OLATB);
	gpio = readRegister(regAddr);

	// set the pin and direction
	bitWrite(gpio,bit,d);

	// write the new GPIO
	regAddr=regForPin(pin,MCP23017_GPIOA,MCP23017_GPIOB);
	writeRegister(regAddr,gpio);
}

void MCP23017_Class::pullUp(uint8_t p, uint8_t d) {
	updateRegisterBit(p,d,MCP23017_GPPUA,MCP23017_GPPUB);
}

uint8_t MCP23017_Class::digitalRead(uint8_t pin) {
	uint8_t bit=bitForPin(pin);
	uint8_t regAddr=regForPin(pin,MCP23017_GPIOA,MCP23017_GPIOB);
	return (readRegister(regAddr) >> bit) & 0x1;
}

/**
 * Configures the interrupt system. both port A and B are assigned the same configuration.
 * Mirroring will OR both INTA and INTB pins.
 * Opendrain will set the INT pin to value or open drain.
 * polarity will set LOW or HIGH on interrupt.
 * Default values after Power On Reset are: (false, false, LOW)
 * If you are connecting the INTA/B pin to arduino 2/3, you should configure the interupt handling as FALLING with
 * the default configuration.
 */
void MCP23017_Class::setupInterrupts(uint8_t mirroring, uint8_t openDrain, uint8_t polarity){
	// configure the port A
	uint8_t ioconfValue=readRegister(MCP23017_IOCONA);
	bitWrite(ioconfValue,6,mirroring);
	bitWrite(ioconfValue,2,openDrain);
	bitWrite(ioconfValue,1,polarity);
	writeRegister(MCP23017_IOCONA,ioconfValue);

	// Configure the port B
	ioconfValue=readRegister(MCP23017_IOCONB);
	bitWrite(ioconfValue,6,mirroring);
	bitWrite(ioconfValue,2,openDrain);
	bitWrite(ioconfValue,1,polarity);
	writeRegister(MCP23017_IOCONB,ioconfValue);
}

/**
 * Set's up a pin for interrupt. uses arduino MODEs: CHANGE, FALLING, RISING.
 *
 * Note that the interrupt condition finishes when you read the information about the port / value
 * that caused the interrupt or you read the port itself. Check the datasheet can be confusing.
 *
 */
void MCP23017_Class::setupInterruptPin(uint8_t pin, uint8_t mode) {

	// set the pin interrupt control (0 means change, 1 means compare against given value);
	updateRegisterBit(pin,(mode!=CHANGE),MCP23017_INTCONA,MCP23017_INTCONB);
	// if the mode is not CHANGE, we need to set up a default value, different value triggers interrupt

	// In a RISING interrupt the default value is 0, interrupt is triggered when the pin goes to 1.
	// In a FALLING interrupt the default value is 1, interrupt is triggered when pin goes to 0.
	updateRegisterBit(pin,(mode==FALLING),MCP23017_DEFVALA,MCP23017_DEFVALB);

	// enable the pin for interrupt
	updateRegisterBit(pin,HIGH,MCP23017_GPINTENA,MCP23017_GPINTENB);

}

uint8_t MCP23017_Class::getLastInterruptPin(){
	uint8_t intf;

	// try port A
	intf=readRegister(MCP23017_INTFA);
	for(int i=0;i<8;i++) if (bitRead(intf,i)) return i;

	// try port B
	intf=readRegister(MCP23017_INTFB);
	for(int i=0;i<8;i++) if (bitRead(intf,i)) return i+8;

	return MCP23017_INT_ERR;

}
uint8_t	MCP23017_Class::SavePorts(void){
	portA=readGPIO(0);
	portB=readGPIO(1);
}
uint8_t MCP23017_Class::getLastInterruptPinValue(){
	uint8_t intPin=getLastInterruptPin();
	if(intPin!=MCP23017_INT_ERR){
		uint8_t intcapreg=regForPin(intPin,MCP23017_INTCAPA,MCP23017_INTCAPB);
		uint8_t bit=bitForPin(intPin);
		return (readRegister(intcapreg)>>bit) & (0x01);
	}

	return MCP23017_INT_ERR;
}
bool	MCP23017_Class::SelfTest(void){
	controlRegisterA=readRegister(MCP23017_IOCONA);
	controlRegisterB=readRegister(MCP23017_IOCONB);
	if (controlRegisterA==controlRegisterB)
	{
		if (controlRegisterA&(0x80)==0)
		{
			isOK=true;
		} 
		else
		{
			isOK=false;
		}
	} 
	else
	{
		isOK=false;
	}
	return isOK;
}
MCP23017_Class	mcp(&I2C_EXPANDER);