/* 
* MemoryManagment_Class.cpp
*
* Created: 7/2/2020 10:33:10 AM
* Author: GMateusDP
*/


#include "MemoryFlash_Class.h"
//#include "AT24MAC_Class.h"
#include "CDC_Class.h"

static	uint8_t	flashBufferStatic[QSPI_ERBLK/1];


// default constructor
MemoryFlash_Class::MemoryFlash_Class()
{
	_currentAddress=0;
	head=512;
	tail=0;
	flashBuffer=&flashBufferStatic[0];
	
	stackpointerEventloger=(uint32_t)&flashMap->EventsLogStateSector;
	
} //MemoryManagment_Class



// default destructor
MemoryFlash_Class::~MemoryFlash_Class()
{
} //~MemoryManagment_Class

bool	MemoryFlash_Class::Init(uint32_t flashChipSize){
		if (qspiFlash.Init())
		{
			usb<<"Flash Memory SelfTest PASSED"<<NEWLINE;
		} 
		else
		{
			usb<<"Flash Memory SelfTest FAILED"<<NEWLINE;
		}
				
		SetChipID(flashChipSize);
		return true;
}

uint32_t MemoryFlash_Class::GetNextAvailableAddress(uint16_t size) {
	if (!AddressCheck(currentAddress, logArea.capacity)){
		return false;
	}

	return head;
}
 bool MemoryFlash_Class::AddressCheck(uint32_t _addr, uint32_t size){
	// uint32_t _submittedAddress = _addr;
	// uint8_t _errorcode = GetErrorCode();
// 	 if (_errorcode == UNKNOWNCAP || _errorcode == NORESPONSE) {
// 		 return false;
// 	 }
// 	 if (!logArea.capacity) {
// 		 _troubleshoot(CALLBEGIN);
// 		 return false;
// 	 }
// 	 if (_submittedAddress + size >= logArea.capacity) {
// 
// 		 #ifdef DISABLEOVERFLOW
// 		 _troubleshoot(OUTOFBOUNDS);
// 		 return false;					// At end of memory - (!pageOverflow)
// 		 #else
// 		 _addressOverflow = ((_submittedAddress + size) - logArea.capacity);
// 		 _currentAddress = _addr;
// 
// 		 return true;					// At end of memory - (pageOverflow)
// 		 #endif
// 	 }
// 	 else {
// 		 _addressOverflow = false;
// 		 _currentAddress = _addr;
// 		 return true;				// Not at end of memory if (address < _chip.capacity)
// 	 }
	return true;
 }
 uint8_t	MemoryFlash_Class::GetErrorCode(){
	 
	 // PLease don forget to do the real impelementation
	 return	0;
 }
  void MemoryFlash_Class::_troubleshoot(uint8_t _code, bool printoverride) {
	//  diagnostics.troubleshoot(_code, printoverride);
  }

 bool MemoryFlash_Class::SetChipID(uint32_t flashChipSize) {
	 //set some default values

// 	 logArea.capacity =GetMemoryCapacity();
// 
// 
// 	 if (!logArea.capacity) {
// 		 #ifdef RUNDIAGNOSTIC
// 		 usb.println(F("Chip capacity cannot be identified"));
// 		 #endif
// 		 if (flashChipSize) {
// 			 // If a custom chip size is defined
// 			 #ifdef RUNDIAGNOSTIC
// 			 usb.println(F("Custom Chipsize defined"));
// 			 #endif
// 			 logArea.capacity = flashChipSize;
// 			 logArea.supported = false;
// 			 return true;
// 		 }
// 
// 		 else {
// 			 _troubleshoot(UNKNOWNCAP);
// 			 return false;
// 		 }
// 
// 	 }
	 return true;
 }
 
 uint32_t	MemoryFlash_Class::SaveRingBuffer(void){
	 uint32_t	i = NextIndex(head);
	 uint32_t	w;
	   if ( i != tail )
	   {
		w=   qspiFlash.WriteAddress((uint8_t*)PTR_RINGBUFFER(&ringBuffer),head,sizeof(ringBuffer));
		   head = i ;
	   }
	   return	w;
 }
 uint32_t MemoryFlash_Class::NextIndex(int index)
 {
	 return (uint32_t)(index +QSPI_BUFFER_SIZE ) % logArea.capacity;
 }
 
 uint32_t	MemoryFlash_Class::PagesAvailable(void){
	  if (head >= tail){
		  return N25Q_FLASH_SIZE-QSPI_BUFFER_SIZE  - head + tail;
	  }else{
		  return tail - head - QSPI_BUFFER_SIZE;
	  }	 	  
 }
 
 uint32_t	MemoryFlash_Class::ReadPage(RingBuffer *p){
	   if(tail == head)
	   return -1;

	   uint32_t value = qspiFlash.ReadAddress((uint8_t*)PTR_RINGBUFFER(p),tail,sizeof(ringBuffer));
	   tail = NextIndex(tail);

	   return value;
 }
 
 bool MemoryFlash_Class::IsFull(void)
 {
	 return (NextIndex(head) == tail);
 }
 
 uint32_t	MemoryFlash_Class::ReadControllerState(void){
	 
	 uint32_t	r=ReadControllerState(&appState);
	 return	r;
 }
  uint32_t	MemoryFlash_Class::ReadControllerState(ECSConfiguration *cs){
	  handlerCtrState=PTR_CTR_STATE(cs);
	 // uint32_t	r=qspiFlash.ReadAddress((uint8_t*)handlerCtrState,APP_STATE_ADDRESS,sizeof(appState));
	  return	0;
  }

