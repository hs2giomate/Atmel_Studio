/* 
* SPI_Syn_Class.h
*
* Created: 4/6/2020 10:22:28 AM
* Author: GMateusDP
*/


#ifndef __SPI_SYN_CLASS_H__
#define __SPI_SYN_CLASS_H__
#include "driver_init.h"
#include "utils.h"

class SPI_Syn_Class
{
//variables
public:
	volatile bool  ready,xferDone;
protected:
private:
	spi_m_sync_descriptor *SPI;
	io_descriptor *spio;
	spi_xfer  *buffer;
	volatile bool	wFlag,rFlag;
//functions
public:
	SPI_Syn_Class();
	SPI_Syn_Class(spi_m_sync_descriptor *);
	~SPI_Syn_Class();
	void init(void);
	int32_t enable(void);
	void disable(void);
	int32_t write( const uint8_t *  p, int32_t n);
	int32_t  read(uint8_t * p, int32_t n);
	
	
	bool	isSPIReady(void);
	void	SetDescriptor(spi_m_sync_descriptor *);
protected:
	
private:
	SPI_Syn_Class( const SPI_Syn_Class &c );
	SPI_Syn_Class& operator=( const SPI_Syn_Class &c );

}; //SPI_Syn_Class


#endif //__SPI_SYN_CLASS_H__
