/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "sensors_config.h"
#include "temperature_sensor_main.h"

struct io_descriptor *usart_edbg_io, *dgi_spi_io;
volatile bool         conversion_done = false;
uint8_t               ADC_result[2];
float                 temperature;

uint8_t DataStreamer_buf[5] = {0x03, 0x00, 0x00, 0x00, 0xFC};

/**
 * Initlize SPI Data Gateway Interface
 */
void SPI_DGI_Init(void)
{
	spi_m_sync_get_io_descriptor(&SPI_0, &dgi_spi_io);
	spi_m_sync_enable(&SPI_0);
}

/**
 * ADC conversion callback
 */
static void convert_cb_ADC(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
	conversion_done = true;
}

/**
 * Initlize ADC async conversion
 */
void ADC_light_init(void)
{
	adc_async_register_callback(&ADC_0, CONF_ADC_CHANNEL, ADC_ASYNC_CONVERT_CB, convert_cb_ADC);
	adc_async_enable_channel(&ADC_0, CONF_ADC_CHANNEL);
	adc_async_start_conversion(&ADC_0);
}

/**
 * Initlize USART EDBG
 */
void USART_EDBG_init(void)
{
	usart_sync_get_io_descriptor(&USART_0, &usart_edbg_io);
	usart_sync_enable(&USART_0);
}

int main(void)
{
	atmel_start_init();
	USART_EDBG_init();
	io_write(usart_edbg_io, (uint8_t *)"Clocks & UART ready!\n", 21);
	ADC_light_init();
	io_write(usart_edbg_io, (uint8_t *)"ADC ready!\n", 11);
	temperature_sensors_init();
	io_write(usart_edbg_io, (uint8_t *)"Temp ready!\n", 12);
	SPI_DGI_Init();
	io_write(usart_edbg_io, (uint8_t *)"SPI ready!\n", 11);

	while (1) {
		delay_ms(100);
		adc_async_start_conversion(&ADC_0);
		while (!conversion_done) {
		}
		adc_async_read_channel(&ADC_0, CONF_ADC_CHANNEL, ADC_result, 2);
		io_write(usart_edbg_io, ADC_result, 2);
		temperature = temperature_sensor_read(TEMPERATURE_SENSOR_0);

		DataStreamer_buf[1] = (uint8_t)temperature;
		DataStreamer_buf[2] = ADC_result[0];
		DataStreamer_buf[3] = ADC_result[1];

		gpio_set_pin_level(DGI_SS, false);
		io_write(dgi_spi_io, DataStreamer_buf, 5);
		gpio_set_pin_level(DGI_SS, true);
	}
}
