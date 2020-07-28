/* 
* LTC2983_Class.h
*
* Created: 4/23/2020 9:06:37 AM
* Author: GMateusDP
*/


#ifndef __LTC2983_CLASS_H__
#define __LTC2983_CLASS_H__
#include "driver_init.h"
#include "LTC2983_configuration_constants.h"
#include "LTC2983_table_coeffs.h"
#include "LT_SPI.h"
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define CHIP_SELECT CS_SPI_LTC  // Chip select pin

class LTC2983_Class
{
//variables
public:
	LT_SPI	spiLT;
	volatile bool	ready;
	spi_m_async_descriptor *SPIA;
protected:
private:
	
//functions
public:
	LTC2983_Class();
	LTC2983_Class(spi_m_async_descriptor *);
	~LTC2983_Class();
	void	init();
	void print_title();
	void assign_channel(uint8_t chip_select, uint8_t channel_number, uint32_t channel_assignment_data);
	void write_custom_table(uint8_t chip_select, struct table_coeffs coefficients[64], uint16_t start_address, uint8_t table_length);
	void write_custom_steinhart_hart(uint8_t chip_select, uint32_t steinhart_hart_coeffs[6], uint16_t start_address);

	void measure_channel(uint8_t chip_select, uint8_t channel_number, uint8_t channel_output);
	void convert_channel(uint8_t chip_select, uint8_t channel_number);
	void wait_for_process_to_finish(uint8_t chip_select);

	void get_result(uint8_t chip_select, uint8_t channel_number, uint8_t channel_output);
	void print_conversion_result(uint32_t raw_conversion_result, uint8_t channel_output);
	void read_voltage_or_resistance_results(uint8_t chip_select, uint8_t channel_number);
	void print_fault_data(uint8_t fault_byte);
	void configure_channels(void);
	void configure_global_parameters(void);
	

	uint32_t transfer_four_bytes(uint8_t chip_select, uint8_t read_or_write, uint16_t start_address, uint32_t input_data);
	uint8_t transfer_byte(uint8_t chip_select, uint8_t read_or_write, uint16_t start_address, uint8_t input_data);

	uint16_t get_start_address(uint16_t base_address, uint8_t channel_number);
	bool is_number_in_array(uint8_t number, uint8_t *array, uint8_t array_length);
	
protected:
	
private:
	void print_config_channel(uint8_t chip_select, uint8_t channel_number, uint16_t start_address);
	LTC2983_Class( const LTC2983_Class &c );
	LTC2983_Class& operator=( const LTC2983_Class &c );

}; //LTC2983_Class

//extern	LTC2983_Class	LTC;
#endif //__LTC2983_CLASS_H__
