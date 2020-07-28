/**
 * \file
 *
 * \brief DMA related functionality implementation.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_dma_init DMA Init Driver
 * \ingroup doc_driver_dma
 *
 * \section doc_driver_dma_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <dma.h>

/**
 * \brief Initialize dma interface
 * \return Initialization status.
 */
int8_t DMA_0_init()
{

	/*Basic Configuration*/

	DMA.CTRL = 1 << DMA_ENABLE_bp       /* Enable: enabled */
	           | DMA_DBUFMODE_CH01_gc   /* Double buffering enabled on channel 0/1 */
	           | DMA_PRIMODE_RR0123_gc; /* Round Robin */

	/* Channel-1 Configurations*/

	// DMA.CH1.CTRLB = DMA_CH_ERRINTLVL_OFF_gc /* Interrupt disabled */
	//		 | DMA_CH_TRNINTLVL_OFF_gc; /* Interrupt disabled */

	// DMA.CH1.ADDRCTRL = DMA_CH_SRCRELOAD_NONE_gc /* No reload */
	//		 | DMA_CH_SRCDIR_FIXED_gc /* Fixed */
	//		 | DMA_CH_DESTRELOAD_NONE_gc /* No reload */
	//		 | DMA_CH_DESTDIR_FIXED_gc; /* Fixed */

	// DMA.CH1.TRIGSRC = DMA_CH_TRIGSRC_OFF_gc; /* Off software triggers only */

	// DMA.CH1.TRFCNT = 0x1; /* Channel Block Transfer Count: 0x1 */

	// DMA.CH1.REPCNT = 0x0; /* Channel Repeat Count: 0x0 */

	// DMA.CH1.SRCADDR0 = 0x0 & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH1.SRCADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH1.SRCADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH1.DESTADDR0 = 0x0 & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH1.DESTADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH1.DESTADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH1.CTRLA = 0 << DMA_CH_ENABLE_bp /* Channel Enable: disabled */
	//		 | 0 << DMA_CH_REPEAT_bp /* Channel Repeat Mode: disabled */
	//		 | 0 << DMA_CH_SINGLE_bp /* Channel Single Shot Data Transfer: disabled */
	//		 | DMA_CH_BURSTLEN_1BYTE_gc; /* 1-byte burst mode */

	/* Channel-0 Configurations*/

	// DMA.CH0.CTRLB = DMA_CH_ERRINTLVL_OFF_gc /* Interrupt disabled */
	//		 | DMA_CH_TRNINTLVL_OFF_gc; /* Interrupt disabled */

	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc    /* Reload at end of burst */
	                   | DMA_CH_SRCDIR_INC_gc       /* Increment */
	                   | DMA_CH_DESTRELOAD_BLOCK_gc /* Reload at end of block */
	                   | DMA_CH_DESTDIR_INC_gc;     /* Increment */

	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc; /* ADCA Channel 0 */

	DMA.CH0.TRFCNT = 0x80; /* Channel Block Transfer Count: 0x80 */

	// DMA.CH0.REPCNT = 0x0; /* Channel Repeat Count: 0x0 */

	// DMA.CH0.SRCADDR0 = 0x0 & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH0.SRCADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH0.SRCADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH0.DESTADDR0 = 0x0 & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH0.DESTADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH0.DESTADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Destination Address: 0x0 */

	DMA.CH0.CTRLA = 1 << DMA_CH_ENABLE_bp       /* Channel Enable: enabled */
	                | 0 << DMA_CH_REPEAT_bp     /* Channel Repeat Mode: disabled */
	                | 1 << DMA_CH_SINGLE_bp     /* Channel Single Shot Data Transfer: enabled */
	                | DMA_CH_BURSTLEN_2BYTE_gc; /* 2-byte burst mode */

	/* Channel-3 Configurations*/

	// DMA.CH3.CTRLB = DMA_CH_ERRINTLVL_OFF_gc /* Interrupt disabled */
	//		 | DMA_CH_TRNINTLVL_OFF_gc; /* Interrupt disabled */

	// DMA.CH3.ADDRCTRL = DMA_CH_SRCRELOAD_NONE_gc /* No reload */
	//		 | DMA_CH_SRCDIR_FIXED_gc /* Fixed */
	//		 | DMA_CH_DESTRELOAD_NONE_gc /* No reload */
	//		 | DMA_CH_DESTDIR_FIXED_gc; /* Fixed */

	// DMA.CH3.TRIGSRC = DMA_CH_TRIGSRC_OFF_gc; /* Off software triggers only */

	// DMA.CH3.TRFCNT = 0x1; /* Channel Block Transfer Count: 0x1 */

	// DMA.CH3.REPCNT = 0x0; /* Channel Repeat Count: 0x0 */

	// DMA.CH3.SRCADDR0 = 0x0 & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH3.SRCADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH3.SRCADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH3.DESTADDR0 = 0x0 & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH3.DESTADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH3.DESTADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH3.CTRLA = 0 << DMA_CH_ENABLE_bp /* Channel Enable: disabled */
	//		 | 0 << DMA_CH_REPEAT_bp /* Channel Repeat Mode: disabled */
	//		 | 0 << DMA_CH_SINGLE_bp /* Channel Single Shot Data Transfer: disabled */
	//		 | DMA_CH_BURSTLEN_1BYTE_gc; /* 1-byte burst mode */

	/* Channel-2 Configurations*/

	// DMA.CH2.CTRLB = DMA_CH_ERRINTLVL_OFF_gc /* Interrupt disabled */
	//		 | DMA_CH_TRNINTLVL_OFF_gc; /* Interrupt disabled */

	// DMA.CH2.ADDRCTRL = DMA_CH_SRCRELOAD_NONE_gc /* No reload */
	//		 | DMA_CH_SRCDIR_FIXED_gc /* Fixed */
	//		 | DMA_CH_DESTRELOAD_NONE_gc /* No reload */
	//		 | DMA_CH_DESTDIR_FIXED_gc; /* Fixed */

	// DMA.CH2.TRIGSRC = DMA_CH_TRIGSRC_OFF_gc; /* Off software triggers only */

	// DMA.CH2.TRFCNT = 0x1; /* Channel Block Transfer Count: 0x1 */

	// DMA.CH2.REPCNT = 0x0; /* Channel Repeat Count: 0x0 */

	// DMA.CH2.SRCADDR0 = 0x0 & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH2.SRCADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH2.SRCADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Source Address: 0x0 */

	// DMA.CH2.DESTADDR0 = 0x0 & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH2.DESTADDR1 = ((uint32_t) 0x0 >> 8 ) & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH2.DESTADDR2 = ((uint32_t) 0x0 >> 16 ) & 0xFF; /* Channel Destination Address: 0x0 */

	// DMA.CH2.CTRLA = 0 << DMA_CH_ENABLE_bp /* Channel Enable: disabled */
	//		 | 0 << DMA_CH_REPEAT_bp /* Channel Repeat Mode: disabled */
	//		 | 0 << DMA_CH_SINGLE_bp /* Channel Single Shot Data Transfer: disabled */
	//		 | DMA_CH_BURSTLEN_1BYTE_gc; /* 1-byte burst mode */

	return 0;
}
