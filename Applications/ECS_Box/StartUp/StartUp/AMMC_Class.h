/* 
* AMMC_Class.h
*
* Created: 6/19/2020 4:38:41 PM
* Author: GMateusDP
*/


#ifndef __AMMC_CLASS_H__
#define __AMMC_CLASS_H__
#include "ARINC_Interface.h"
#include "ALU_Class.h"

class AMMC_Class: public	ARINC_Interface
{
//variables
public:
	
protected:
	uint8_t	activeLine;
	friend	class	ALU_Class;
private:
	bool selected;

//functions
public:
	AMMC_Class();
	~AMMC_Class();
	bool	IsSelected(void);
	bool	GetSelected(void);
	
protected:
private:
	AMMC_Class( const AMMC_Class &c );
	AMMC_Class& operator=( const AMMC_Class &c );

}; //AMMC_Class

extern	AMMC_Class	ammc1;
extern	AMMC_Class	ammc2;
extern	AMMC_Class	*ammc;

#endif //__AMMC_CLASS_H__
