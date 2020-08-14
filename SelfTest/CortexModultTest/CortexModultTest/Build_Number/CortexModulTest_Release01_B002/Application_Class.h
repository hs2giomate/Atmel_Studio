/* 
* Application_Class.h
*
* Created: 7/22/2020 12:46:04 PM
* Author: GMateusDP
*/


#ifndef __APPLICATION_CLASS_H__
#define __APPLICATION_CLASS_H__
#include "FlashMemoryClass.h"
#define		POWERON_SINGLE_TIME	3000

class Application_Class
{
//variables
public:
		uint8_t tx_buffer[ISSI_BUFFER_SIZE] ;
	uint8_t rx_buffer[ISSI_BUFFER_SIZE] ;
protected:
private:
	bool is_corrupted;

	uint8_t	modus;
	uint8_t totalPins;
	uint32_t	addressCounter;
	uint32_t	errorCounter,succesCounter,succesLimit;

//functions
public:
	Application_Class();
	~Application_Class();
	void		Init(void);
	void		Run(void);
	bool		FastMemoryCheck(void);
	
protected:
private:
	Application_Class( const Application_Class &c );
	Application_Class& operator=( const Application_Class &c );
	uint32_t	NextAddress(void);
	bool		IsDataCorrupted(void);
	bool		CheckFlashMemory(void);
	void		RunOnMode(void);
	void		ChangeModus(void);
	
	uint8_t		FillTXBuffer(void);
}; //Application_Class
extern	Application_Class	app;
#endif //__APPLICATION_CLASS_H__
