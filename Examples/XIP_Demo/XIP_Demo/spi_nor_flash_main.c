/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "spi_nor_flash_main.h"
#include "conf_spi_nor_flash.h"

static struct n25q256a SPI_NOR_FLASH_0_descr;

struct spi_nor_flash *SPI_NOR_FLASH_0;

void QSPI_INSTANCE_exit_xip(void)
{
	gpio_set_pin_function(PA08, 0);
	gpio_set_pin_function(PB11, 0);
	gpio_set_pin_function(PB10, 0);

	gpio_set_pin_direction(PA08, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PB11, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PB10, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA08, true);
	gpio_set_pin_level(PB11, false);
	gpio_set_pin_level(PB10, false);

	delay_us(1);

	for (int i = 0; i < 7; i++) {
		gpio_set_pin_level(PB10, true);
		delay_us(1);
		gpio_set_pin_level(PB10, false);
		delay_us(1);
	}

	gpio_set_pin_level(PB11, true);
	delay_us(1);
	QSPI_INSTANCE_PORT_init();
}

/**
 * \brief Initialize Temperature Sensors
 */
void spi_nor_flash_init(void)
{

	qspi_sync_enable(&QSPI_INSTANCE);
	SPI_NOR_FLASH_0 = n25q256a_construct(
	    &SPI_NOR_FLASH_0_descr.parent, &QSPI_INSTANCE, QSPI_INSTANCE_exit_xip, CONF_SPI_NOR_FLASH_0_QUAD_MODE);
}
