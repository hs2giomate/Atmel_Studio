/* 
* LTC2983_Class.cpp
*
* Created: 4/23/2020 9:06:36 AM
* Author: GMateusDP
*/


#include "LTC2983_Class.h"
#include "CDC_Class.h"


LTC2983_Class	*ptrLTCClass;
static void	MeassureTimeout(const struct timer_task *const timer_task){
	ptrLTCClass->meassureTimeout=true;
	asm("nop");
}
static void ConverterReady(void){
	ptrLTCClass->ready=true;
}

// default constructor
LTC2983_Class::LTC2983_Class()
{
} //LTC2983_Class

// default destructor
LTC2983_Class::~LTC2983_Class()
{
} //~LTC2983_Class

LTC2983_Class::LTC2983_Class(spi_m_async_descriptor *SPI_LTC){
	ptrLTCClass=this;
	SPIA=SPI_LTC;
	};

bool LTC2983_Class::Init(){
	rx=spiLT->rxLiteBuffer;
	tx=spiLT->txLiteBuffer;
	chipSelect=CS_TEMP1;
	auxTimer=&hvacTimer;
	activeChannels[0]=4;
	activeChannels[1]=8;
	activeChannels[2]=15;
	spiLT->set_descriptor(SPIA);
	spiLT->init();
	while(!IsConverterDone());
	configure_channels();
	configure_global_parameters();

	ready=true;	
	
	return	SelfTest();
}

bool LTC2983_Class::Init(uint32_t csPin,LT_SPI* spiLite){
	
	ptrLTCClass=this;
	chipSelect=csPin;
	spiLT=spiLite;
	rx=spiLT->rxLiteBuffer;
	tx=spiLT->txLiteBuffer;
	auxTimer=&hvacTimer;
	activeChannels[0]=4;
	activeChannels[1]=8;
	activeChannels[2]=15;
	delay_ms(200);
	temperatureTimer.Init();
	configure_channels();
	configure_global_parameters();
	//while(!IsConverterDone());
	ready=true;
	
	return	SelfTest();
}

void LTC2983_Class::print_title()
{
	usb.print(F("\n******************************************************************\n\r"));
	usb.print(F(" LTC2983 Demonstration Program                      \n\r"));
	usb.print(F("                                                                \n\r"));
	usb.print(F(" This program demonstrates how to read the multi-sensor         \n\r"));
	usb.print(F(" temperature measurement system.                                \n\r"));
	usb.print(F("                                                                \n\r"));
	usb.print(F(" Set the baud rate to 115200 and select the newline terminator. \n\r"));
	usb.print(F("******************************************************************\n\r"));
}

// ***********************
// Program the part
// ***********************
void LTC2983_Class::assign_channel(uint32_t chip_select, uint8_t channel_number, uint32_t channel_assignment_data)
{
	uint16_t start_address = get_start_address(CH_ADDRESS_BASE, channel_number);
	transfer_four_bytes(chip_select, WRITE_TO_RAM, start_address, channel_assignment_data);
	print_config_channel(chip_select,channel_number,start_address);
}


void LTC2983_Class::write_custom_table(uint32_t chip_select, struct table_coeffs coefficients[64], uint16_t start_address, uint8_t table_length)
{
	int8_t i;
	uint32_t coeff;

	gpio_set_pin_level(chip_select,false);  

	spiLT->transfer(WRITE_TO_RAM);
	spiLT->transfer(highByte(start_address));
	spiLT->transfer(lowByte(start_address));

	for (i=0; i< table_length; i++)
	{
		coeff = coefficients[i].measurement;
		spiLT->transfer((uint8_t)(coeff >> 16));
		spiLT->transfer((uint8_t)(coeff >> 8));
		spiLT->transfer((uint8_t)coeff);

		coeff = coefficients[i].temperature;
		spiLT->transfer((uint8_t)(coeff >> 16));
		spiLT->transfer((uint8_t)(coeff >> 8));
		spiLT->transfer((uint8_t)coeff);
	}
	gpio_set_pin_level(chip_select,true) ;
}


