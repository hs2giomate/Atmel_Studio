/* 
* I2C_ObjectList.cpp
*
* Created: 10/1/2020 12:53:52 PM
* Author: GMateusDP
*/


#include "I2C_ObjectList.h"
#include "I2C_Asyn_Class.h"

// default constructor
I2C_ObjectList::I2C_ObjectList()
{
	objList=&i2cList;
} //I2C_ObjectList

// default destructor
I2C_ObjectList::~I2C_ObjectList()
{
} //~I2C_ObjectList

int I2C_ObjectList::AddI2CObject(I2CPointerAndDescriptor& ck ){
	
		ASSERT((objList)&&(&ck));
		if (is_list_element(objList,&ck)) {

			ASSERT(false);
			return ERR_ALREADY_INITIALIZED;
		}
		AddObject(objList,&ck);
		return ERR_NONE;
}

void I2C_ObjectList::MakeCoupleKey(i2c_m_sync_desc *ptrI2C,uint32_t* ptrClass,I2CPointerAndDescriptor& ck){
		//coupleKey.ptrI2CAsynClass=ptrClass;
		coupleKey.i2cDescr=ptrI2C;
		ck=coupleKey;
			
}

void I2C_ObjectList::AddObject(list_descriptor *list,I2CPointerAndDescriptor* nck){
		I2CPointerAndDescriptor *it, *head = (I2CPointerAndDescriptor *)list_get_head(list);

		if (!head) {
			list_insert_as_head(list, nck);
			return;
		}else{
			list_insert_at_end(list,nck);
		}
}

uint32_t I2C_ObjectList::GetClassPointer(i2c_m_sync_desc*	i2cDescr){
		struct list_element *it;
		//I2CPointerAndDescriptor *it;
		I2CPointerAndDescriptor *k;
		uint32_t ptr;
		for (it = objList->head; it; it = it->next) {
			k=(I2CPointerAndDescriptor*)it;
			
			if (k->i2cDescr== i2cDescr){
				ptr=k->ptrI2CAsynClass;
				return ptr;
			}
		}

		return 0;
}