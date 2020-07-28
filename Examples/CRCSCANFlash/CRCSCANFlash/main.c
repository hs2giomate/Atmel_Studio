#include "atmel_start.h"
#include "main.h"
#include <util/delay.h>

int main(void)
{
	// Initializes MCU, drivers and middleware
	atmel_start_init();

	// Initialize CRCSCAN
	//  - Run Priority mode on entire Flash
	//  - Enable CRCSCAN
	CRCSCAN_CTRLB = (0x0 << CRCSCAN_MODE0_bp)
	                | // 0x0 PRIORITY, 0x2 SINGLE mode in background, 0x3 CONTINUOUS mode in background
	                (0x0 << CRCSCAN_SRC0_bp); // 0x0 FLASH, 0x1 APPLICATION, 0x2 BOOT
	CRCSCAN_CTRLA = (1 << CRCSCAN_ENABLE_bp); // Enable CRCSCAN

	// Application will be stalled until CRCSCAN has finished

	// Program Flow
	while (1) {
		// Previous CRC Calculation Passed
		if (CRCSCAN_STATUS & CRCSCAN_OK_bm) {
			// Toggle LED0 with OK_DELAY
			LED0_toggle_level();
			_delay_ms(OK_DELAY);
		}

		// Previous CRC Calculation Failed
		else if (CRCSCAN_STATUS == 0x0) {
			// Toggle LED0 with FAIL_DELAY
			LED0_toggle_level();
			_delay_ms(FAIL_DELAY);
		}
	}

	return 0;
}
