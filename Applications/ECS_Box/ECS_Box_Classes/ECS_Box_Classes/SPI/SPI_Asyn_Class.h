/* 
* SPI_Asyn_Class.h
*
* Created: 2/5/2020 12:17:57 PM
* Author: GMateusDP
*/

#ifndef __SPI_ASYN_CLASS_H__
#define __SPI_ASYN_CLASS_H__

#include "driver_init.h"
//#include "utils.h"
#define  SPI_ASYN_BUFFER_SIZE	32



class SPI_Asyn_Class
{

//variables
public:
	volatile bool  xferDone;
	 uint8_t	*txBuffer;
	 uint8_t	*rxBuffer;
protected:
private:
	uint8_t portNumber;
	void (*SPICallBackHandler)(void);
	 SPI_Asyn_Class* pSPIClass;
	 int	callbackCounter;
	 io_descriptor *spio;
	 spi_m_async_descriptor *SPIA;
	 spi_m_async_status stat;
	
		
//functions
public:
	SPI_Asyn_Class();
	SPI_Asyn_Class(spi_m_async_descriptor *spi);
	~SPI_Asyn_Class();

	void init(void);
	int32_t enable(void);
	void disable(void);
	int32_t Write( const uint8_t *  p, int32_t n);
	int32_t  Read(uint8_t * p, int32_t n);
	void  setSPIReady(void);
	void	set_descriptor(spi_m_async_descriptor *spi);
	bool	isSPIReady(void);
	bool	isTXDone(void);
	bool	isRXDone(void);
	bool	isBusy(void);


protected:
private:
	SPI_Asyn_Class( const SPI_Asyn_Class &c );
	SPI_Asyn_Class& operator=( const SPI_Asyn_Class &c );
	//void  SPICallbackHandler(void);
	

}; //SPI_Asyn_Class

#endif //__SPI_ASYN_CLASS_H__
