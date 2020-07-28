/**
 * \file
 *
 * \brief QSPI DMA Application
 *
 * Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
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
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
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

#include <atmel_start.h>
#include "spi_nor_flash_main.h"

#define BUFFER_SIZE 512
/** Size of minimum erase block */
#define QSPI_ERBLK (4 * 1024)
/** Size of data to erase (blocks to cover writing area) */
#define QSPI_ERSIZE ((BUFFER_SIZE + QSPI_ERBLK - 1) & ~(QSPI_ERBLK - 1))

/* Declare  Tx and Rx buffer and initialize to 0 */
uint8_t tx_buffer[BUFFER_SIZE] = {0};
uint8_t rx_buffer[BUFFER_SIZE] = {0};

static struct n25q256a SPI_NOR_FLASH_0_descr;

struct spi_nor_flash *SPI_NOR_FLASH_0;

void QSPI_INSTANCE_exit_xip(void)
{
	gpio_set_pin_function(FLASH_IO0, 0);
	gpio_set_pin_function(FLASH_CS, 0);
	gpio_set_pin_function(FLASH_CLK, 0);

	gpio_set_pin_direction(FLASH_IO0, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(FLASH_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(FLASH_CLK, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(FLASH_IO0, true);
	gpio_set_pin_level(FLASH_CS, false);
	gpio_set_pin_level(FLASH_CLK, false);

	delay_us(1);

	for (int i = 0; i < 7; i++) {
		gpio_set_pin_level(FLASH_CLK, true);
		delay_us(1);
		gpio_set_pin_level(FLASH_CLK, false);
		delay_us(1);
	}

	gpio_set_pin_level(FLASH_CS, true);
	delay_us(1);
	QSPI_INSTANCE_PORT_init();
}

/**
 * \brief Initialize NOR Flash memory
 */
void spi_nor_flash_init(void)
{

	qspi_dma_enable(&QSPI_INSTANCE);
	SPI_NOR_FLASH_0 = n25q256a_construct(
	    &SPI_NOR_FLASH_0_descr.parent, &QSPI_INSTANCE, QSPI_INSTANCE_exit_xip, CONF_SPI_NOR_FLASH_0_QUAD_MODE);
}

/* DMA Transfer complete callback */
static void qspi_xfer_complete_cb(struct _dma_resource *resource)
{
	/* Pull Up Chip select line*/
	hri_qspi_write_CTRLA_reg(QSPI, QSPI_CTRLA_ENABLE | QSPI_CTRLA_LASTXFER);
}

int main(void)
{
	bool is_corrupted = false;

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Register DMA complete Callback and initialize NOR flash */
	qspi_dma_register_callback(&QSPI_INSTANCE, QSPI_DMA_CB_XFER_DONE, qspi_xfer_complete_cb);
	spi_nor_flash_init();

	printf("QSPI Program Started\n\r");
	/* Initialize Tx buffer */
	for (int i = 0; i < BUFFER_SIZE; i++) {
		tx_buffer[i] = (uint8_t)i;
	}

	/* Erase flash memory */
	if (ERR_NONE == SPI_NOR_FLASH_0->interface->erase(SPI_NOR_FLASH_0, 0, QSPI_ERSIZE)) {
		printf("Flash erase successful\n\r");
	}

	/* Write data to flash memory */
	if (ERR_NONE == SPI_NOR_FLASH_0->interface->write(SPI_NOR_FLASH_0, (uint8_t *)tx_buffer, 0, BUFFER_SIZE)) {
		printf("Flash write successful \n\r");
	}

	/* Read data from flash memory */
	if (ERR_NONE == SPI_NOR_FLASH_0->interface->read(SPI_NOR_FLASH_0, (uint8_t *)rx_buffer, 0, BUFFER_SIZE)) {
		printf("Flash read successful\n\r");
	}

	/* Data verification */
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (tx_buffer[i] != rx_buffer[i]) {
			is_corrupted = true;
			printf("Flash data verification failed.\n\r");
		}
	}

	if (!is_corrupted) {
		printf("Write - Read is successful in QSPI Flash memory.\n\r");
	}

	while (1) {
	}
}
