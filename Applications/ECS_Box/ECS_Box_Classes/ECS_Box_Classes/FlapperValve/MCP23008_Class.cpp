/* 
* MCP23008_Class.cpp
*
* Created: 10/2/2020 6:08:21 PM
* Author: GMateusDP
*/


#include "MCP23008_Class.h"

MCP23008_Class::MCP23008_Class()
{
	
} //MCP23008_Class
// default constructor
MCP23008_Class::MCP23008_Class(uint8_t add)
{
	i2cAddress=add;
} //MCP23008_Class

// default destructor
MCP23008_Class::~MCP23008_Class()
{
} //~MCP23008_Class

void MCP23008_Class::Init(uint8_t addr) {
	i2cAddress=addr;
	//i2ca.Set_descriptor(ptrI2CDescr);
	//isReady=i2ca.Init(addr)==0;
	//ext_irq_register(PIN_PA04,FUNC_PTR(MCPHadChanged));
	//ext_irq_register(PIN_PA05,FUNC_PTR(MCPHadChanged));
}
bool MCP23008_Class::Init(uint8_t addr,I2C_Asyn_Class *i2c){
	txBuffer=i2c->txBuffer;
	rxBuffer=i2c->rxBuffer;
	i2cAddress=addr;
	//i2cClass=i2c;
	isOK=i2cClass->isOK;
	return isOK;
	//ext_irq_register(PIN_PA05,FUNC_PTR(MCPHadChanged));
}
bool MCP23008_Class::Init(uint8_t addr,I2C_Sync_Class *i2c){
	txBuffer=i2c->txBuffer;
	rxBuffer=i2c->rxBuffer;
	i2cAddress=addr;
	i2cClass=i2c;
	isOK=i2cClass->isOK;
	return isOK;
	//ext_irq_register(PIN_PA05,FUNC_PTR(MCPHadChanged));
}
void MCP23008_Class::Init(i2c_m_async_desc *i2c){
	ptrI2CDescr=i2c;
	Init((uint8_t)MCP23008_ADDRESS);
}

/**
 * Initializes the default MCP23017, with 000 for the configurable part of the address
 */
bool MCP23008_Class::Init(void) {
    Init((uint8_t)MCP23008_ADDRESS);
	return	SelfTest();	
}
/**
 * Bit number associated to a give Pin
 */
uint8_t MCP23008_Class::bitForPin(uint8_t pin){
    return pin%8;
}


/**
 * Register address, port dependent, for a given PIN
 */
uint8_t MCP23008_Class::regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
    return (pin<8)?portAaddr:portBaddr;
}


/**
 * Reads a given register
 */
uint8_t MCP23008_Class::readRegister(uint8_t addr){
	// read the current GPINTEN
	//i2ca.read_cmd(addr,&value);
	i2cClass->SetSlaveAddress(i2cAddress);
	i2cClass->Write(&addr,1);

	i2cClass->Read(&registerValue,1);

	return registerValue;
}
uint8_t MCP23008_Class::ReadGPIORegister(uint8_t add){
	i2cClass->SetSlaveAddress(i2cAddress);
	return readRegister(MCP23008_GPIOA);
}
uint8_t MCP23008_Class::ReadGPIORegister(void){
	i2cClass->SetSlaveAddress(i2cAddress);
	return readRegister(MCP23008_GPIOA);
}


/**
 * Writes a given register
 */
uint32_t MCP23008_Class::writeRegister(uint8_t addr, uint8_t value){
	// Write the register
	i2cClass->SetSlaveAddress(i2cAddress);
	uint8_t array[2];
	array[0]=addr;
	array[1]=value;
	return	i2cClass->Write(array,2);
	//while(!(i2cClass->txReady));
}
uint8_t MCP23008_Class::WriteGPIORegister(uint8_t v){
	i2cClass->SetSlaveAddress(i2cAddress);
	writeRegister(MCP23008_GPIOA,v);
	return v;
}


void	MCP23008_Class::SetPortInput(void){
	for (uint8_t i=0;i<8;i++)
	{
		pinMode(i,MCP23008_INPUT);
		pullUp(i,MCP23008_HIGH);
	}
}
void	MCP23008_Class::SetPortInput(uint8_t inputs){
	if (inputs>0)
	{
		for (uint8_t i=0;i<8;i++)
		{
			if (inputs&(0x01<<i))
			{
				pinMode(i,MCP23008_INPUT);
				pullUp(i,MCP23008_HIGH);
			}else{
				pinMode(i,MCP23008_OUTPUT);
			}

		}
	}
	else
	{
		SetPortOutput();
	}
	
}
void	MCP23008_Class::SetPortOutput(void){
	for (uint8_t i=0;i<8;i++)
	{
		pinMode(i,MCP23008_OUTPUT);
	}
}
/**
 * Helper to update a single bit of an A/B register.
 * - Reads the current register value
 * - Writes the new register value
 */
void MCP23008_Class::updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
	uint8_t regValue;
	uint8_t regAddr=portAaddr;
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
void MCP23008_Class::pinMode(uint8_t p, uint8_t d) {
    updateRegisterBit(p,(d==MCP23008_INPUT),MCP23008_IODIRA,MCP23008_IODIRA);
}

