/**
 * \file
 *
 * \brief N25Q256A component declaration
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
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _N25Q256A_H_INCLUDED
#define _N25Q256A_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <spi_nor_flash.h>

/** Register addresses of n25q256a */
/** Resume from deep power-down command code. */
#define N25Q_SOFT_RESET_ENABLE 0x66
/** Resume from deep power-down command code. */
#define N25Q_SOFT_RESET 0x99
/** Read status register command code. */
#define N25Q_READ_STATUS_REGISTER 0x05
/** Write status register command code. */
#define N25Q_WRITE_STATUS_REGISTER 0x01
/** Read status register command code. */
#define N25Q_READ_FLAG_STATUS_REGISTER 0x70
/** Write volatile config register command code. */
#define N25Q_WRITE_VOLATILE_CONFIG_REGISTER 0x81
/** Read volatile config register command code. */
#define N25Q_READ_VOLATILE_CONFIG_REGISTER 0x85
/** Write enhanced volatile config register command code. */
#define N25Q_WRITE_ENHANCED_VOLATILE_CONFIG_REGISTER 0x61
/** Read enhanced volatile config register command code. */
#define N25Q_READ_ENHANCED_VOLATILE_CONFIG_REGISTER 0x65
/** Write enable command code. */
#define N25Q_WRITE_ENABLE 0x06
/** Write disable command code. */
#define N25Q_WRITE_DISABLE 0x04
/** Byte/page program command code. */
#define N25Q_BYTE_PAGE_PROGRAM 0x02
/** Byte/page program command code. */
#define N25Q_QUAD_INPUT_PROGRAM 0x32
/** Block erase command code (4K block). */
#define N25Q_BLOCK_ERASE_4K 0x20
/** Block erase command code (64K block). */
#define N25Q_BLOCK_ERASE_64K 0xD8
/** Chip erase command code 2. */
#define N25Q_BULK_ERASE 0xC7
/** Read array (low frequency) command code. */
#define N25Q_READ_ARRAY_LF 0x03
/** Read array command code. */
#define N25Q_FAST_READ 0x0B
/** Fast Read array  command code. */
#define N25Q_DUAL_OUTPUT_READ 0x3B
/** Fast Read array  command code. */
#define N25Q_QUAD_OUTPUT_READ 0x6B
/** Fast Read array  command code. */
#define N25Q_DUAL_IO_FAST_READ 0xBB
/** Fast Read array  command code. */
#define N25Q_QUAD_I0_FAST_READ 0xEB
/** Read manufacturer and device ID command code. */
#define N25Q_READ_JEDEC_ID 0x9F

/** Size of n25q256a reg */
#define N25Q_RD_REG_SIZE(op) (1)
#define N25Q_WR_REG_SIZE(op) (1)

/** Size of n25q256a */
#define N25Q_PAGE_SIZE 256
#define N25Q_SECTOR_SIZE 4096
#define N25Q_BLOCK_SIZE 65536
#define N25Q_FLASH_SIZE 0x200000

typedef void (*func)(void);

/**
 * \brief N25Q256A spi nor flash
 */
struct n25q256a {
	/** The parent abstract spi nor flash */
	struct spi_nor_flash parent;
	/** Pin exit xip function pointer*/
	func pin_exit_xip;
	/** Quad mode used by SPI read/write data */
	uint8_t quad_mode;
	/** XIP mode used */
	uint8_t xip_mode;
};

/**
 * \brief Construct n25q256a spi nor flash
 *
 * \param[in] me        Pointer to the abstract spi nor flash structure
 * \param[in] io        Pointer to the instance of controller
 * \param[in] quad_mode 0 single-bit SPI data, 1 quad SPI data
 *
 * \return pointer to initialized flash
 */
struct spi_nor_flash *n25q256a_construct(struct spi_nor_flash *const me, void *const io, func pin_exit_xip,
                                         const uint8_t quad_mode);

/**
 * \brief Read bytes from the spi nor Flash
 *
 * \param[in]  me       Pointer to the abstract spi nor flash structure
 * \param[out] buf      Pointer to the buffer
 * \param[in]  address  Source bytes address to read from flash
 * \param[in]  length   Number of bytes to read
 * \return status.
 */
int32_t n25q256a_read(const struct spi_nor_flash *const me, uint8_t *buf, uint32_t address, uint32_t length);

/**
 * \brief Write bytes to the spi nor Flash
 *
 * \param[in] me        Pointer to the abstract spi nor flash structure
 * \param[in] buf       Pointer to the buffer
 * \param[in] address   Destination bytes address to write into flash
 * \param[in] length    Number of bytes to write
 * \return status.
 */
int32_t n25q256a_write(const struct spi_nor_flash *const me, uint8_t *buf, uint32_t address, uint32_t length);

/**
 * \brief Erase sectors/blocks in the spi nor Flash
 * \param[in] me        Pointer to the abstract spi nor flash structure
 * \param[in] address   Destination bytes address aligned with sector/block start
 * \param[in] length    Number of bytes to be erase
 * \return status.
 *
 * \note length must be multiple of sector size
 */
int32_t n25q256a_erase(const struct spi_nor_flash *const me, uint32_t address, uint32_t length);

/**
 * \brief Enable the XIP mode (continous read)
 *
 * \param[in] me        Pointer to the abstract spi nor flash structure
 * \return status.
 */
int32_t n25q256a_enable_xip(const struct spi_nor_flash *const me);

/**
 * \brief Disable the XIP mode (continous read)
 *
 * \param[in] me        Pointer to the abstract spi nor flash structure
 * \return status.
 */
int32_t n25q256a_disable_xip(const struct spi_nor_flash *const me);
uint32_t n25q256a_read_reg(const struct spi_nor_flash *const me, uint8_t width, uint8_t inst);

#ifdef __cplusplus
}
#endif

#endif /* _N25Q256A_H_INCLUDED */
