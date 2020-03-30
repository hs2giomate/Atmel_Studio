/**
 * \file
 *
 * \brief DAC Example using Events via DMA.
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

#include <atmel_start.h>
#include "hpl_dma.h"

/* Application Constants */
#define DMAC_CHANNEL_DAC 0U
#define DAC_MODULE 0U
#define LED_DELAY_MS 1000U

/* RTOS Tasks Constants */
#define TASK_EXAMPLE_STACK_SIZE (128 / sizeof(portSTACK_TYPE))
#define TASK_EXAMPLE_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

/* RTOS Tasks Handle */
static TaskHandle_t xCreatedExampleTask;

/* Source Address Pointer for DMAC */
uint32_t *src_addr = 0;

/* SineWave Buffer (64 sample points) */
uint16_t const sine_wave_64samples[64]
    = {2047, 2248, 2447, 2641, 2831, 3012, 3184, 3346, 3495, 3630, 3749, 3853, 3938, 4006, 4055, 4084,
       4094, 4084, 4055, 4006, 3938, 3853, 3749, 3630, 3495, 3346, 3184, 3012, 2831, 2641, 2447, 2248,
       2047, 1846, 1648, 1453, 1264, 1082, 910,  748,  600,  465,  345,  242,  156,  88,   39,   10,
       0,    10,   39,   88,   156,  242,  345,  465,  600,  748,  910,  1082, 1264, 1453, 1648, 1846};

/**
 * DMAC configuration task
 *
 * \param[in] none
 *
 */
void configure_dmac_start_tc(void)
{
	/* Disable the TC(enabled as part of Init()) and enable after
	 * DMAC is configured and enabled correctly */
	hri_tc_write_CTRLA_ENABLE_bit(TC0, false);
	hri_tc_wait_for_sync(TC0, TC_SYNCBUSY_ENABLE_Pos);

	/* Clear the status flags set because TC was running already */
	hri_tc_get_INTFLAG_reg(TC0, (TC_INTFLAG_OVF | TC_INTFLAG_ERR | TC_INTFLAG_MC0 | TC_INTFLAG_MC1));

	/* Enable the DAC so that it can start conversion when trigger comes */
	hri_dac_set_DACCTRL_ENABLE_bit(DAC, false);
	hri_dac_set_CTRLA_ENABLE_bit(DAC);
	hri_dac_wait_for_sync(DAC, DAC_SYNCBUSY_ENABLE);

	/* Get the Source address for DMAC (starting address of source buffer) */
	src_addr = sine_wave_64samples;

	/* Configure the DMAC source address, destination address,
	 * next descriptor address, data count and Enable the DMAC Channel */
	_dma_set_source_address(DMAC_CHANNEL_DAC, (const void *)src_addr);
	_dma_set_destination_address(DMAC_CHANNEL_DAC, (const void *)&DAC->DATA[DAC_MODULE].reg);
	_dma_set_data_amount(DMAC_CHANNEL_DAC, (sizeof sine_wave_64samples) / 2);
	_dma_set_next_descriptor(DMAC_CHANNEL_DAC, DMAC_CHANNEL_DAC);
	_dma_enable_transaction(DMAC_CHANNEL_DAC, false);

	/* Enable the TC so that it can start triggering DMAC channel.
	 * This triggers DAC conversion to be started */
	hri_tc_write_CTRLA_ENABLE_bit(TC0, true);
	hri_tc_wait_for_sync(TC0, TC_SYNCBUSY_ENABLE_Pos);
}

/**
 * RTOS example task
 *
 * \param[in] none
 *
 */
void example_task(void)
{
	while (1) {
		/* Toggle LED0 */
		gpio_toggle_pin_level(LED0);
		vTaskDelay(LED_DELAY_MS);
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Print Example Header */
	printf(" -- Event triggered DAC Example (RTOS Based) -- \r\n");

	/* Configure DMAC to trigger DAC based on TC Output event
	 * Connect PA03(in SAME54 Xplained Pro EXT2) to VDD - because of
	 * ERRATUM */
	configure_dmac_start_tc();

	/* Create Application task */
	if (xTaskCreate((TaskFunction_t)example_task,
	                "Example",
	                TASK_EXAMPLE_STACK_SIZE,
	                NULL,
	                TASK_EXAMPLE_STACK_PRIORITY,
	                xCreatedExampleTask)
	    != pdPASS) {
		while (1) {
			;
		}
	}

	/* Start the RTOS Scheduler */
	vTaskStartScheduler();

	while (1) {
		/* Code should not come here */
	}
}