uint32_t	MemoryFlash_Class::ReadDeafultApplicationState(HVACState& as){
	handlerAppState=PTR_HVAC_STATE(&as);
	uint32_t	add=(uint32_t )(&flashMap->hvacStateSector.hvacDefaultState);
	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)handlerAppState,add,sizeof(HVACState));
	return	r;
}
// uint32_t	MemoryFlash_Class::ReadDeaful(HVACState& as){
// 	handlerAppState=PTR_HVAC_STATE(&as);
// 	uint32_t	add=(uint32_t )(&flashMap->hvacStateSector.hvacDefaultState);
// 	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)handlerAppState,add,sizeof(HVACState));
// 	return	r;
// }
 uint32_t	MemoryFlash_Class::WriteValidApplicationState(HVACState& hs ){
	uint32_t w=  WriteCurrentState(hs);
	crc32=CalculateCRC((uint32_t *)PTR_HVAC_STATE(&hs),sizeof(HVACState));
	w=WriteCRCAppState(crc32);
	return	w;
 }
 uint32_t	MemoryFlash_Class::WriteValidConfigurationData(ConfigurationData& cd){
	 uint32_t	w=WriteCurrentConfigurationData(cd);
	 crc32=CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&cd),sizeof(ConfigurationData));
	 w=WriteCRCConfigurationData(crc32);
	 return w;
 };
 uint32_t	MemoryFlash_Class::SaveCurrentState(HVACState& hs){
	 uint32_t	add=(uint32_t)&flashMap->hvacStateSector;
	 HVACStateSector	*hss=(HVACStateSector*)&flashBuffer[0];
	 uint32_t r=qspiFlash.ReadAddress((uint8_t*)hss,add,QSPI_ERBLK);
	 if (r==0)
	 {
		 memcpy((void*)&(hss->hvacState),(void*)&hs,sizeof(HVACState));
		 qspiFlash.Erase(add);
		 uint32_t w=qspiFlash.WriteAddress((uint8_t*)hss,add,QSPI_ERBLK);
		  return	w;
	 }

	 return	r;
 }
 uint32_t	MemoryFlash_Class::SaveParameters(UserParameters& up){
	  uint32_t	add=(uint32_t)&flashMap->parametersSector;
	  ParametersSector	*pss=(ParametersSector*)&flashBuffer[0];
	  uint32_t r=qspiFlash.ReadAddress((uint8_t*)pss,add,QSPI_ERBLK);
	  if (r==0)
	  {
		  memcpy((void*)&(pss->parameters),(void*)&up,sizeof(UserParameters));
		  qspiFlash.Erase(add);
		  uint32_t w=qspiFlash.WriteAddress((uint8_t*)pss,add,QSPI_ERBLK);
		  return	w;
	  }

	  return	r;
  }
   uint32_t	MemoryFlash_Class::SaveEventLog(uint8_t *evl){
	  
	
		   if ((stackpointerEventloger+QSPI_ERBLK)>N25Q_FLASH_SIZE)
		   {
			   uint32_t	addRef=(uint32_t)&flashMap->EventsLogStateSector;
			   stackpointerEventloger=addRef;
		   } 
		   else
		   {
			   
		   }
	
		  stackpointerEventloger+=QSPI_ERBLK;
		   uint32_t w=qspiFlash.WriteAddress(evl, stackpointerEventloger,QSPI_ERBLK);
		   return	w;

	 
   }
uint32_t	MemoryFlash_Class::SaveApplicationState(HVACState& hs ){
	  uint32_t w=  SaveCurrentState(hs);
	  crc32=CalculateCRC((uint32_t *)PTR_HVAC_STATE(&hs),sizeof(HVACState));
	  w=SaveCRCAppState(crc32);
	  return	w;
  }
uint32_t	MemoryFlash_Class::SaveCRCAppState(uint32_t crc){
// 	uint32_t	add=(uint32_t)&flashMap->hvacStateSector.crcAppState;
// 	uint32_t	bufferAddres=((uint32_t)&flashBuffer)+add;
// 	memcpy((void*)bufferAddres,&crc,sizeof(uint32_t));
// 	return	bufferAddres;
	return WriteCRCAppState(crc);
}
 
	MemoryFlash_Class	memory;