void LTC2983_Class::write_custom_steinhart_hart(uint32_t chip_select, uint32_t steinhart_hart_coeffs[6], uint16_t start_address)
{
	int8_t i;
	uint32_t coeff;

	gpio_set_pin_level(chip_select,false); 

	spiLT->transfer(WRITE_TO_RAM);
	spiLT->transfer(highByte(start_address));
	spiLT->transfer(lowByte(start_address));

	for (uint8_t ii = 0; ii < 6; ii++)
	{
		coeff = steinhart_hart_coeffs[ii];
		spiLT->transfer((uint8_t)(coeff >> 24));
		spiLT->transfer((uint8_t)(coeff >> 16));
		spiLT->transfer((uint8_t)(coeff >> 8));
		spiLT->transfer((uint8_t)coeff);
	}
	gpio_set_pin_level(chip_select,true);
}




// *****************
// Measure channel
// *****************
void LTC2983_Class::measure_channel(uint32_t chip_select, uint8_t channel_number, uint8_t channel_output)
{
	convert_channel(chip_select, channel_number);
	delay_ms(1);
	get_result(chip_select, channel_number, channel_output);
}


void LTC2983_Class::convert_channel(uint32_t chip_select, uint8_t channel_number)
{
	// Start conversion
	conversionFinished=0;
	transfer_byte(chip_select, WRITE_TO_RAM, COMMAND_STATUS_REGISTER, CONVERSION_CONTROL_BYTE | channel_number);
	channelOnProcess=channel_number;
	wait_for_process_to_finish(chip_select);
}

void LTC2983_Class::ConvertAsyncChannelIndex( uint8_t channel_index)
{
	// Start conversion
	uint8_t channel_number=activeChannels[channel_index];
	conversionFinished=0;
	ready=false;
	transfer_byte(chipSelect, WRITE_TO_RAM, COMMAND_STATUS_REGISTER, CONVERSION_CONTROL_BYTE | channel_number);
	channelOnProcess=channel_number;
	//wait_for_process_to_finish(chip_select);
}

void LTC2983_Class::wait_for_process_to_finish(uint32_t chip_select)
{
	process_finished = 0;
	uint8_t data;
	//while(!gpio_get_pin_level(INT_LTC2983));
	while ((process_finished == 0)&&(!meassureTimeout))
	{
		data = transfer_byte(chip_select, READ_FROM_RAM, COMMAND_STATUS_REGISTER, 0);
		process_finished  = data & 0x40;
		delay_ms(1);
	}
	conversionFinished=process_finished == 0x40;
}

bool LTC2983_Class::IsConverterDone(void)
{
	process_finished = 0;
	uint8_t data;
	delay_ms(1);
	data = transfer_byte(chipSelect, READ_FROM_RAM, COMMAND_STATUS_REGISTER, 0);
	process_finished  = data & 0x40;
	return process_finished == 0x40;
}

// *********************************
// Get results
// *********************************
void LTC2983_Class::print_config_channel(uint32_t chip_select, uint8_t channel_number, uint16_t start_address)
{
	uint32_t raw_data;
	raw_data = transfer_four_bytes(chip_select, READ_FROM_RAM, start_address, 0);

	usb.print(F("\nChannel Config "));
	usb.println(channel_number);

	// 24 LSB's are conversion result
	
	usb<<"register value: ";
	usb.println(raw_data,HEX);
	
}
void LTC2983_Class::get_result(uint32_t chip_select, uint8_t channel_number, uint8_t channel_output)
{
	start_address = get_start_address(CONVERSION_RESULT_MEMORY_BASE, channel_number);
	raw_data = transfer_four_bytes(chip_select, READ_FROM_RAM, start_address, 0);

	usb.print(F("\nChannel "));
	usb.println(channel_number);

	// 24 LSB's are conversion result
	raw_conversion_result = raw_data & 0xFFFFFF;
	print_conversion_result(raw_conversion_result, channel_output);

	// If you're interested in the raw voltage or resistance, use the following
	if (channel_output != VOLTAGE)
	{
		read_voltage_or_resistance_results(chip_select, channel_number);
	}

	// 8 MSB's show the fault data
	fault_data = raw_data >> 24;
	print_fault_data(fault_data);
}

