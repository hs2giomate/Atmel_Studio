/* ========================================================================== *
* AST Köhler GmbH
* Johannesstr. 14-16
* D-70794 Filderstadt
*
*/
/**\file interface.h
* <b>Project: CONT12KW</b>
*
* Compiler:  Microchip XC16 Compiler v1.60\n
* Controller: dsPIC33FJ128MC706A
*
*
* \date 1.12.2020
* \author Frank Köhler AST Köhler GmbH
*
* \brief  This is the interface Header file.\n
* $Revision:  $
*/
/*
* $Id: interface.h   $
*** ========================================================================== */

#ifndef INTERFACE_CCU_H
#define	INTERFACE_CCU_H

#ifdef	__cplusplus
extern "C" {
	#endif
/*!< Type definition of status flags*/
typedef union tagSTATUSFLAGS
{
  struct
  {
      unsigned motor_ready:1;           //!< 1: motor control selftest ok
      unsigned temp_ready:1;            //!< 1: temperature sensors selftest ok
      unsigned tick: 1;			//!< gets set every miilisecond
      unsigned mtick :1;		//!< copy of this for motor control

      unsigned pstick : 1;		//!< 400ms Anschlag
      unsigned run_ok :1;		//!< 1: ready to switch on motor
      unsigned : 1;              //!< 1: not used
      unsigned : 1;             //!< 1: not used

      unsigned sekunde : 1;		//!< 1: Sekunde antriggern
      unsigned mtesten : 1;             //!< 1: motor testmode active
      unsigned testen : 1;              //!< 1: Output testmode active
      unsigned enable : 1;              //!< 1: Enable input active

      unsigned disablepc : 1;           //!< 1: Input current limit inputs disabled
      unsigned crc_ok : 1;              //!< 1: CRC ok
      unsigned input_on : 1;
      unsigned  : 1;
  } b;
  unsigned int w;
} STATUSFLAGS;

/*!< Type definition of error flags*/
typedef union tagERRORFLAGS 
{
  struct
  {
    unsigned ptfault_mot: 1;     //!< 1: Self test error on Motor temperature sensor
    unsigned ptfault_pwr: 1;     //!< 1: Self test error on Controller temperature sensor
    unsigned ptfault_hs: 1;      //!< 1: Self test error on Heatsink sensor
    unsigned ptfault_atru: 1;    //!< 1: Self test error on ATRU sensor

    unsigned : 1;
    unsigned faulta: 1;          //!< 1: Hardware overcurrent condition detected
    unsigned faultb: 1;          //!< 1: Hardware Overvoltage/Undervoltage/high Ripple  condition detected
    unsigned undervoltage: 1;    //!< 1: Undervoltage condition detected

    unsigned overvoltage:1;      //!< 1: Overvoltage condition detected
    unsigned overcurrent:1;      //!< 1: Overcurrent condition detected 
    unsigned overheat: 1;        //!< 1: Controller overheated
    unsigned overheat_motor: 1;  //!< 1: Motor overheated

    unsigned overheat_trafo: 1;  //!< 1: ATRU overheated
    unsigned disabled : 1;       //!< 1: Hardware fault detection active
    unsigned crc_bad : 1;        //!< 1: FLASH CRC fault
    unsigned sync_fault: 1;      //!< 1: Motor sync failed
  } b;
  unsigned int w;
} ERRORFLAGS;

#define errormask 0x3fff
#define non_recoverable_fault_mask 0x3fff
#define temp_mask 0x1c0f
#define errormask_disabled 0xdfff

typedef union tagMOTORFLAGS 
{
  struct
  {
    unsigned on : 1;
    unsigned run : 1;
    unsigned accelerate : 1;
    unsigned estimate : 1;
    
    unsigned mint :1;
    unsigned sa:1;
    unsigned sb:1;
    unsigned position : 1;
    
    unsigned speed : 1;
    unsigned power : 1;
    unsigned offset : 1;
    unsigned direction : 1;
    
    unsigned start: 1;  //!< 1: Motor starten
    unsigned stop: 1;   //!< 1: Motor stoppen
    unsigned startup: 1;
  } b;
  unsigned int w;
} MOTORFLAGS;








#ifdef	__cplusplus
}
#endif

#endif	/* INTERFACE__CCU_H */

