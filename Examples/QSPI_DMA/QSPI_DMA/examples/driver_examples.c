/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static uint8_t buf[16] = {0x0};

static void xfer_complete_cb_QSPI_INSTANCE(struct _dma_resource *resource)
{
	/* Transfer completed */
}

/**
 * Example of using QSPI_INSTANCE to get N25Q256A status value,
 * and check bit 0 which indicate embedded operation is busy or not.
 */
void QSPI_INSTANCE_example(void)
{
	struct _qspi_command cmd = {
	    .inst_frame.bits.inst_en      = 1,
	    .inst_frame.bits.data_en      = 1,
	    .inst_frame.bits.addr_en      = 1,
	    .inst_frame.bits.dummy_cycles = 8,
	    .inst_frame.bits.tfr_type     = QSPI_READMEM_ACCESS,
	    .instruction                  = 0x0B,
	    .address                      = 0,
	    .buf_len                      = 14,
	    .rx_buf                       = buf,
	};

	qspi_dma_register_callback(&QSPI_INSTANCE, QSPI_DMA_CB_XFER_DONE, xfer_complete_cb_QSPI_INSTANCE);
	qspi_dma_enable(&QSPI_INSTANCE);
	qspi_dma_serial_run_command(&QSPI_INSTANCE, &cmd);
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}
