/* 
* FlapperValve_Class.h
*
* Created: 7/31/2020 3:45:11 PM
* Author: GMateusDP
*/


#ifndef __FLAPPERVALVE_CLASS_H__
#define __FLAPPERVALVE_CLASS_H__


class FlapperValve_Class
{
//variables
public:
protected:
private:
	

//functions
public:
	FlapperValve_Class();
	~FlapperValve_Class();
	bool	Init();
protected:
private:
	FlapperValve_Class( const FlapperValve_Class &c );
	FlapperValve_Class& operator=( const FlapperValve_Class &c );

}; //FlapperValve_Class
extern	FlapperValve_Class	fv1;
#endif //__FLAPPERVALVE_CLASS_H__
