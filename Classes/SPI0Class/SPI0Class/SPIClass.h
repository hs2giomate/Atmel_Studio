/*
 * SPIClass.h
 *
 * Created: 1/30/2020 9:01:34 AM
 *  Author: GMateusDP
 */ 


#ifndef SPICLASS_H_
#define SPICLASS_H_

#include "driver_init.h"
#include "utils.h"
enum
{
	kSPI0 = 0,	//!	SPI0
	kNumberOfSPI
};

 class	SPIClass
 {
	public:
	SPIClass(void);
	SPIClass(uint8_t p);
	~SPIClass(void);
	
	struct io_descriptor *spio;
	struct spi_m_sync_descriptor SPI0;
	void init(void);
	int32_t enable(void);
	void disable(void);
	int32_t write( const uint8_t *  p, int32_t n);
	int32_t  read(uint8_t * p, int32_t n);
	//friend FlashMemoryClass
	
	private:
	uint8_t portNumber;
	
	};


#endif /* SPICLASS_H_ */