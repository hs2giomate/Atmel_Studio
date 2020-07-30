/* 
* N25Q256_Class.h
*
* Created: 6/26/2020 9:33:38 AM
* Author: GMateusDP
*/


#ifndef __N25Q256_CLASS_H__
#define __N25Q256_CLASS_H__

#include "atmel_start.h"
#include "spi_nor_flash_main.h"
#include "conf_spi_nor_flash.h"
#include "RingBuffer.h"

	



#define QSPI_BUFFER_SIZE 256
/** Size of minimum erase block */
#define QSPI_ERBLK (4 * 1024)
/** Size of data to erase (blocks to cover writing area) */
#define QSPI_ERSIZE ((QSPI_BUFFER_SIZE + QSPI_ERBLK - 1) & ~(QSPI_ERBLK - 1))



class N25Q256_Class
{
//variables
public:
		volatile bool xferDone;

protected:
private:
	qspi_sync_descriptor  *ptrQSPIDescriptor;
	
	uint8_t tx_buffer[QSPI_BUFFER_SIZE];
	uint8_t rx_buffer[QSPI_BUFFER_SIZE];

	spi_nor_flash *SPI_NOR_FLASH;
	static n25q256a SPI_NOR_FLASH_descr;
	uint32_t	memoryCapacity;
	bool			is_corrupted,isOK;
	uint8_t		status;
	_qspi_command cmd;
	uint32_t	currentAddress;
	

//functions
public:
	N25Q256_Class();
	N25Q256_Class(qspi_sync_descriptor *);
	~N25Q256_Class();
	bool Init(void);
	void Init(qspi_sync_descriptor *);
	uint32_t	Erase(uint32_t);
	uint32_t	WriteAddress(uint8_t *p,uint32_t addr, uint32_t size );
	uint32_t	ReadAddress(uint8_t *p,uint32_t addr, uint32_t size);

	uint32_t	GetMemoryCapacity(void);
	bool		IsReady(void);
	bool		WaitOnBusy(void);
protected:

private:
	N25Q256_Class( const N25Q256_Class &c );
	N25Q256_Class& operator=( const N25Q256_Class &c );
	void spi_nor_flash_init();
	bool	SelfTest(void);
	uint8_t		GetStatus(void);

}; //N25Q256_Class
extern	N25Q256_Class	flash;
#endif //__N25Q256_CLASS_H__
