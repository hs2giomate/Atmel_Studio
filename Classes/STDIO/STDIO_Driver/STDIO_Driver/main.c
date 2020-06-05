#include <atmel_start.h>
#include "stdio_test.h"
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	float a,b,c;
	a=1.1;
	b=2.22;
	c=3.333;
	/* Replace with your application code */
	while (1) {
		printCalculatedAccels(a,b,c);
		delay_ms(1000);
		printf("\n\r");
	}
}
