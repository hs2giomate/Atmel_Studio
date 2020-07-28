========
XIP Demo
========

The example demostrates Execute-in-place (XIP) mode of Serial NOR Flash Memory.

The example copies pre-compiled binary functions to Serial NOR Flash,
and executes them in main loop to blink the on-board LED in different ways.

- blink_1: Toggles LED and wait fixed period of time, so that the LED blinks.
- blink_2: Toggles LED and wait some time, then toggles LED and wait another
  time. The total time equals to the period in blink_1. As a result the LED
  blinks in a different way.

Which mode is executed is controlled by on-board switch button.

Drivers & middleware
--------------------

* SPI NOR Flash
* STDIO
* Synchronous USART
* GPIO

Default interface settings
--------------------------

* USART

  * No parity
  * 8-bit character size
  * 1 stop bit
  * 115200 baud-rate
