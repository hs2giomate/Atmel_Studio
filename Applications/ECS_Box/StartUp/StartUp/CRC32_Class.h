/* 
* CRC32_Class.h
*
* Created: 7/8/2020 5:40:57 PM
* Author: GMateusDP
*/


#ifndef __CRC32_CLASS_H__
#define __CRC32_CLASS_H__
#include "driver_init.h"
COMPILER_ALIGNED(4)
class CRC32_Class
{
//variables
public:
protected:
private:
	uint32_t	refCRC32;
//functions
public:
	CRC32_Class();
	~CRC32_Class();
	uint32_t	Init(uint32_t refCRC=0xFFFFFFFF);
	uint32_t	CalculateCRC(uint32_t *const data, const uint32_t len,uint32_t crc);
	uint32_t	CalculateCRC(uint32_t *const data, const uint32_t len);
protected:
private:
	CRC32_Class( const CRC32_Class &c );
	CRC32_Class& operator=( const CRC32_Class &c );

}; //CRC32_Class

#endif //__CRC32_CLASS_H__
