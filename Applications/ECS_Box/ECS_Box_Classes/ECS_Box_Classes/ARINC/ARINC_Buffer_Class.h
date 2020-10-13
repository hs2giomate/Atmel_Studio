/* 
* ARINC_Buffer_Class.h
*
* Created: 10/9/2020 3:47:31 PM
* Author: GMateusDP
*/


#ifndef __ARINC_BUFFER_CLASS_H__
#define __ARINC_BUFFER_CLASS_H__
#include "ARINC_Conversions.h"
#include "Protocol_Class.h"
#define TX_LABELS_NUMBER 32
#define RX_LABELS_NUMBER 32
#define  COCKPITS_TEMPERATURE 374

class ARINC_Buffer_Class: private virtual ARINC_Conversions,private virtual Protocol_Class
{
//variables
public:
	uint8_t LabelsArrayTX[TX_LABELS_NUMBER];                // All Rec1 32 labels
	uint8_t LabelsArrayRX1[RX_LABELS_NUMBER];                // All Rec2 256 labels
	uint8_t  LabelsArrayRX2[RX_LABELS_NUMBER];                // All Rec2 256 labels
	uint8_t receiverBuffer[RX_LABELS_NUMBER][4];             // [# of buffers][16 bytes]
	uint8_t	transmitBuffer[TX_LABELS_NUMBER][4];
protected:
private:
	uint8_t index,octalLabel;
	uint32_t data;
	uint8_t	localArray[4];
//functions
public:
	ARINC_Buffer_Class();
	~ARINC_Buffer_Class();
	void SaveTemperature(void);
protected:
private:
	ARINC_Buffer_Class( const ARINC_Buffer_Class &c );
	ARINC_Buffer_Class& operator=( const ARINC_Buffer_Class &c );

}; //ARINC_Buffer_Class

#endif //__ARINC_BUFFER_CLASS_H__
