/**
 * \file
 *
 * \brief Application implement
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
#include <string.h>

/* Include macros for clocks' frequencies */
#include "hpl_qspi_config.h"
#include "peripheral_clk_config.h"

/*
 * Include binary function and related macros, including
 * - CONF_EXEC_ADDR:   Address for code execution (XIP area entries)
 * - CONF_EXT_RW_TEST: Compiling option to extend dummy data after executable
 *                     binary for bulk read/write verification.
 * - QSPI_LMADDR:      Load address, binary bytes to load to QSPI Flash
 * - QSPI_LMSIZE:      Load size, in number of bytes
 * - QSPI_ERSIZE:      Erase size, aligned with blocks to cover writing area
 *
 * - delay_ms_t: Type of delay function, invoked as delay_ms(ms)
 * - func_blink_t: Type of blink function, invoked as blink(delay_func_entry)
 * - blink_1: Blink function 1 invoke entry, on XIP area
 * - blink_2: Blink function 2 invoke entry, on XIP area
 */
#include "func_blink.h"

/**
 * Validate QSPI code, return true if identical, otherwise return false with hints printed.
 */
static bool _validate_code(void)
{
	uint32_t  i;
	uint32_t *src = (uint32_t *)QSPI_LMADDR;
	uint32_t *dst = (uint32_t *)CONF_EXEC_ADDR;
	for (i = 0; i < (QSPI_LMSIZE >> 2); i++) {
		if (src[i] != dst[i]) {
			printf("Code not identical @ 0x%x : 0x%x <> 0x%x\r\n", (unsigned)i, (unsigned)src[i], (unsigned)dst[i]);
			return false;
		}
	}
	return true;
}

/**
 * Load QSPI code
 */
static void _load_qspi(void)
{
	uint32_t *src = (uint32_t *)QSPI_LMADDR;
	uint32_t *dst = (uint32_t *)CONF_EXEC_ADDR;

	printf("Load QSPI code: @ 0x%x -> 0x%x, %u bytes, erase %u\r\n",
	       (unsigned)src,
	       (unsigned)dst,
	       (unsigned)QSPI_LMSIZE,
	       (unsigned)QSPI_ERSIZE);

	/* Enable XIP mode, so dsc can be accessed directly. */
	SPI_NOR_FLASH_0->interface->enxip(SPI_NOR_FLASH_0);
	printf("Check QSPI code:\r\n");
	if (!_validate_code()) {
		printf("QSPI Code loading ...");
		SPI_NOR_FLASH_0->interface->erase(SPI_NOR_FLASH_0, 0, QSPI_ERSIZE);
		SPI_NOR_FLASH_0->interface->write(SPI_NOR_FLASH_0, (uint8_t *)src, 0, QSPI_LMSIZE);
		printf("OK\r\n");
#if CONF_EXT_RW_TEST
		/* Check again and display immediate result.
		 * In case written binary extended to large size for write/read test.
		 */
		if (_validate_code()) {
			printf("Loaded code validation OK\r\n");
		}
#endif
	} else {
		printf("QSPI Code OK, skip loading\r\n");
	}
	printf("Done\r\n");
}

/**
 * Application entry
 */
int main(void)
{
	atmel_start_init();

	printf("\r\n");
	printf("==== QSPI Flash XIP Demo ====\r\n");
	printf("- CPU    : %dKHz\r\n", CONF_CPU_FREQUENCY / 1000);
	printf("- QSPI CK: %dKHz\r\n", CONF_QSPI_BAUD / 1000);

	_load_qspi();

	printf("Run!\r\n");
	printf("LED blinks,\r\n");
	printf("Press SW0 to change blinking mode.\r\n");

	while (1) {
		if (gpio_get_pin_level(SW0)) {
			blink_1(delay_ms);
		} else {
			blink_2(delay_ms);
		}
	}
}