float LTC2983_Class::GetSingleChannelResult( uint8_t channel_number, uint8_t channel_output)
{
	start_address = get_start_address(CONVERSION_RESULT_MEMORY_BASE, channel_number);
	raw_data = transfer_four_bytes(chipSelect, READ_FROM_RAM, start_address, 0);


	// 24 LSB's are conversion result
	raw_conversion_result = raw_data & 0xFFFFFF;
	ConvertResult(raw_conversion_result, channel_number, channel_output);

	// If you're interested in the raw voltage or resistance, use the following
	if (channel_output != VOLTAGE)
	{
		//read_voltage_or_resistance_results(chipSelect, channel_number);
	}

	// 8 MSB's show the fault data
	fault_data = raw_data >> 24;
	return scaled_result;
}

float LTC2983_Class::GetSingleChannelResultIndex( uint8_t channel_index){
	return GetSingleChannelResult(activeChannels[channel_index]);
}

float LTC2983_Class::GetSingleChannelResult( uint8_t channel_number){
	
	return GetSingleChannelResult(channel_number,TEMPERATURE);
}
uint32_t	LTC2983_Class::SaveChannelValue(uint8_t ch){
		start_address = get_start_address(CONVERSION_RESULT_MEMORY_BASE, ch);
		raw_data = transfer_four_bytes(chipSelect, READ_FROM_RAM, start_address, 0);
		channelsRawData[ch]=raw_data;
		return	raw_data;
}


void LTC2983_Class::print_conversion_result(uint32_t raw_conversion_result, uint8_t channel_output)
{
	signed_data = raw_conversion_result;
	// Convert the 24 LSB's into a signed 32-bit integer
	if(signed_data & 0x800000)
		signed_data = signed_data | 0xFF000000;

	// Translate and print result
	if (channel_output == TEMPERATURE)
	{
		scaled_result = float(signed_data) / 1024;
		usb.print(F("  Temperature = "));
		usb.println(scaled_result);
	}
	else if (channel_output == VOLTAGE)
	{
		scaled_result = float(signed_data) / 2097152;
		usb.print(F("  Direct ADC reading in V = "));
		usb.println(scaled_result);
	}
	
}
void LTC2983_Class::ConvertResult(uint32_t raw_conversion_result,uint8_t channel_number, uint8_t channel_output)
{
	signed_data = raw_conversion_result;
	// Convert the 24 LSB's into a signed 32-bit integer
	if(signed_data & 0x800000)
	signed_data = signed_data | 0xFF000000;

	// Translate and print result
	if (channel_output == TEMPERATURE)
	{
		scaled_result= float(signed_data) / 1024;
		meassuredValues[GetAtiveChannelNumeration(channel_number)] = scaled_result;

	}
	else if (channel_output == VOLTAGE)
	{
		scaled_result = float(signed_data) / 2097152;

	}
	
}
uint8_t LTC2983_Class::GetAtiveChannelNumeration(uint8_t chn){
	for (uint8_t ii = 0; ii < NUMBER_TEMPERATURE_CHANNELS; ii++)
	{
		if (activeChannels[ii]==chn)
		{
			return ii;
		}
	}
	return 0;
}


void LTC2983_Class::read_voltage_or_resistance_results(uint32_t chip_select, uint8_t channel_number)
{
	int32_t raw_data;
	float voltage_or_resistance_result;
	uint16_t start_address = get_start_address(VOUT_CH_BASE, channel_number);

	raw_data = transfer_four_bytes(chip_select, READ_FROM_RAM, start_address, 0);
	voltage_or_resistance_result = (float)raw_data/1024;
	usb.print(F("  Voltage or resistance = "));
	usb.println(voltage_or_resistance_result);
}


