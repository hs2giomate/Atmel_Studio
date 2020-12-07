/* 
* ARINC_Handler_Class.cpp
*
* Created: 11/30/2020 11:24:32 AM
* Author: GMateusDP
*/


#include "ARINC_Handler_Class.h"


static ARINC_Interface staticArinc;



// default constructor
ARINC_Handler_Class::ARINC_Handler_Class()
{
	
	isOK=false;
} //ARINC_Handler_Class

// default destructor
ARINC_Handler_Class::~ARINC_Handler_Class()
{
} //~ARINC_Handler_Class


bool ARINC_Handler_Class::Init_ARINC(void){
	ar=&staticArinc;
	isOK=ar->Init();
	
	return isOK;
}

bool ARINC_Handler_Class::R1GotMessage(void){
	return ar->newMessageR1;
}
bool ARINC_Handler_Class::HandleCommunication(void){
	ar->ReadRXBuffer(1);
	return ar->newMessageR1;
}

bool ARINC_Handler_Class::PollData(void){
	ar->TransmitTXBuffer();
	return false;
}




ARINC_Handler_Class arinc;