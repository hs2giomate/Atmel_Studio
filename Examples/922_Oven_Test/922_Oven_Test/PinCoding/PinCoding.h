/*--------------------------------------------------------------------------

PinCoding.h

Interface
Ansteuerung Pin Coding Board

Autor: Steffen Simon

$Date: 2017-09-11 15:33:39 +0200 (Mo, 11. Sep 2017) $
$Revision: 7534 $

--------------------------------------------------------------------------*/
#ifndef PIN_CODING_H
#define PIN_CODING_H

#include "coreTypes.h"

/** \brief Ansteuerung LE-Modul: Vaporizer, UDC, IDC, Frequency, Phase
 */
class PinCoding
	{
	public:
		PinCoding(void);
	#if (ZST5_Revision == 0x500)	
		void	setPowerSupplyState(bool on);
		bool	getPowerSupplyState(void) const;
	#endif
		bool	connect(uint16 thresholds[3]);
		bool	updatePinCoding(void);
		uint32	getPinCodingValue(void);
		
		uint16	getPhaseValue(uint8 phase);
		
		void	setPhaseCheckDelayTime(tick_t t);
		bool	handlePeriodicTask(bool throwEventOnPhaseLoss);
	#if (PARTNUMBER == 920)||(PARTNUMBER == 922)
		bool	elementFailureDetected(uint8&);
		bool	thermalFuseFailureDetected(void);
	#endif

	private:
		uint16	readoutPhaseCurrentValue(uint8 phase);

		uint16	averagePhaseCurrentValue[3];
		uint16	phaseLossDetectionThreshold[3];
		
		uint32	code;
		tick_t	nextPhaseCheckTime;
		uint8	bytes[3];
	#if (PARTNUMBER == 920)||(PARTNUMBER == 922)
		uint8	elementFailureCounter[3];
	#endif
		uint8	phaseLossCounter[3];
		bool	adcReady[3];
	};


#endif // PIN_CODING_H