// Translate the fault byte into usable fault data and print it out
void LTC2983_Class::print_fault_data(uint8_t fault_byte)
{
	//
	usb.print(F("  FAULT DATA = "));
	usb.println(fault_byte, BIN);

	if (fault_byte & SENSOR_HARD_FAILURE)
	usb.println(F("  - SENSOR HARD FALURE"));
	if (fault_byte & ADC_HARD_FAILURE)
	usb.println(F("  - ADC_HARD_FAILURE"));
	if (fault_byte & CJ_HARD_FAILURE)
	usb.println(F("  - CJ_HARD_FAILURE"));
	if (fault_byte & CJ_SOFT_FAILURE)
	usb.println(F("  - CJ_SOFT_FAILURE"));
	if (fault_byte & SENSOR_ABOVE)
	usb.println(F("  - SENSOR_ABOVE"));
	if (fault_byte & SENSOR_BELOW)
	usb.println(F("  - SENSOR_BELOW"));
	if (fault_byte & ADC_RANGE_ERROR)
	usb.println(F("  - ADC_RANGE_ERROR"));
	if (!(fault_byte & VALID_TEMPERATURE))
	usb.println(F("INVALID READING !!!!!!"));
	if (fault_byte == 0b11111111)
	usb.println(F("CONFIGURATION ERROR !!!!!!"));
}

// *********************
// SPI RAM data transfer
// *********************
// To write to the RAM, set ram_read_or_write = WRITE_TO_RAM.
// To read from the RAM, set ram_read_or_write = READ_FROM_RAM.
// input_data is the data to send into the RAM. If you are reading from the part, set input_data = 0.

uint32_t LTC2983_Class::transfer_four_bytes(uint32_t chip_select, uint8_t ram_read_or_write, uint16_t start_address, uint32_t input_data)
{

	uint8_t i;
	
	for (i = 0; i < LTC2983_SPI_BUFFER_SIZE; i++)
	{
		rx[i]=0;
	}

	tx[0] = ram_read_or_write;
	tx[1] = highByte(start_address);
	tx[2] = lowByte(start_address);
	tx[3] = (uint8_t)(input_data >> 24);
	tx[4] = (uint8_t)(input_data >> 16);
	tx[5] = (uint8_t)(input_data >> 8);
	tx[6] = (uint8_t) input_data;

	spiLT->spi_transfer_block(chip_select,tx,rx, LTC2983_SPI_BUFFER_SIZE);

	output_data = (uint32_t) rx[0] << 24 |
	(uint32_t) rx[1] << 16 |
	(uint32_t) rx[2] << 8  |
	(uint32_t) rx[3];
	asm("nop");
	return output_data;
}


uint8_t LTC2983_Class::transfer_byte(uint32_t chip_select, uint8_t ram_read_or_write, uint16_t start_address, uint8_t input_data)
{

	tx[0] = ram_read_or_write;
	tx[1] = (uint8_t)(start_address >> 8);
	tx[2] = (uint8_t)start_address;
	tx[3] = input_data;
	spiLT->spi_transfer_block(chip_select,tx, rx, 4);
	return rx[0];
}


// ******************************
// Misc support functions
// ******************************
uint16_t LTC2983_Class::get_start_address(uint16_t base_address, uint8_t channel_number)
{
	return base_address + 4 * (channel_number-1);
}


bool LTC2983_Class::is_number_in_array(uint8_t number, uint8_t *array, uint8_t array_length)
// Find out if a number is an element in an array
{
	bool found = false;
	for (uint8_t i=0; i< array_length; i++)
	{
		if (number == array[i])
		{
			found = true;
		}
	}
	return found;
}

