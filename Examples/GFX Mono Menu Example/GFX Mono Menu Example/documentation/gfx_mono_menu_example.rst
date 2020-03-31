=====================
GFX Mono Menu Example
=====================

OLED1 Xplained pro board must be connected to EXT3 extension header for 
SAMD21/L21/DA1/E54/ Xplained Pro boards and to EXT1 extension header 
for SAMV71/E70/G53/G55/ Xplained Pro boards.

This example draws the menu with two options on the display. BUTTON 3 on OLED1
Xplained board is used for menu navigation, BUTTON 2 is used to choose menu
option. Depending on menu option LED1 on OLED1 Xplained board is on or off.

The External IRQ driver is used to detect button pressing.

Drivers
-------
* Synchronous SPI Master
* External IRQ
* GPIO

Default interface settings
--------------------------
* SPI Master

  * 50000 baud-rate
  * 8-bit character size
