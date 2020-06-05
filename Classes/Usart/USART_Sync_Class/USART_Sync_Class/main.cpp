/*
 * USART_Sync_Class.cpp
 *
 * Created: 6/2/2020 12:57:40 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "string.h"
#include "eDeviceSerialStream.h"
static	 serialStream		asc2;
int main(void)
{
	atmel_start_init();
	asc2.init();
	/* Replace with your application code */
	while (1) {
	asc2<<"we are streaming";
		delay_ms(1000);
	}
}
