/*
 * printf.c
 

 * Created: 5/29/2020 9:55:57 AM
 *  Author: GMateusDP
 */ 

 

	 
	  #include "prinft.h"
		#define PORTRAIT_U 0
		#define PORTRAIT_D 1
		#define LANDSCAPE_R 2
		#define LANDSCAPE_L 3
		#define LOCKOUT 0x40
	 
	 	char	buffer[64];
	 	char	*p=&buffer[0];
	 	void printAccels(int x,int y,int z)
	 	{
			 /*
		 	p=itoa(x,&buffer[0],10);
		 	io_write(stdioDescriptor,(uint8_t *)p,strlen(buffer));
		 	io_write(stdioDescriptor, (uint8_t *)"\t", 2);
		 	p=itoa(y,&buffer[0],10);
		 	io_write(stdioDescriptor,(uint8_t *)p,strlen(buffer));
		 	io_write(stdioDescriptor, (uint8_t *)"\t", 2);
		 	p=itoa(z,&buffer[0],10);
		 	io_write(stdioDescriptor,(uint8_t *)p,strlen(buffer));
		 	io_write(stdioDescriptor, (uint8_t *)"\t", 2);
			 */
		 	printf("\t");
	 	}

	 	// This function demonstrates how to use the accel.cx, accel.cy,
	 	//  and accel.cz variables.
	 	// Before using these variables you must call the accel.read()
	 	//  function!
	 	void printCalculatedAccels(float cx,float cy, float cz)
	 	{
			 /*
		 	p=utoa(cx,&buffer[0],10);
		 	io_write(stdioDescriptor,(uint8_t *)p,strlen(buffer));
		 	io_write(stdioDescriptor, (uint8_t *)"\t", 2);
		 	p=utoa(cy,&buffer[0],10);
		 	io_write(stdioDescriptor,(uint8_t *)p,strlen(buffer));
		 	io_write(stdioDescriptor, (uint8_t *)"\t", 2);
		 	p=utoa(cz,&buffer[0],10);
		 	io_write(stdioDescriptor,(uint8_t *)p,strlen(buffer));
		 	io_write(stdioDescriptor, (uint8_t *)"\t", 2);
			 */
	 	}

	 	// This function demonstrates how to use the accel.readPL()
	 	// function, which reads the portrait/landscape status of the
	 	// sensor.
	 	void printOrientation(int pl)
	 	{
		 	// accel.readPL() will return a byte containing information
		 	// about the orientation of the sensor. It will be either
		 	// PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
		 	// LOCKOUT.
		 
		 	switch (pl)
		 	{
			 	case PORTRAIT_U:
			 	
			 	p="Portrait Up";
				 printf(p);
			 //	io_write(stdioDescriptor, (uint8_t *)p, strlen(p));
			 	break;
			 	case PORTRAIT_D:
			 	p="Portrait Down";
				 	 printf(p);
			 //	io_write(stdioDescriptor, (uint8_t *)p, strlen(p));
			 	break;
			 	case LANDSCAPE_R:
			 	p="Landscape Right";
				 	 printf(p);
			 //	io_write(stdioDescriptor, (uint8_t *)p, strlen(p));
			 	break;
			 	case LANDSCAPE_L:
			 	p="Landscape Left";
				 	 printf(p);
			 //	io_write(stdioDescriptor, (uint8_t *)p, strlen(p));
			 	break;
			 	case LOCKOUT:
			 	p="Flat";
				 	 printf(p);
			 //	io_write(stdioDescriptor, (uint8_t *)p, strlen(p));
			 	break;
		 	}
	 	}
	 
