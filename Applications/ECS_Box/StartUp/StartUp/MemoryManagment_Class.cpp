/* 
* MemoryManagment_Class.cpp
*
* Created: 7/2/2020 10:33:10 AM
* Author: GMateusDP
*/


#include "MemoryManagment_Class.h"
#include "AT24MAC_Class.h"

// default constructor
MemoryManagment_Class::MemoryManagment_Class()
{
	_currentAddress=0;
	head=512;
	tail=0;

	
} //MemoryManagment_Class



// default destructor
MemoryManagment_Class::~MemoryManagment_Class()
{
} //~MemoryManagment_Class

bool	MemoryManagment_Class::Init(uint32_t flashChipSize){
		flash.Init();
		eeprom.Init();
		SetChipID(flashChipSize);
}

uint32_t MemoryManagment_Class::GetNextAvailableAddress(uint16_t size) {
	if (!AddressCheck(currentAddress, logArea.capacity)){
		return false;
	}

	return head;
}
 bool MemoryManagment_Class::AddressCheck(uint32_t _addr, uint32_t size){
	 uint32_t _submittedAddress = _addr;
	 uint8_t _errorcode = GetErrorCode();
	 if (_errorcode == UNKNOWNCAP || _errorcode == NORESPONSE) {
		 return false;
	 }
	 if (!logArea.capacity) {
		 _troubleshoot(CALLBEGIN);
		 return false;
	 }
	 if (_submittedAddress + size >= logArea.capacity) {

		 #ifdef DISABLEOVERFLOW
		 _troubleshoot(OUTOFBOUNDS);
		 return false;					// At end of memory - (!pageOverflow)
		 #else
		 _addressOverflow = ((_submittedAddress + size) - logArea.capacity);
		 _currentAddress = _addr;

		 return true;					// At end of memory - (pageOverflow)
		 #endif
	 }
	 else {
		 _addressOverflow = false;
		 _currentAddress = _addr;
		 return true;				// Not at end of memory if (address < _chip.capacity)
	 }
 }
 uint8_t	MemoryManagment_Class::GetErrorCode(){
	 
	 // PLease don forget to do the real impelementation
	 return	GetReadStatus();
 }
  void MemoryManagment_Class::_troubleshoot(uint8_t _code, bool printoverride) {
	  diagnostics.troubleshoot(_code, printoverride);
  }

 bool MemoryManagment_Class::SetChipID(uint32_t flashChipSize) {
	 //set some default values

	 logArea.capacity =GetMemoryCapacity();


	 if (!logArea.capacity) {
		 #ifdef RUNDIAGNOSTIC
		 usb.println(F("Chip capacity cannot be identified"));
		 #endif
		 if (flashChipSize) {
			 // If a custom chip size is defined
			 #ifdef RUNDIAGNOSTIC
			 usb.println(F("Custom Chipsize defined"));
			 #endif
			 logArea.capacity = flashChipSize;
			 logArea.supported = false;
			 return true;
		 }

		 else {
			 _troubleshoot(UNKNOWNCAP);
			 return false;
		 }

	 }
	 return true;
 }
 
 uint32_t	MemoryManagment_Class::SaveRingBuffer(void){
	 uint32_t	i = NextIndex(head);
	 uint32_t	w;
	   if ( i != tail )
	   {
		w=   flash.WriteAddress((uint8_t*)PTR_RINGBUFFER(&ringBuffer),head,sizeof(ringBuffer));
		   head = i ;
	   }
	   return	w;
 }
 uint32_t MemoryManagment_Class::NextIndex(int index)
 {
	 return (uint32_t)(index +QSPI_BUFFER_SIZE ) % logArea.capacity;
 }
 
 uint32_t	MemoryManagment_Class::PagesAvailable(void){
	  if (head >= tail){
		  return N25Q_FLASH_SIZE-QSPI_BUFFER_SIZE  - head + tail;
	  }else{
		  return tail - head - QSPI_BUFFER_SIZE;
	  }	 	  
 }
 
 uint32_t	MemoryManagment_Class::ReadPage(RingBuffer *p){
	   if(tail == head)
	   return -1;

	   uint32_t value = flash.ReadAddress((uint8_t*)PTR_RINGBUFFER(p),tail,sizeof(ringBuffer));
	   tail = NextIndex(tail);

	   return value;
 }
 
 bool MemoryManagment_Class::IsFull(void)
 {
	 return (NextIndex(head) == tail);
 }
 
 uint32_t	MemoryManagment_Class::ReadControllerState(void){
	 
	 uint32_t	r=ReadControllerState(&appState);
	 return	r;
 }
  uint32_t	MemoryManagment_Class::ReadControllerState(ECSConfiguration *cs){
	  handlerCtrState=PTR_CTR_STATE(cs);
	  uint32_t	r=flash.ReadAddress((uint8_t*)handlerCtrState,APP_STATE_ADDRESS,sizeof(appState));
	  return	r;
  }

uint32_t	MemoryManagment_Class::ReadDeafultApplicationState(HVACState& as){
	handlerAppState=PTR_HVAC_STATE(&as);
	uint32_t	add=(uint32_t )(&flashMap->hvacDefaultState);
	uint32_t	r=flash.ReadAddress((uint8_t*)handlerAppState,add,sizeof(HVACState));
	return	r;
}
 uint32_t	MemoryManagment_Class::WriteValidApplicationState(HVACState& hs ){
	uint32_t w=  WriteCurrentState(hs);
	crc32=CalculateCRC((uint32_t *)PTR_HVAC_STATE(&hs),sizeof(HVACState));
	w=WriteCRCAppState(crc32);
	return	w;
 }
 uint32_t	MemoryManagment_Class::WriteValidConfigurationData(ConfigurationData& cd){
	 uint32_t	w=WriteCurrentConfigurationData(cd);
	 crc32=CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&cd),sizeof(ConfigurationData));
	 w=WriteCRCConfigurationData(crc32);
 };
 
 MemoryManagment_Class	memory;