void LTC2983_Class::configure_channels()
{
	
	uint32_t channel_assignment_data;

	// ----- Channel 2: Assign Sense Resistor -----
	channel_assignment_data =
	SENSOR_TYPE__SENSE_RESISTOR |
	(uint32_t) 0x9C4000 << SENSE_RESISTOR_VALUE_LSB;		// sense resistor - value: 10000.
	assign_channel(chipSelect, 2, channel_assignment_data);
	// ----- Channel 4: Assign RTD PT-100 -----
	channel_assignment_data =
	SENSOR_TYPE__RTD_PT_100 |
	RTD_RSENSE_CHANNEL__2 |
	RTD_NUM_WIRES__2_WIRE |
	RTD_EXCITATION_MODE__NO_ROTATION_SHARING |
	RTD_EXCITATION_CURRENT__50UA |
	RTD_STANDARD__AMERICAN;
	assign_channel(chipSelect, 4, channel_assignment_data);
	// ----- Channel 6: Assign Sense Resistor -----
	channel_assignment_data =
	SENSOR_TYPE__SENSE_RESISTOR |
	(uint32_t) 0x9C4000 << SENSE_RESISTOR_VALUE_LSB;		// sense resistor - value: 10000.
	assign_channel(chipSelect, 6, channel_assignment_data);
	// ----- Channel 8: Assign RTD PT-100 -----
	channel_assignment_data =
	SENSOR_TYPE__RTD_PT_100 |
	RTD_RSENSE_CHANNEL__6 |
	RTD_NUM_WIRES__4_WIRE |
	RTD_EXCITATION_MODE__ROTATION_SHARING |
	RTD_EXCITATION_CURRENT__50UA |
	RTD_STANDARD__AMERICAN;
	assign_channel(chipSelect, 8, channel_assignment_data);
	// ----- Channel 13: Assign Sense Resistor -----
	channel_assignment_data =
	SENSOR_TYPE__SENSE_RESISTOR |
	(uint32_t) 0x9C4000 << SENSE_RESISTOR_VALUE_LSB;		// sense resistor - value: 10000.
	assign_channel(chipSelect, 13, channel_assignment_data);
	// ----- Channel 15: Assign RTD PT-100 -----
	channel_assignment_data =
	SENSOR_TYPE__RTD_PT_100 |
	RTD_RSENSE_CHANNEL__13 |
	RTD_NUM_WIRES__2_WIRE |
	RTD_EXCITATION_MODE__NO_ROTATION_SHARING |
	RTD_EXCITATION_CURRENT__50UA |
	RTD_STANDARD__AMERICAN;
	assign_channel(chipSelect, 15, channel_assignment_data);

}




void LTC2983_Class::configure_global_parameters()
{
	// -- Set global parameters
	transfer_byte(chipSelect, WRITE_TO_RAM, 0xF0, TEMP_UNIT__C |
	REJECTION__50_60_HZ);
	// -- Set any extra delay between conversions (in this case, 0*100us)
	transfer_byte(chipSelect, WRITE_TO_RAM, 0xFF, 0);
}
bool	LTC2983_Class::SelfTest(void){
	process_finished = 0;

	meassureTimeout=false;
	ptrLTCClass=this;	
	temperatureTimer.Start_oneShot_task((FUNC_PTR)MeassureTimeout,TEMPERATURE_MEASSURE_TIMEOUT);

		
	while((fault_data!=VALID_TEMPERATURE)&&(!meassureTimeout)){

		ConvertAsyncChannelIndex(0);
		delay_ms(1);
		//while(!IsConverterDone());
		while((!ready)&&(!meassureTimeout));
		if (!meassureTimeout)
		{
			
			if (ready)
			{
			GetSingleChannelResult(activeChannels[0]);
	
			}else{
				delay_ms(1);
			}

		}else
		{
			//delay_ms(1);
		}

	}
	temperatureTimer.Remove_task(FUNC_PTR(MeassureTimeout));
	

	isOK=fault_data==VALID_TEMPERATURE;
	return	isOK;
	}

//LTC2983_Class	temperatures(&SPI_TEMP);


