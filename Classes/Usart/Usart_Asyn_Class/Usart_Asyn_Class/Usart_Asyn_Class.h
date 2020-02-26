/* 
* Usart_Asyn_Class.h
*
* Created: 2/12/2020 10:21:01 AM
* Author: GMateusDP
*/


#ifndef __USART_ASYN_CLASS_H__
#define __USART_ASYN_CLASS_H__
#include "driver_init.h"
#include "utils.h"
#ifndef USART_0_BUFFER_SIZE
	#define USART_0_BUFFER_SIZE 16
	static uint8_t USART_0_buffer[USART_0_BUFFER_SIZE];
#endif

class Usart_Asyn_Class
{
//variables
public:
	volatile bool  txReady, rxReady;
	struct io_descriptor *usartio;
	struct usart_async_descriptor USART0;
protected:
private:
	uint8_t portNumber;
	
	Usart_Asyn_Class* pUsartClass;
//functions
public:
	Usart_Asyn_Class();
	Usart_Asyn_Class(uint8_t p);
	~Usart_Asyn_Class();
	
	void init(void);
	int32_t enable(void);
	void disable(void);
	int32_t write( const uint8_t *  p, int32_t n);
	int32_t  read(uint8_t * p, int32_t n);
	void  setUsartTxReady(void);
	void  setUsartRxReady(void);
	
	bool	isUsartReady(void);
protected:
private:
	Usart_Asyn_Class( const Usart_Asyn_Class &c );
	Usart_Asyn_Class& operator=( const Usart_Asyn_Class &c );

}; //Usart_Asyn_Class

#endif //__USART_ASYN_CLASS_H__
