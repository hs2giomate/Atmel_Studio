/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

/*
  Multiple Serial test

  Receives from the main serial port, sends to the others.
  Receives from serial port 1, sends to the main serial (Serial 0).

  This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc.

  The circuit:
  - any serial device attached to Serial port 1
  - Serial Monitor open on Serial port 0

  created 30 Dec 2008
  modified 20 May 2012
  by Tom Igoe & Jed Roach
  modified 27 Nov 2015
  by Arturo Guadalupi

  This example code is in the public domain.
*/
float floatvalue=1.23456;

void setup() {
  // initialize both serial ports:
 
  SerialUSB.begin(9600);
  
  
}

void loop() {
  // read from port 1, send to port 0:


  if (SerialUSB.available()) {
    int inByte = SerialUSB.read();
    if (inByte>33)
    {
		  SerialUSB.println(inByte);
		  SerialUSB.println("it works");
		  SerialUSB.println(floatvalue);
    }
  
	
  }
  delay(100);
}
