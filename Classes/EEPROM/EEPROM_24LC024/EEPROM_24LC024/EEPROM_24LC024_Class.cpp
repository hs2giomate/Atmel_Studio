/* 
* EEPROM_24LC024_Class.cpp
*
* Created: 4/29/2020 8:54:46 AM
* Author: GMateusDP
*/


#include "EEPROM_24LC024_Class.h"
EEPROM_24LC024_Class	*ptrEEPROM;

I2C_Asyn_Class EEPROM_24LC024_Class::i2c;
// default constructor
EEPROM_24LC024_Class::EEPROM_24LC024_Class()
{
	I2C_A=I2C_EEPROM;
	ptrEEPROM=this;
} //EEPROM_24LC024_Class

EEPROM_24LC024_Class::EEPROM_24LC024_Class(i2c_m_async_desc *i2c_a)
{
	I2C_A=*i2c_a;
	ptrEEPROM=this;
} //I2C_Asyn_Class

// default destructor
EEPROM_24LC024_Class::~EEPROM_24LC024_Class()
{
	i2c.disable();
} //~EEPROM_24LC024_Class

void	EEPROM_24LC024_Class::init(){
	i2c.set_descriptor(&I2C_A);
	i2c.init();
	i2c.set_slaveaddr(EEPROM_I2C_ADDRESS_SHIFT);
	usb.init();
}


uint8_t EEPROM_24LC024_Class::read_id_string(char *buffer){
	uint8_t i, j;                // Iteration variables
	uint8_t comma_position[8] = {0};  // Contains the position of the commas in the buffer
	uint8_t comma_count = 0;      // The number of commas found in the buffer
	uint8_t buffer_count;       // The number of characters read
	int8_t option;              // Temporary demo board option
	// read the id string from the demo board EEPROM
	// starting EEPROM address=0, terminator=0x0a, max buffer length=52 bytes
	// Enable I2C
	//pinMode(QUIKEVAL_MUX_MODE_PIN, OUTPUT);   // Configure MUX to disconnect I2C pins on QuikEval connector. Remember EEPROM has its own I2C pins on the the connector.
	//int8_t QUIKEVAL_MUX_MODE_PIN_state;
	//QUIKEVAL_MUX_MODE_PIN_state = digitalRead(QUIKEVAL_MUX_MODE_PIN);
	//digitalWrite(QUIKEVAL_MUX_MODE_PIN, LOW);
	buffer_count = read_buffer_with_terminator(EEPROM_I2C_ADDRESS, buffer, 0, QUIKEVAL_ID_TERMINATOR, QUIKEVAL_ID_SIZE+2);
	//digitalWrite(QUIKEVAL_MUX_MODE_PIN, QUIKEVAL_MUX_MODE_PIN_state);  // Restore the QUIKEVAL_MUX_MODE_PIN (8) to its previous state.  (But, it will be left as output.)
	if (buffer_count == 0) return(0);   // quit if no data read
	// find comma positions
	for (i = 0; i < buffer_count; i++)
	{
		if (buffer[i] == ',') comma_position[comma_count++]=i;
	}

	if (comma_position[6] < comma_position[5])// comma_position[6]=strlen(buffer);  // some demo boards are missing the last comma
	{
		for (i = buffer_count - 2; i > comma_position[5]; i--)
		{
			if (buffer[i] != '-')
			{
				comma_position[6] = i+1;
				break;
			}
		}
	}



	// get product name. All characters before the 1st comma.
	for (i = 0; i < comma_position[0]; i++)
	{
		exprimental_board.product_name[i]=buffer[i];
	}
	exprimental_board.product_name[i]='\0';  // add terminator
	// Get demo board name between the 5th and 6th comma
	j = 0;

	for (i = comma_position[5]+1; i < comma_position[6]; i++)
	{
		exprimental_board.name[j++]=buffer[i];
	}
	exprimental_board.name[j]='\0';  // add terminator
	// get demo board option from the demo board name
	for (i = 0; i < strlen(exprimental_board.name); i++)
	{
		if (exprimental_board.name[i] == '-') option = exprimental_board.name[i+1];
	}
	if ((option >= 0x41) && (option <= 0x5A)) exprimental_board.option = option;
	// final demo board name is 6 characters without the option
	exprimental_board.name[6]='\0';
	return(buffer_count);
}

