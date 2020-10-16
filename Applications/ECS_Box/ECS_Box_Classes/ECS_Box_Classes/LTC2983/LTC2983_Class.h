/* 
* LTC2983_Class.h
*
* Created: 4/23/2020 9:06:37 AM
* Author: GMateusDP
*/


#ifndef __LTC2983_CLASS_H__
#define __LTC2983_CLASS_H__
#include "driver_init.h"
#include "Timer_Class.h"
#include "LTC2983_configuration_constants.h"
#include "LTC2983_table_coeffs.h"
#include "LT_SPI.h"

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

//#define CHIP_SELECT CS_SPI_LTC  // Chip select pin
#define NUMBER_TEMPERATURE_CHANNELS	3
#define TEMPERATURE_MEASSURE_TIMEOUT	1000
#define	LTC2983_SPI_BUFFER_SIZE		7



class LTC2983_Class
{
//variables
public:

	volatile bool	ready,conversionFinished,meassureTimeout;
	uint8_t	activeChannels[NUMBER_TEMPERATURE_CHANNELS];
	uint8_t fault_data;
	uint8_t	channelOnProcess;
	float	meassuredValues[NUMBER_TEMPERATURE_CHANNELS];
	bool	isOK;
	LT_SPI	*spiLT;
	uint32_t output_data;
protected:
private:
	uint8_t *tx, *rx;
	Timer_Class	*auxTimer;
	uint8_t process_finished,data,i,j,k;
	
	spi_m_async_descriptor *SPIA;
	
	float	channelsRawData[NUMBER_TEMPERATURE_CHANNELS];
	uint32_t raw_conversion_result,raw_data,chipSelect;
	float scaled_result;
	int32_t signed_data;
	uint16_t start_address;
	
//functions
public:
	LTC2983_Class();
	LTC2983_Class(spi_m_async_descriptor *);
	~LTC2983_Class();
	bool IsConverterDone();
	bool	Init();
	bool Init(uint32_t csPin,LT_SPI* spiLite);
	float GetSingleChannelResultIndex( uint8_t channel_number);
	void print_title();
	void assign_channel(uint32_t chip_select, uint8_t channel_number, uint32_t channel_assignment_data);
	void write_custom_table(uint32_t chip_select, struct table_coeffs coefficients[64], uint16_t start_address, uint8_t table_length);
	void write_custom_steinhart_hart(uint32_t chip_select, uint32_t steinhart_hart_coeffs[6], uint16_t start_address);

	void measure_channel(uint32_t chip_select, uint8_t channel_number, uint8_t channel_output);
	void convert_channel(uint32_t chip_select, uint8_t channel_number);


	void get_result(uint32_t chip_select, uint8_t channel_number, uint8_t channel_output);

	
	uint32_t	SaveChannelValue(uint8_t);
	

	uint32_t transfer_four_bytes(uint32_t chip_select, uint8_t read_or_write, uint16_t start_address, uint32_t input_data);
	uint8_t transfer_byte(uint32_t chip_select, uint8_t read_or_write, uint16_t start_address, uint8_t input_data);

	uint16_t get_start_address(uint16_t base_address, uint8_t channel_number);
	
	void ConvertAsyncChannelIndex(uint8_t channel_index);
	void ConvertResult(uint32_t raw_conversion_result,uint8_t channel_number, uint8_t channel_output);
	float GetSingleChannelResult( uint8_t channel_number);
	
protected:
	
private:
	void print_config_channel(uint32_t chip_select, uint8_t channel_number, uint16_t start_address);
	LTC2983_Class( const LTC2983_Class &c );
	LTC2983_Class& operator=( const LTC2983_Class &c );
	bool	SelfTest(void);
	void wait_for_process_to_finish(uint32_t chip_select);
	void print_conversion_result(uint32_t raw_conversion_result, uint8_t channel_output);
	void read_voltage_or_resistance_results(uint32_t chip_select, uint8_t channel_number);
	void print_fault_data(uint8_t fault_byte);
	void configure_channels(void);
	void configure_global_parameters(void);
	bool is_number_in_array(uint8_t number, uint8_t *array, uint8_t array_length);
	uint8_t GetAtiveChannelNumeration(uint8_t chn);
	float GetSingleChannelResult( uint8_t channel_number, uint8_t channel_output);

}; //LTC2983_Class

//extern	LTC2983_Class	temperatures;
#endif //__LTC2983_CLASS_H__
