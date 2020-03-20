/* 
* Samba_Boot.h
*
* Created: 3/2/2020 12:43:29 PM
* Author: GMateusDP
*/


#ifndef __SAMBA_BOOT_H__
#define __SAMBA_BOOT_H__
#include "sam.h"
#ifndef BOOT_USART_MODULE
#define BOOT_USART_MODULE					SERCOM5
#endif

class Samba_Boot
{
//variables
public:
protected:
private:
Samba_Boot *Boot_ptr;

//functions
public:
	Samba_Boot();
	~Samba_Boot();
	
	void check_Boot_Key(void);
	friend void SERCOM5_Handler(void);
	void start_booting(void);
	void sendSharp(void);
	friend class DeviceClass;
protected:
private:
	Samba_Boot( const Samba_Boot &c );
	Samba_Boot& operator=( const Samba_Boot &c );

}; //Samba_Boot



void SERCOM5_Handler(void);

#endif //__SAMBA_BOOT_H__
