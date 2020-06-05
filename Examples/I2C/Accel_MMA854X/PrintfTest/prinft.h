/*
 * prinft.h
 *
 * Created: 5/29/2020 9:54:08 AM
 *  Author: GMateusDP
 */ 


#ifndef PRINFT_H_
#define PRINFT_H_
 #ifdef __cplusplus
 extern "C" {
	 #endif
  #include "stdlib.h"
  #include "string.h"
  //#include "driver_init.h"
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

#endif /* PRINFT_H_ */