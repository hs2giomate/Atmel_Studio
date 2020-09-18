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

/**
 * Example of using SPI_MEMORIES to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_MEMORIES[12] = "Hello World!";

void SPI_MEMORIES_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_MEMORIES, &io);

	spi_m_sync_enable(&SPI_MEMORIES);
	io_write(io, example_SPI_MEMORIES, 12);
}

/**
 * Example of using CALENDAR.
 */
static struct calendar_alarm alarm;

static void alarm_cb(struct calendar_descriptor *const descr)
{
	/* alarm expired */
}

void CALENDAR_example(void)
{
	struct calendar_date date;
	struct calendar_time time;

	calendar_enable(&CALENDAR);

	date.year  = 2000;
	date.month = 12;
	date.day   = 31;

	time.hour = 12;
	time.min  = 59;
	time.sec  = 59;

	calendar_set_date(&CALENDAR, &date);
	calendar_set_time(&CALENDAR, &time);

	alarm.cal_alarm.datetime.time.sec = 4;
	alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm.cal_alarm.mode              = REPEAT;

	calendar_set_alarm(&CALENDAR, &alarm, alarm_cb);
}

static struct timer_task TIMER_INTERFACE_task1, TIMER_INTERFACE_task2;

/**
 * Example of using TIMER_INTERFACE.
 */
static void TIMER_INTERFACE_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_INTERFACE_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_INTERFACE_example(void)
{
	TIMER_INTERFACE_task1.interval = 100;
	TIMER_INTERFACE_task1.cb       = TIMER_INTERFACE_task1_cb;
	TIMER_INTERFACE_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_INTERFACE_task2.interval = 200;
	TIMER_INTERFACE_task2.cb       = TIMER_INTERFACE_task2_cb;
	TIMER_INTERFACE_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_INTERFACE, &TIMER_INTERFACE_task1);
	timer_add_task(&TIMER_INTERFACE, &TIMER_INTERFACE_task2);
	timer_start(&TIMER_INTERFACE);
}

static struct timer_task TIMER_CALENDAR_task1, TIMER_CALENDAR_task2;

/**
 * Example of using TIMER_CALENDAR.
 */
static void TIMER_CALENDAR_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_CALENDAR_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_CALENDAR_example(void)
{
	TIMER_CALENDAR_task1.interval = 100;
	TIMER_CALENDAR_task1.cb       = TIMER_CALENDAR_task1_cb;
	TIMER_CALENDAR_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_CALENDAR_task2.interval = 200;
	TIMER_CALENDAR_task2.cb       = TIMER_CALENDAR_task2_cb;
	TIMER_CALENDAR_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_CALENDAR, &TIMER_CALENDAR_task1);
	timer_add_task(&TIMER_CALENDAR, &TIMER_CALENDAR_task2);
	timer_start(&TIMER_CALENDAR);
}
