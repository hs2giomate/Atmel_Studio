#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1) {
		
		
		if (gpio_get_pin_level(NMI))
		{
			
		} 
		else
		{
			gpio_toggle_pin_level(redLed);
			delay_ms(500);
		}
		
	}
}
