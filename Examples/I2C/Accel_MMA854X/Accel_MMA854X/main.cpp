/*
 * Accel_MMA854X.cpp
 *
 * Created: 5/28/2020 3:19:05 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library
//#include "stdio_start.h"
//#include "stdio_test.h"
MMA8452Q accel;


int main(void)
{
	atmel_start_init();
	 accel.init();
	
    /* Replace with your application code */
    while (1) 
    {
		  if (accel.available())
		  {
			  // First, use accel.read() to read the new variables:
			  accel.read();
			  
			  // accel.read() will update two sets of variables.
			  // * int's x, y, and z will store the signed 12-bit values
			  //   read out of the accelerometer.
			  // * floats cx, cy, and cz will store the calculated
			  //   acceleration from those 12-bit values. These variables
			  //   are in units of g's.
			  // Check the two function declarations below for an example
			  // of how to use these variables.
			  //printCalculatedAccels(accel.cx,accel.cy,accel.cz);
			  //printAccels(); // Uncomment to print digital readings
			  
			  // The library also supports the portrait/landscape detection
			  //  of the MMA8452Q. Check out this function declaration for
			  //  an example of how to use that.
			 // printOrientation((int)accel.readPL());
			  
			 // printf("\n\r");
		  }
    }
}
