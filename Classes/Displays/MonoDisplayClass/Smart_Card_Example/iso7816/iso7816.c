/**
 * \file
 *
 * \brief Smart Card Standard ISO7816 driver.
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

#include "iso7816.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup asfdoc_sam0_iso7816_group Smart Card Service
 *
 * The smart card service provides functions for cards supporting ISO7816 protocol.
 *
 * \ref asfdoc_sam0_iso7816_qs.
 *
 * @{
 */

/** Case for APDU commands. */
#define CASE1 1
#define CASE2 2
#define CASE3 3

/** NULL byte to restart byte procedure. */
#define ISO_NULL_VAL 0x60

/** Receive timeout. */
#define RX_TIMEOUT 12000

/** Reset wait time. */
#define RST_WAIT_TIME 120

/** Variable for state of send and receive from USART. */
static uint8_t iso7816_usart_status = ISO7816_USART_RCV;

/** Smart Card reset PIN. */
static uint32_t              iso7816_pin_rst;
static struct io_descriptor *iso7816_io;

/** Generator source frequency of smart card usart. */
volatile uint32_t iso7816_gs_frequency;

/**
 * \brief Get a character from ISO7816.
 *
 * \param[in] p_char_received Pointer for store the received char.
 *
 * \return Status of the operation.
 * \retval STATUS_OK                If the operation was completed
 * \retval STATUS_ERR_TIMEOUT       If operation was not completed, due to USART
 *                                  module timing out
 */
static int32_t iso7816_get_char(uint8_t *p_char_received)
{
	uint32_t ul_timeout = 0;

	if (iso7816_usart_status == ISO7816_USART_SEND) {
		iso7816_usart_status = ISO7816_USART_RCV;
	}

	/* Wait USART ready for reception. */
	while (io_read(iso7816_io, p_char_received, 1) != 1) {
		if (ul_timeout++ > RX_TIMEOUT * (iso7816_gs_frequency / 1000000)) {
			return ERR_TIMEOUT;
		}
	}

	/* Return status. */
	return ERR_NONE;
}

/**
 * \brief Send a char to ISO7816.
 *
 * \param[in] uc_char Char to be sent.
 *
 * \return Status of the operation.
 */
static int32_t iso7816_send_char(uint8_t uc_char)
{
	int32_t ul_status;
	uint8_t length;

	if (iso7816_usart_status == ISO7816_USART_RCV) {
		iso7816_usart_status = ISO7816_USART_SEND;
	}

	length = io_write(iso7816_io, &uc_char, 1);

	if (length == 1) {
		ul_status = ERR_NONE;
	} else {
		ul_status = ERR_FAILURE;
	}

	/* Return status. */
	return (ul_status);
}

/**
 * \brief ISO7816 ICC power on.
 */
static void iso7816_icc_power_on(void)
{
	/* Set RESET Master Card. */
	gpio_set_pin_level(iso7816_pin_rst, ISO7816_PIN_POWER_ON);
}

/**
 * \brief ISO7816 ICC power off.
 */
static void iso7816_icc_power_off(void)
{
	/* Clear RESET Master Card. */
	gpio_set_pin_level(iso7816_pin_rst, ISO7816_PIN_POWER_OFF);
}

/**
 * \brief Transfer Block TPDU T = 0.
 */
uint16_t iso7816_xfr_block_tpdu_t0(const uint8_t *p_apdu, uint8_t *p_message, uint16_t us_length)
{
	ASSERT(p_apdu);
	ASSERT(p_message);

	uint16_t us_ne_nc;
	uint16_t us_apdu_index    = 4;
	uint16_t us_message_index = 0;
	uint8_t  uc_sw1           = 0;
	uint8_t  uc_proc_byte;
	uint8_t  uc_cmd_case;
	uint8_t  p_dummy[6];

	iso7816_send_char(p_apdu[0]); /* CLA */
	iso7816_send_char(p_apdu[1]); /* INS */
	iso7816_send_char(p_apdu[2]); /* P1 */
	iso7816_send_char(p_apdu[3]); /* P2 */
	iso7816_send_char(p_apdu[4]); /* P3 */

	/* Handle the four structures of command APDU. */
	us_apdu_index = 4;

	if (us_length == 4) {
		uc_cmd_case = CASE1;
		us_ne_nc    = 0;
	} else if (us_length == 5) {
		uc_cmd_case = CASE2;
		us_ne_nc    = p_apdu[4]; /* C5 */
		if (us_ne_nc == 0) {
			us_ne_nc = 256;
		}
	} else if (us_length == 6) {
		us_ne_nc    = p_apdu[4]; /* C5 */
		uc_cmd_case = CASE3;
	} else if (us_length == 7) {
		us_ne_nc = p_apdu[4]; /* C5 */
		if (us_ne_nc == 0) {
			uc_cmd_case = CASE2;
			us_ne_nc    = (p_apdu[5] << 8) + p_apdu[6];
		} else {
			uc_cmd_case = CASE3;
		}
	} else {
		us_ne_nc = p_apdu[4]; /* C5 */
		if (us_ne_nc == 0) {
			uc_cmd_case = CASE3;
			us_ne_nc    = (p_apdu[5] << 8) + p_apdu[6];
		} else {
			uc_cmd_case = CASE3;
		}
	}

	/* Handle Procedure Bytes. */
	do {
		/*Dummy Read - Start*/
		iso7816_get_char(&p_dummy[0]);
		iso7816_get_char(&p_dummy[1]);
		iso7816_get_char(&p_dummy[2]);
		iso7816_get_char(&p_dummy[3]);
		/*Dummy Read - End*/
		iso7816_get_char(&uc_proc_byte);
		uint8_t uc_proc_byte_x = (uc_proc_byte ^ 0xff);
		/* Handle NULL. */
		if (ISO_NULL_VAL == uc_proc_byte) {
			continue;
		}
		/* Handle sw1. */
		else if (((uc_proc_byte & 0xF0) == 0x60) || ((uc_proc_byte & 0xF0) == 0x90)) {
			uc_sw1 = 1;
			break;
		}
		/* Handle INS. */
		else if (p_apdu[1] == uc_proc_byte) {
			if (uc_cmd_case == CASE2) {
				/* Receive data from card. */
				do {
					iso7816_get_char(&p_message[us_message_index++]);
				} while (0 != --us_ne_nc);
			} else {
				/* Send data. */
				do {
					iso7816_send_char(p_apdu[++us_apdu_index]);
				} while (0 != --us_ne_nc);
				/*Dummy Read - Start*/
				iso7816_get_char(&p_dummy[4]);
				iso7816_get_char(&p_dummy[5]);
				/*Dummy Read - End*/
			}
		}
		/* Handle INS ^ 0xff. */
		else if (p_apdu[1] == uc_proc_byte_x) {
			if (uc_cmd_case == CASE2) {
				/* receive data from card. */
				iso7816_get_char(&p_message[us_message_index++]);
			} else {
				iso7816_send_char(p_apdu[us_apdu_index++]);
			}
			us_ne_nc--;
		} else {
			break;
		}
	} while (us_ne_nc != 0);

	/* Status Bytes. */
	if (uc_sw1 == 0) {
		iso7816_get_char(&p_message[us_message_index++]); /* uc_sw1 */
	} else {
		p_message[us_message_index++] = uc_proc_byte;
	}
	iso7816_get_char(&p_message[us_message_index++]); /* SW2 */

	return (us_message_index);
}

