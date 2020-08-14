/* 
* CBIT_Class.h
*
* Created: 7/8/2020 10:31:45 AM
* Author: GMateusDP
*/


#ifndef __CBIT_CLASS_H__
#define __CBIT_CLASS_H__
typedef struct StatusBits
{
	bool	hvacOK;
};

class CBIT_Class
{
//variables
public:
	StatusBits	statusBits;
	
protected:
private:
	volatile	bool	isOK;
	

//functions
public:
	CBIT_Class();
	~CBIT_Class();
	bool	IsOK(void);
	operator	bool();
protected:
private:
	CBIT_Class( const CBIT_Class &c );
	CBIT_Class& operator=( const CBIT_Class &c );

}; //CBIT_Class

extern	CBIT_Class	cBit;

#endif //__CBIT_CLASS_H__