/**
 * Reads all 16 pins (port A and B) into a single 16 bits variable.
 */

/**
 * Read a single port, A or B, and return its current 8 bit value.
 * Parameter b should be 0 for GPIOA, and 1 for GPIOB.
 */


/**
 * Writes all the pins in one go. This method is very useful if you are implementing a multiplexed matrix and want to get a decent refresh rate.
 */


void MCP23008_Class::digitalWrite(uint8_t pin, uint8_t d) {
	uint8_t gpio;
	uint8_t bit=bitForPin(pin);


	// read the current GPIO output latches
	uint8_t regAddr=regForPin(pin,MCP23008_OLATA,MCP23008_OLATA);
	gpio = readRegister(regAddr);

	// set the pin and direction
	bitWrite(gpio,bit,d);

	// write the new GPIO
	regAddr=regForPin(pin,MCP23008_GPIOA,MCP23008_GPIOA);
	writeRegister(regAddr,gpio);
}
bool MCP23008_Class::WriteDigit(uint8_t pin, bool b) {
	uint8_t gpio;
	uint8_t bit=bitForPin(pin);


	// read the current GPIO output latches
	uint8_t regAddr=regForPin(pin,MCP23008_OLATA,MCP23008_OLATA);
	gpio = readRegister(regAddr);

	// set the pin and direction
	bitWrite(gpio,bit,b);

	// write the new GPIO
	regAddr=regForPin(pin,MCP23008_GPIOA,MCP23008_GPIOA);
	return	writeRegister(regAddr,gpio)==0;
}

void MCP23008_Class::pullUp(uint8_t p, uint8_t d) {
	updateRegisterBit(p,d,MCP23008_GPPUA,MCP23008_GPPUA);
}

uint8_t MCP23008_Class::digitalRead(uint8_t pin) {
	uint8_t bit=bitForPin(pin);
	uint8_t regAddr=regForPin(pin,MCP23008_GPIOA,MCP23008_GPIOA);
	return (readRegister(regAddr) >> bit) & 0x1;
}
bool MCP23008_Class::ReadDigit(uint8_t pin) {
	uint8_t bit=bitForPin(pin);
	uint8_t regAddr=regForPin(pin,MCP23008_GPIOA,MCP23008_GPIOA);
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
void MCP23008_Class::setupInterrupts(uint8_t mirroring, uint8_t openDrain, uint8_t polarity){
	// configure the port A
	uint8_t ioconfValue=readRegister(MCP23008_IOCONA);
	bitWrite(ioconfValue,6,mirroring);
	bitWrite(ioconfValue,2,openDrain);
	bitWrite(ioconfValue,1,polarity);
	writeRegister(MCP23008_IOCONA,ioconfValue);


}

/**
 * Set's up a pin for interrupt. uses arduino MODEs: CHANGE, FALLING, RISING.
 *
 * Note that the interrupt condition finishes when you read the information about the port / value
 * that caused the interrupt or you read the port itself. Check the datasheet can be confusing.
 *
 */
void MCP23008_Class::setupInterruptPin(uint8_t pin, uint8_t mode) {

	// set the pin interrupt control (0 means change, 1 means compare against given value);
	updateRegisterBit(pin,(mode!=MCP23008_CHANGE),MCP23008_INTCONA,MCP23008_INTCONA);
	// if the mode is not CHANGE, we need to set up a default value, different value triggers interrupt

	// In a RISING interrupt the default value is 0, interrupt is triggered when the pin goes to 1.
	// In a FALLING interrupt the default value is 1, interrupt is triggered when pin goes to 0.
	updateRegisterBit(pin,(mode==MCP23008_FALLING),MCP23008_DEFVALA,MCP23008_DEFVALA);

	// enable the pin for interrupt
	updateRegisterBit(pin,MCP23008_HIGH,MCP23008_GPINTENA,MCP23008_GPINTENA);

}

void MCP23008_Class::SetChangeInterruptAllPins(){
	setupInterrupts(false,false,MCP23008_HIGH);
	for (uint8_t ii = 0; ii <MCP23008_NUMBER_PINS ; ii++)
	{
		setupInterruptPin(ii,MCP23008_CHANGE);
	}
}

uint8_t MCP23008_Class::getLastInterruptPin(){
	uint8_t intf;

	// try port A
	intf=readRegister(MCP23008_INTFA);
	for(int i=0;i<8;i++) if (bitRead(intf,i)) return i;



	return MCP23008_INT_ERR;

}
uint8_t	MCP23008_Class::SavePorts(void){

	if (hasChanged)
	{
		ClearIntRegisters();
	}
	return portA;
}
void MCP23008_Class::ClearIntRegisters(){
		readRegister(MCP23008_INTCAPA);

}
uint8_t MCP23008_Class::getLastInterruptPinValue(){
	uint8_t intPin=getLastInterruptPin();
	if(intPin!=MCP23008_INT_ERR){
		uint8_t intcapreg=regForPin(intPin,MCP23008_INTCAPA,MCP23008_INTCAPA);
		uint8_t bit=bitForPin(intPin);
		return (readRegister(intcapreg)>>bit) & (0x01);
	}

	return MCP23008_INT_ERR;
}
bool	MCP23008_Class::SelfTest(void){

	return isOK;
}