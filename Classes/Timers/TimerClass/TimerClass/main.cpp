/*
 * TimerClass.cpp
 *
 * Created: 3/2/2020 11:36:46 AM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>

#include "Timer_Class.h"
#include "CDC_Class.h"
void blink_LED0(void){
	gpio_toggle_pin_level(LED0);
}

int main(void)
{
    /* Initialize the SAM system */
    atmel_start_init();
	timer_start(&TIMER_0);
	CDC_Class	usb0;
	Timer_Class			timer1(&TIMER_1);
	timer1.Init();
	timer1.add_task((FUNC_PTR)blink_LED0,1000);
	
    /* Replace with your application code */
    while (1) 
    {
		while (gpio_get_pin_level(SW0_Button));
		while (!gpio_get_pin_level(SW0_Button));
		
		timer1.start();
		while (gpio_get_pin_level(SW0_Button));
		while (!gpio_get_pin_level(SW0_Button));
		timer1.stop();
    }
}

