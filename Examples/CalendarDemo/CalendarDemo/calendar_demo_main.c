/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "atmel_start.h"
#include <hal_gpio.h>
#include <hal_delay.h>
#include "hal_calendar.h"

static struct calendar_alarm alarm0, alarm1, alarm2, alarm3, alarm4, alarm5;

/* [callback] */
void calendar_alarm_callback0(struct calendar_descriptor *const calendar)
{
	gpio_toggle_pin_level(LED0);
}

void calendar_alarm_callback1(struct calendar_descriptor *const calendar)
{
	gpio_toggle_pin_level(LED0);
}

void calendar_alarm_callback2(struct calendar_descriptor *const calendar)
{
	gpio_toggle_pin_level(LED0);
}

void calendar_alarm_callback3(struct calendar_descriptor *const calendar)
{
	gpio_toggle_pin_level(LED0);
}

void calendar_alarm_callback4(struct calendar_descriptor *const calendar)
{
	gpio_toggle_pin_level(LED0);
}

void calendar_alarm_callback5(struct calendar_descriptor *const calendar)
{
	gpio_toggle_pin_level(LED0);
}

int main(void)
{
	atmel_start_init();

	/* off the light */
	gpio_set_pin_level(LED0, true);

	/* Set current time. */
	struct calendar_date date;
	date.year  = 2000;
	date.month = 12;
	date.day   = 31;
	struct calendar_time time;
	time.hour = 13;
	time.min  = 59;
	time.sec  = 59;

	calendar_enable(&CALENDAR_0);
	calendar_set_time(&CALENDAR_0, &time);
	calendar_set_date(&CALENDAR_0, &date);

	/* Set alarm. */
	alarm0.cal_alarm.datetime.date.month = 0;
	alarm0.cal_alarm.datetime.date.day   = 0;
	alarm0.cal_alarm.datetime.time.hour  = 0;
	alarm0.cal_alarm.datetime.time.min   = 0;
	alarm0.cal_alarm.datetime.time.sec   = 0;
	alarm0.cal_alarm.option              = CALENDAR_ALARM_MATCH_SEC;
	alarm0.cal_alarm.mode                = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm0, calendar_alarm_callback0);

	alarm1.cal_alarm.datetime.date.month = 0;
	alarm1.cal_alarm.datetime.date.day   = 0;
	alarm1.cal_alarm.datetime.time.hour  = 0;
	alarm1.cal_alarm.datetime.time.min   = 0;
	alarm1.cal_alarm.datetime.time.sec   = 10;
	alarm1.cal_alarm.option              = CALENDAR_ALARM_MATCH_SEC;
	alarm1.cal_alarm.mode                = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm1, calendar_alarm_callback1);

	alarm2.cal_alarm.datetime.date.month = 0;
	alarm2.cal_alarm.datetime.date.day   = 0;
	alarm2.cal_alarm.datetime.time.hour  = 0;
	alarm2.cal_alarm.datetime.time.min   = 0;
	alarm2.cal_alarm.datetime.time.sec   = 20;
	alarm2.cal_alarm.option              = CALENDAR_ALARM_MATCH_SEC;
	alarm2.cal_alarm.mode                = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm2, calendar_alarm_callback2);

	alarm3.cal_alarm.datetime.date.month = 0;
	alarm3.cal_alarm.datetime.date.day   = 0;
	alarm3.cal_alarm.datetime.time.hour  = 0;
	alarm3.cal_alarm.datetime.time.min   = 0;
	alarm3.cal_alarm.datetime.time.sec   = 30;
	alarm3.cal_alarm.option              = CALENDAR_ALARM_MATCH_SEC;
	alarm3.cal_alarm.mode                = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm3, calendar_alarm_callback3);

	alarm4.cal_alarm.datetime.date.month = 0;
	alarm4.cal_alarm.datetime.date.day   = 0;
	alarm4.cal_alarm.datetime.time.hour  = 0;
	alarm4.cal_alarm.datetime.time.min   = 0;
	alarm4.cal_alarm.datetime.time.sec   = 40;
	alarm4.cal_alarm.option              = CALENDAR_ALARM_MATCH_SEC;
	alarm4.cal_alarm.mode                = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm4, calendar_alarm_callback4);

	alarm5.cal_alarm.datetime.date.month = 0;
	alarm5.cal_alarm.datetime.date.day   = 0;
	alarm5.cal_alarm.datetime.time.hour  = 0;
	alarm5.cal_alarm.datetime.time.min   = 0;
	alarm5.cal_alarm.datetime.time.sec   = 50;
	alarm5.cal_alarm.option              = CALENDAR_ALARM_MATCH_SEC;
	alarm5.cal_alarm.mode                = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm5, calendar_alarm_callback5);

	while (1) {
	}
}
