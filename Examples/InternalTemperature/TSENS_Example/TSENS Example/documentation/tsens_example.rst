=============
TSENS Example
=============

This example periodically reads data from two temperature sensors using the ADC 
driver with the period of a second. Using the two conversion results and the 
temperature calibration parameters found in the NVM Software Calibration Area, 
the die temperature T can be calculated. The temperature value in Celsius will 
be printed to console via the USART driver.


Drivers
-------
* STDIO Redirect
* ADC
* Delay

Default interface settings
--------------------------
* USART

  * Baud rate: 38400
  * Data bits: 8 
  * Stop bits: 1
  * Parity: None
  * Flow control: None

* ADC

  * 1.0V reference voltage
  * 12-bit resolution
