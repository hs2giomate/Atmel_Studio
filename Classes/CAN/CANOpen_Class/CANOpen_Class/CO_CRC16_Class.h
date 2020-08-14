/* 
* CO_CRC16_Class.h
*
* Created: 8/10/2020 4:09:50 PM
* Author: GMateusDP
*/


#ifndef __CO_CRC16_CLASS_H__
#define __CO_CRC16_CLASS_H__


class CO_CRC16_Class
{
//variables
public:
protected:
private:

//functions
public:
	CO_CRC16_Class();
	~CO_CRC16_Class();
	unsigned short crc16_ccitt(
	const unsigned char     block[],
	unsigned int            blockLength,
	unsigned short          crc);
protected:
private:
	CO_CRC16_Class( const CO_CRC16_Class &c );
	CO_CRC16_Class& operator=( const CO_CRC16_Class &c );

}; //CO_CRC16_Class

#endif //__CO_CRC16_CLASS_H__
