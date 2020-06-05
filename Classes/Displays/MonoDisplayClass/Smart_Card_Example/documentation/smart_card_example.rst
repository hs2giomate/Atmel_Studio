==================
Smart card Example
==================

This example communicates with a smart card (AT88SC1616C) via ISO7816, reset the
smart card, send command to the smart card through user choice via EDBG VCP and
receive the appropriate response from the smart card.

The files iso7816.c and iso7816.h software provide in this example is used to
transform APDU commands to TPDU commands for the smart card. The ISO7816 provided
here is for the protocol T=0 only. The sending and reception of a character
is made under polling mode.

The example application uses the USART driver to communicate with the console
via EDBG Virtual COM Port.

Hardwares
---------
* One smart card Xplained Pro extension wing
* One of SAM MCU Xplained Pro kit

  * SAM E54 Xplained Pro kit, EXT2 is used to connect Smart card wing
  * SAM L10/L11 Xplained Pro kit, EXT1 is used to connect Smart card wing

    * Use a wire to connect pin PA09 and PA17 on EXT1 to provide clock to smart card
* One smart card (AT88SC1616C Atmel CryptoMemory in ISO Module form)

Drivers
-------
* USART
* GPIO

Middleware
----------
* STDIO

Default interface settings
--------------------------
* USART

  * No parity
  * 8-bit character size
  * 1 stop bit
  * 9600 baud-rate

* USART with ISO7816

  * Even parity
  * 8-bit character size
  * 1 stop bit
  * 9600 baud-rate
  * 3.57MHz clock
