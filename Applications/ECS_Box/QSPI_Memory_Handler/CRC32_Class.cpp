/* 
* CRC32_Class.cpp
*
* Created: 7/8/2020 5:40:56 PM
* Author: GMateusDP
*/


#include "CRC32_Class.h"
#include "math.h"

// default constructor
CRC32_Class::CRC32_Class()
{
} //CRC32_Class

// default destructor
CRC32_Class::~CRC32_Class()
{
} //~CRC32_Class

uint32_t	CRC32_Class::Init(uint32_t refCRC/* =0xFFFFFFFF */){
	crc_sync_enable(&CRC_CALC);
	refCRC32=refCRC;
	return	refCRC32;
}

uint32_t	CRC32_Class::CalculateCRC(uint32_t *const data, const uint32_t len,uint32_t crc){
	crc_sync_enable(&CRC_CALC);
	uint32_t	lenght32=(uint32_t)floor(len/4);
	crc_sync_crc32(&CRC_CALC, data, lenght32, &crc);
	return (crc^0xFFFFFFFF);
}

uint32_t	CRC32_Class::CalculateCRC(uint32_t *const data, const uint32_t len){
	uint32_t	lenght32=(uint32_t)floor(len/4);
	uint32_t crc=Init();
	crc_sync_crc32(&CRC_CALC, data, lenght32, &crc);
	return (crc^0xFFFFFFFF);
}