/**
 * \brief Answer To Reset (ATR).
 */
void iso7816_data_block_atr(uint8_t *p_atr, uint8_t *p_length)
{
	ASSERT(p_atr);

	uint32_t i;
	uint32_t j;
	uint8_t  uc_value;

	*p_length = 0;

	/* Read ATR TS. */
	iso7816_get_char(&p_atr[0]);

	/* Read ATR T0. */
	iso7816_get_char(&p_atr[1]);
	uc_value = p_atr[1] & 0xF0;
	i        = 2;

	/* Read ATR Ti. */
	while (uc_value) {
		if (uc_value & 0x10) { /* TA[i] */
			iso7816_get_char(&p_atr[i++]);
		}

		if (uc_value & 0x20) { /* TB[i] */
			iso7816_get_char(&p_atr[i++]);
		}

		if (uc_value & 0x40) { /* TC[i] */
			iso7816_get_char(&p_atr[i++]);
		}

		if (uc_value & 0x80) { /* TD[i] */
			iso7816_get_char(&p_atr[i]);
			uc_value = p_atr[i++] & 0xF0;
		} else {
			uc_value = 0;
		}
	}

	/* Historical Bytes. */
	uc_value = p_atr[1] & 0x0F;
	for (j = 0; j < uc_value; j++) {
		iso7816_get_char(&p_atr[i++]);
	}

	*p_length = i;
}

/**
 * \brief Pin status for ISO7816 RESET PIN.
 */
bool iso7816_get_reset_statuts(void)
{
	return gpio_get_pin_level(iso7816_pin_rst);
}

/**
 * \brief Cold reset.
 */
void iso7816_cold_reset(void)
{
	uint32_t i;

	/* Tb: wait 400 cycles */
	for (i = 0; i < (RST_WAIT_TIME * (iso7816_gs_frequency / 1000000)); i++) {
	}

	iso7816_icc_power_on();
}

/**
 * \brief Warm reset.
 */
void iso7816_warm_reset(void)
{
	uint32_t i;

	iso7816_icc_power_off();

	/* Tb: wait 400 cycles. */
	for (i = 0; i < (RST_WAIT_TIME * (iso7816_gs_frequency / 1000000)); i++) {
	}

	iso7816_icc_power_on();
}

/**
 * \brief Decode ATR trace.
 */
void iso7816_decode_atr(uint8_t *p_atr)
{
	uint32_t i;
	uint32_t j;
	uint32_t y;
	uint8_t  uc_offset;

	i = 2;
	y = p_atr[1] & 0xF0;

	/* Read ATR Ti. */
	uc_offset = 1;
	while (y) {
		if (y & 0x10) { /* TA[i] */
			i++;
		}
		if (y & 0x20) { /* TB[i] */
			i++;
		}
		if (y & 0x40) { /* TC[i] */
			i++;
		}
		if (y & 0x80) { /* TD[i] */
			y = p_atr[i++] & 0xF0;
		} else {
			y = 0;
		}
		uc_offset++;
	}

	y = p_atr[1] & 0x0F;
	for (j = 0; j < y; j++) {
		i++;
	}
}

/**
 * \brief Initializes a ISO7816 interface device.
 */
void iso7816_init(struct io_descriptor *io, uint32_t pin_rst, uint32_t clock_get_hz)
{
	iso7816_io           = io;
	iso7816_pin_rst      = pin_rst;
	iso7816_gs_frequency = clock_get_hz;
}
	//@}

	/**
	 * \page asfdoc_sam0_iso7816_qs
	 *
	 * Driver for smart card via ISO7816.
	 * See \ref appdoc_sam0_smart_card_main "here" for project documentation.
	 *
	 */

#ifdef __cplusplus
}
#endif
