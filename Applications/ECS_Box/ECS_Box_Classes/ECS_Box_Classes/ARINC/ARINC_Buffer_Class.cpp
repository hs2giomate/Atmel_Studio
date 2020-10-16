/* 
* ARINC_Buffer_Class.cpp
*
* Created: 10/9/2020 3:47:30 PM
* Author: GMateusDP
*/


#include "ARINC_Buffer_Class.h"
#include "TemperatureSensors_Class.h"
#include "string.h"

static  uint8_t bufferLabelsArrayTX[TX_LABELS_NUMBER];                // All Rec1 32 labels
static	uint8_t bufferLabelsArrayRX1[RX_LABELS_NUMBER];                // All Rec2 256 labels
static	uint8_t bufferLabelsArrayRX2[RX_LABELS_NUMBER];                // All Rec2 256 labels
static 	uint8_t bufferReceiverBuffer[RX_LABELS_NUMBER][4];             // [# of buffers][16 bytes]
static	uint8_t	bufferTransmitBuffer[TX_LABELS_NUMBER][4];


// default constructor
ARINC_Buffer_Class::ARINC_Buffer_Class()
{
	

	//receiverBuffer=            // [# of buffers][16 bytes]
	//transmitBuffer=&bufferTransmitBuffer[0];
} //ARINC_Buffer_Class

// default destructor
ARINC_Buffer_Class::~ARINC_Buffer_Class()
{
} //~ARINC_Buffer_Class

void	ARINC_Buffer_Class::InitBuffer(void){
	LabelsArrayTX=&bufferLabelsArrayTX[0];
	LabelsArrayRX1=&bufferLabelsArrayRX1[0];
	LabelsArrayRX2=&bufferLabelsArrayRX2[0];
		for (uint8_t i = 0; i < TX_LABELS_NUMBER; i++)
		{
			pointerArrayRX[i]=bufferReceiverBuffer[i];
			pointerArrayTX[i]=bufferTransmitBuffer[i];
		}
		receiverBuffer=pointerArrayRX;
		transmitBuffer=pointerArrayTX;
}

void ARINC_Buffer_Class::SaveTemperature(void){
	octalLabel=Label2Byte(COCKPITS_TEMPERATURE);
	index=GetIndexTXLabelarray(octalLabel,LabelsArrayTX);
	float temperature=temperatures.values[0][1];
	data=MakeLabel374(FloatTwoComplement(temperature,15),temperature);
	Uint32FourBytesArray(data,localArray);
	memcpy(transmitBuffer[index],localArray,4);
}

void ARINC_Buffer_Class::SaveStatus(uint32_t st){
	octalLabel=Label2Byte(ECS_COMMAND_ACKNOWLEDGE);
	index=GetIndexTXLabelarray(octalLabel,LabelsArrayTX);
	data=MakeLabel270(st);
	Uint32FourBytesArray(data,localArray);
	memcpy(transmitBuffer[index],localArray,4);
}

void ARINC_Buffer_Class::SortReceivedData(uint8_t* receiver){
		uint8_t i,j,l;
		for (i = 0; i <RX_LABELS_NUMBER ; i++)
		{
			if (LabelsArrayRX1[i]>0)
			{
			
					if (receiver[3]==LabelsArrayRX1[i])
					{
						memcpy(receiverBuffer[i],receiver,4);
					}
			
	
			}
		}
}