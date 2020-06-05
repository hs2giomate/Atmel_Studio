/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAME54 has 14 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8
#define GPIO_PIN_FUNCTION_J 9
#define GPIO_PIN_FUNCTION_K 10
#define GPIO_PIN_FUNCTION_L 11
#define GPIO_PIN_FUNCTION_M 12
#define GPIO_PIN_FUNCTION_N 13

#define MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN GPIO(GPIO_PORTC, 1)
#define OLED1_BUTTON2 GPIO(GPIO_PORTC, 2)
#define OLED1_BUTTON3 GPIO(GPIO_PORTC, 3)
#define OLED1_SPI_MOSI GPIO(GPIO_PORTC, 4)
#define OLED1_SPI_SCK GPIO(GPIO_PORTC, 5)
#define OLED1_SPI_MISO_NC GPIO(GPIO_PORTC, 7)
#define MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN GPIO(GPIO_PORTC, 14)
#define OLED1_BUTTON1 GPIO(GPIO_PORTC, 30)
#define MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN GPIO(GPIO_PORTC, 31)
#define OLED1_LED1 GPIO(GPIO_PORTD, 10)

#endif // ATMEL_START_PINS_H_INCLUDED
