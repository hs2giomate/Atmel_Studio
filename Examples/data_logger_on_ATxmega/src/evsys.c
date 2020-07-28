/**
 * \file
 *
 * \brief EVSYS related functionality implementation.
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
 * \addtogroup doc_driver_evsys
 *
 * \section doc_driver_evsys_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <evsys.h>

/**
 * \brief Initialize EVSYS interface
 *
 * \return Initialization status.
 */
int8_t EVENT_SYSTEM_0_init()
{

	// EVSYS.CH0MUX = EVSYS_CHMUX_OFF_gc; /* Off */

	// EVSYS.CH1MUX = EVSYS_CHMUX_OFF_gc; /* Off */

	// EVSYS.CH2MUX = EVSYS_CHMUX_OFF_gc; /* Off */

	// EVSYS.CH3MUX = EVSYS_CHMUX_OFF_gc; /* Off */

	// EVSYS.CH4MUX = EVSYS_CHMUX_OFF_gc; /* Off */

	// EVSYS.CH5MUX = EVSYS_CHMUX_OFF_gc; /* Off */

	// EVSYS.CH6MUX = EVSYS_CHMUX_OFF_gc; /* Off */

	EVSYS.CH7MUX = EVSYS_CHMUX_RTC_OVF_gc; /* RTC Overflow */

	// EVSYS.CH0CTRL = 0 << EVSYS_QDEN_bp /* Quadrature Decode Enable: disabled */
	//		 | 0 << EVSYS_QDIEN_bp /* Quadrature Decode Index Enable: disabled */
	//		 | EVSYS_QDIRM_00_gc /* QDPH0 = 0, QDPH90 = 0 */
	//		 | EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	// EVSYS.CH1CTRL = EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	// EVSYS.CH2CTRL = 0 << EVSYS_QDEN_bp /* Quadrature Decode Enable: disabled */
	//		 | 0 << EVSYS_QDIEN_bp /* Quadrature Decode Index Enable: disabled */
	//		 | EVSYS_QDIRM_00_gc /* QDPH0 = 0, QDPH90 = 0 */
	//		 | EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	// EVSYS.CH3CTRL = EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	// EVSYS.CH4CTRL = 0 << EVSYS_QDEN_bp /* Quadrature Decode Enable: disabled */
	//		 | 0 << EVSYS_QDIEN_bp /* Quadrature Decode Index Enable: disabled */
	//		 | EVSYS_QDIRM_00_gc /* QDPH0 = 0, QDPH90 = 0 */
	//		 | EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	// EVSYS.CH5CTRL = EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	// EVSYS.CH6CTRL = EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	// EVSYS.CH7CTRL = EVSYS_DIGFILT_1SAMPLE_gc; /* 1 SAMPLE */

	return 0;
}
