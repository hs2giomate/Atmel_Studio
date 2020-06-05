/*
 * stdio_test.h
 *
 * Created: 5/29/2020 10:47:18 AM
 *  Author: GMateusDP
 */ 


#ifndef STDIO_TEST_H_
#define STDIO_TEST_H_

 #ifdef __cplusplus
	 extern "C" {
 #endif
 	#include "atmel_start.h"
 	#include "stdio_start.h"
	 	#define PORTRAIT_U 0
	 	#define PORTRAIT_D 1
	 	#define LANDSCAPE_R 2
	 	#define LANDSCAPE_L 3
	 	#define LOCKOUT 0x40

	 void printAccels(int x,int y,int z);


	 // This function demonstrates how to use the accel.cx, accel.cy,
	 //  and accel.cz variables.
	 // Before using these variables you must call the accel.read()
	 //  function!
	 void printCalculatedAccels(float cx,float cy, float cz);


	 // This function demonstrates how to use the accel.readPL()
	 // function, which reads the portrait/landscape status of the
	 // sensor.
	 void printOrientation(int pl);


 #ifdef __cplusplus
	}
 #endif



#endif /* STDIO_TEST_H_ */