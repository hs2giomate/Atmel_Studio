/* 
* I2C_ObjectList.h
*
* Created: 10/1/2020 12:53:52 PM
* Author: GMateusDP
*/


#ifndef __I2C_OBJECTLIST_H__
#define __I2C_OBJECTLIST_H__

#include "utils_list.h"
#include "hal_i2c_m_async.h"
struct I2CPointerAndDescriptor
{
	uint32_t	dummy;
	i2c_m_async_desc*	i2cDescr;
	uint32_t		ptrI2CAsynClass;
};
typedef struct I2CPointerAndDescriptor I2CPointerAndDescriptor;


class I2C_ObjectList
{
//variables
public:
	I2CPointerAndDescriptor coupleKey;
protected:
private:
	list_descriptor		*objList;
//functions
public:
	I2C_ObjectList();
	~I2C_ObjectList();
	void MakeCoupleKey(i2c_m_async_desc *ptrI2,uint32_t* ptrClass,I2CPointerAndDescriptor& ck);
	int AddI2CObject(I2CPointerAndDescriptor& ck );
	uint32_t GetClassPointer(i2c_m_async_desc*	i2cDescr);
protected:
private:
	I2C_ObjectList( const I2C_ObjectList &c );
	I2C_ObjectList& operator=( const I2C_ObjectList &c );
	void AddObject(list_descriptor *list,I2CPointerAndDescriptor* ck );
}; //I2C_ObjectList

#endif //__I2C_OBJECTLIST_H__
