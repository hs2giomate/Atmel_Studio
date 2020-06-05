/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries.
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
#include "iso7816/iso7816.h"
#include "peripheral_clk_config.h"
#include <string.h>

#define STRING_HEADER                                                                                                  \
	"--Smart Card(ISO7816) Quick Start Example --\r\n"                                                                 \
	"-- Compiled: " __DATE__ " " __TIME__ " --\r\n"

/* Maximum uc_size in bytes of the smart card answer to a uc_command. */
#define MAX_ANSWER_SIZE 10

/* Maximum ATR uc_size in bytes. */
#define MAX_ATR_SIZE 55

#define CMD1_LEN 7
#define CMD2_LEN 5

/* Smart card clock frequency (use GCLK3 output). */
#define CONF_SMART_CARD_CLK_FREQ 3573350

/* Test command #1 - Write to memory Test Zone {CLA, INS, Add, Add, No of bytes, Data}*/
const uint8_t test_cmd1[CMD1_LEN] = {0x00, 0xB4, 0x00, 0x0A, 0x02, 0x25, 0x22};

/* Test command #2 - Read from Memory Test Zone {CLA, INS, Add, Add, No of bytes}*/
const uint8_t test_cmd2[CMD2_LEN] = {0x00, 0xB6, 0x00, 0x0A, 0x02};

/**
 * \brief Detect smart card.
 */
static void smart_card_detect(void)
{
	if (gpio_get_pin_level(CARD_DET) == false) {
		printf("Please insert a smart card.\r\n");
	}
	while (gpio_get_pin_level(CARD_DET) == false) {
		/* Wait insert smart card */
	}

	printf("Smart card is detected.\r\n");
	delay_ms(1000);
}

/**
 * \brief Initialize the SERCOM USART (ISO7816 mode) for smart card.
 */
static void smart_card_init(void)
{
	struct io_descriptor *io;

	gpio_set_pin_level(CARD_VCC, true);

	usart_sync_get_io_descriptor(&SMART_CARD_COM, &io);
	iso7816_init(io, CARD_RST, CONF_SMART_CARD_CLK_FREQ);

	usart_sync_enable(&SMART_CARD_COM);
}

/**
 * \brief Displays a menu which enables the user to send several commands to the
 * smart card and check its answers.
 */
static void send_receive_cmd(void)
{
	uint8_t  uc_message[MAX_ANSWER_SIZE];
	uint8_t  uc_size;
	uint16_t uc_key;
	uint8_t  uc_command;
	uint8_t  i;

	/* Clear message buffer. */
	for (i = 0; i < MAX_ANSWER_SIZE; i++) {
		uc_message[i] = 0;
	}

	/* Display menu. */
	printf("The following three User Inputs can be sent:\r\n");
	printf("  1 - Execute Write command: ");
	for (i = 0; i < CMD1_LEN; i++) {
		printf("0x%02X ", test_cmd1[i]);
	}
	printf("\r\n  2 - Execute Read Command: ");

	for (i = 0; i < CMD2_LEN; i++) {
		printf("0x%02X ", test_cmd2[i]);
	}
	printf("\r\n  q - Quit the Application. ");
	/* Get user input. */
	uc_key = 0;
	while (uc_key != 'q') {
		printf("\r\n\r\nChoice ? ('1' or '2' or 'q'): ");
		uc_key = getchar();
		printf("%c\r\n", uc_key);
		uc_command = (uint8_t)uc_key - '0';

		/* Check user input. */
		uc_size = 0;
		if (uc_command == 1) {
			printf("Sending test command #1 (Writing 2 bytes in Memory Test Zone) : ");
			for (i = 0; i < CMD1_LEN; i++) {
				printf("0x%02X ", test_cmd1[i]);
			}
			printf("\r\n...");
			uc_size = iso7816_xfr_block_tpdu_t0(test_cmd1, uc_message, CMD1_LEN);
		} else {
			if (uc_command == 2) {
				printf("Sending test command #2 (Reading 2 bytes from Memory Test Zone): ");
				for (i = 0; i < CMD2_LEN; i++) {
					printf("0x%02X ", test_cmd2[i]);
				}
				printf("\r\n...");
				uc_size = iso7816_xfr_block_tpdu_t0(test_cmd2, uc_message, CMD2_LEN);
			}
		}

		/* Output smart card answer. */
		if (uc_size > 0) {
			printf("\r\nAnswer: ");
			for (i = 0; i < uc_size; i++) {
				printf("0x%02X ", uc_message[i]);
			}
		}
	}
	printf("\r\nPlease wait... \r\n");
	gpio_set_pin_level(CARD_VCC, false);
	printf("The smart card can now be safely removed. \r\n");
}

int main(void)
{
	uint8_t  p_atr[MAX_ATR_SIZE];
	uint8_t  uc_size;
	uint16_t i;

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Display example information. */
	printf("%s \r\n", STRING_HEADER);

	/* Smart card detect and display the status. */
	smart_card_detect();

	/* Smart card Initialization. */
	smart_card_init();

	/* Smart card warm reset. */
	iso7816_warm_reset();

	memset(p_atr, 0, sizeof(p_atr));
	iso7816_data_block_atr(p_atr, &uc_size);

	if (uc_size != 0) {
		printf("Reset response from smart card: ");
		for (i = 0; i < uc_size; i++) {
			printf("0x%02X ", p_atr[i]);
		}
		printf("\r\n");
	}

	/* User inputs commands to send / receive / quit. */
	send_receive_cmd();

	/* Replace with your application code */
	while (1) {
	}
}