uint8_t  EEPROM_24LC024_Class::read_buffer_with_terminator(uint8_t i2c_address, char *buffer, uint16_t address_data, char terminator, uint8_t count)
{
	uint8_t i = 0;
	int32_t ack=0;
	uint8_t data,byte;
	char	*refBuffer=buffer;
	uint8_t	test[]="por que no funciona!";
	uint8_t	read_test[16];
	*buffer='\0';  
	refBuffer=(char*)(&read_test[0]);                          // Initialize buffer with null
	if (eeprom_poll(i2c_address) == 1)      // Check if the EEPROM is ready
	
	{
		//i2c.send_stop(); 
		if (EEPROM_DATA_SIZE > 0x100){
			byte=address_data>>8;
			i2c.write(&byte,1);
		}           // Send upper byte of address if size > 256 bytes
		byte=i2c_address | I2C_WRITE_BIT;
			i2c.write(&byte,1); 
		   // I2C address + read
	//	for (i = 0; i < 16-1; i++)           // Read count-1 bytes with ACK
	//	{
		
	//		byte=16*i;
		//	i2c.write(&byte,1); 
		//	i2c.write_cmd(byte,16*i);
			i2c.write(&test[0],16);
	//		i2c.send_stop();
	//		ack=i2c.write_ack(&byte,1);;
	//	}
		//i2c.write(&byte,1);
		i2c.send_stop();
		byte=i2c_address | I2C_WRITE_BIT;
		ack=i2c.write_ack(&byte,1);

	
		byte=address_data;
		i2c.write_ack(&byte,1);      // I2C address + read
		//i2c.send_stop();
	//	i2c.write_cmd(i2c_address,(uint8_t)address_data);  
	//	i2c.send_stop();          // Send lower byte of address
		//	i2c_repeated_start();               // I2C repeated start
		
		byte=i2c_address>>1;
		byte |= I2C_READ_BIT;
		//i2c.read_cmd(byte,&read_test[0]);  
		i2c.write(&byte,1);      // I2C address + read
	     // I2C address + read
	
		//i2c.send_stop();   
		for (i = 0; i < 16-1; i++)           // Read count-1 bytes with ACK
		{
			i2c.read(&read_test[0],4);                 // Receive 1 EEPROM data byte with ACK
	//		i2c.read_cmd(byte,&data); 
			               // Receive 1 EEPROM data byte with ACK
			*buffer++=data;                   // Place data byte in buffer and increment pointer
		//	if (data == terminator) break;
		}
		i2c.send_stop();                   // Read one more byte from EEPROM with NACK
		
		i2c.disable();                         // I2C stop
		*buffer = 0;                          // Place null terminator at end of buffer
		return(i+1);
	}
	else
	return(0);
}
int8_t EEPROM_24LC024_Class::eeprom_poll(uint8_t i2c_address)
{
	uint8_t timer_count;
	int8_t ack;
	//i2c.enable();
	uint8_t		byte=i2c_address>>0;                           // I2C start
	byte|=I2C_WRITE_BIT;    
	ack =(int8_t)i2c.write(&byte,1);     // I2C address + !write
	i2c.send_stop();
	if (ack==I2C_NACK)
	{
		// acknowledge not received, enter timer loop
		//for (timer_count = 0; timer_count < EEPROM_TIMEOUT*EEPROM_TIMEOUT; timer_count++)
		while (ack==I2C_NACK)
		{
			//i2c.enable();               // I2C repeated start
			byte=i2c_address | I2C_WRITE_BIT;
			ack =(int8_t)i2c.write_ack(&byte,1);     // I2C address + !write
			if (ack == 1)
			break;                  // If the EEPROM acknowledges, jump out of delay loop
			else
			delay_ms(1);
		}
		if (ack) i2c.disable();  ;                    // Generate a stop if no acknowledge
	}
	return(ack);
}
int8_t  EEPROM_24LC024_Class::discover_demo(char *demo_name)
{
	int8_t connected;
	connected = 1;
	// read the ID from the serial EEPROM on the board
	// reuse the buffer declared in UserInterface
	if (read_id_string(&ui_buffer[0]) == 0) connected = 0;
	// make sure it is the correct demo board
	if (strcmp(exprimental_board.name, demo_name) != 0) connected = 0;
	if (connected != 0)
	{
		usb.xprint("Demo Board Name: ");
		usb.xprint(exprimental_board.name);
		usb.xprint("\r\nProduct Name: ");
		usb.xprint(exprimental_board.product_name);
		if (exprimental_board.option)
		{
			usb.xprint("\r\nDemo Board Option: ");
			usb.xprint(&exprimental_board.option);
		}
	}
	else
	{
		usb.xprint("Demo board ");
		usb.xprint(demo_name);
		usb.xprint(" \r\nnot found, \nfound \r\n");
		usb.xprint(exprimental_board.name);
		usb.xprint(" \r\ninstead. \nConnect the correct demo board, then press the reset button.");
	}
	return(connected);
}
