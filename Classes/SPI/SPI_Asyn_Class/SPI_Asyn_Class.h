/* 
* SPI_Asyn_Class.h
*
* Created: 2/5/2020 12:17:57 PM
* Author: GMateusDP
*/

#ifndef __SPI_ASYN_CLASS_H__
#define __SPI_ASYN_CLASS_H__

#include "driver_init.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
	#endif
//static void complete_cb_SPI_0(const struct spi_m_async_descriptor *const io_descr) __attribute__ ((signal));

#ifdef __cplusplus
}
#endif

//static SPICallBackHandler	EICHandler;
//static void complete_cb_SPI_0(const struct spi_m_async_descriptor *const io_descr);
enum
{
	kSPI0 = 0,	//!	SPI0
	kNumberOfSPI
};
class SPI_Asyn_Class
{
//friend  void complete_cb_SPI_0(const struct spi_m_async_descriptor *const io_descr);
//variables
public:
	 volatile bool  ready;
	struct io_descriptor *spio;
	struct spi_m_async_descriptor SPI0;
protected:
private:
	uint8_t portNumber;
	void (*SPICallBackHandler)(void);
	 SPI_Asyn_Class* pSPIClass;
		
//functions
public:
	SPI_Asyn_Class();
	SPI_Asyn_Class(uint8_t p);
	~SPI_Asyn_Class();

	void init(void);
	int32_t enable(void);
	void disable(void);
	int32_t write( const uint8_t *  p, int32_t n);
	int32_t  read(uint8_t * p, int32_t n);
	void  setSPIReady(void);
	
	bool	isSPIReady(void);
	//static void ISRFunc(){pSPIClass->setSPIReady();}
	// void classInterruptHandler(void);
	//void setCallback(void (*userDefinedCallback)(void)){ SPICallBackHandler= userDefinedCallback; }

protected:
private:
	SPI_Asyn_Class( const SPI_Asyn_Class &c );
	SPI_Asyn_Class& operator=( const SPI_Asyn_Class &c );
	//void  SPICallbackHandler(void);
	

}; //SPI_Asyn_Class

#endif //__SPI_ASYN_CLASS_H__
