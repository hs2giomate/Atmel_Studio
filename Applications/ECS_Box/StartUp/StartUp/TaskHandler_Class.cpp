/* 
* TaskHandler_Class.cpp
*
* Created: 10/5/2020 6:48:00 PM
* Author: GMateusDP
*/


#include "TaskHandler_Class.h"
#include "utils_assert.h"
//static TaskHandler_Class	taskStorage[MAXIMUM_TASKS];
//list_descriptor		taskStorage;
static	ControllerTask	taskStorage[MAXIMUM_TASKS];
static	list_element	listElements[MAXIMUM_TASKS];
// default constructor
TaskHandler_Class::TaskHandler_Class()
{
	taskListPrivate=(list_descriptor*)listElements;
	//taskList->head->next=NULL;
	
	//task->id=(ALUTaskEnum)0;


} //TaskHandler_Class

// default destructor
TaskHandler_Class::~TaskHandler_Class()
{
} //~TaskHandler_Class

void TaskHandler_Class::InitTaskArray(void){
	for (i=1; i<MAXIMUM_TASKS; i++)
	{
		taskStorage[i].elem.next = (list_element*)NULL;
		taskStorage[i].id=kALUNotTaskEvent;
		taskStorage[i-1].elem.next = &taskStorage[i].elem;
		listElements[i].next = (list_element*)NULL;
		listElements[i-1].next = &listElements[i];
	}
	taskListPrivate->head->next=NULL;
	taskListPrivate->head=NULL;

		//taskList->head->next=NULL;
		taskList=taskListPrivate;
}

int TaskHandler_Class::AddTask(ALUTaskEnum& at ){
	ChooseAvailableTask();
		task->id=at;
	ASSERT((taskListPrivate)&&(task));
	if (is_list_element(taskListPrivate,task)) {

		//ASSERT(false);
		return ERR_ALREADY_INITIALIZED;
	}
	AddALUTask(taskListPrivate,task);
	return ERR_NONE;
}
int TaskHandler_Class::AddTask(ControllerTask& tk ){
	if (IsNotDeclaredTask(tk))
	{
		ChooseAvailableTask();
		task->elem=tk.elem;
		task->id=tk.id;
		task->prio=tk.prio;
		ASSERT((taskListPrivate)&&(task));
		if (is_list_element(taskListPrivate,task)) {

			//ASSERT(false);
			return ERR_ALREADY_INITIALIZED;
		}
		AddALUTask(taskListPrivate,task);
		return ERR_NONE;
	}else{
		return ERR_ALREADY_INITIALIZED;
	}
	
}

void TaskHandler_Class::AddALUTask(list_descriptor *list,ControllerTask* pct){
	ControllerTask *it, *head = (ControllerTask *)list_get_head(list);

	if (!head) {
		list_insert_as_head(list, pct);
		return;
		}else{
		list_insert_at_end(list,pct);
	}

		

}

int32_t TaskHandler_Class::RemoveTask(ALUTaskEnum& at){
			task->id=at;
		
		ASSERT(taskListPrivate && (task));
	

		if (!is_list_element(taskListPrivate, task)) {
			//ASSERT(false);
			return ERR_NOT_FOUND;
		}
		list_delete_element(taskListPrivate, task);

		return ERR_NONE;
}
int32_t TaskHandler_Class::RemoveTask(ControllerTask& tk){


	ASSERT(taskListPrivate && (&tk));
	if (IsNotDeclaredTask(tk))
	{
			return ERR_NOT_FOUND;
	} 
	else
	{
		
		if (!is_list_element(taskListPrivate, task)) {
			//ASSERT(false);
			return ERR_NOT_FOUND;
		}
			
		list_delete_element(taskListPrivate, task);
		task->id=kALUNotTaskEvent;
		return ERR_NONE;
	}

	return ERR_NOT_FOUND;
}

ControllerTask	TaskHandler_Class::GetHighPrioTask(void){
	list_element *it;
	ControllerTask* ct;
	uint8_t		highPrio=kALUNumberTasks;
		
	for (it = taskListPrivate->head; it; it = it->next) {
		ct=(ControllerTask*)it;
		if ((ct->prio)<(highPrio)) {
			highPrio=ct->prio;
			task=ct;
			
		}
	}
	return (ControllerTask)(*task);
}

void	TaskHandler_Class::ChooseAvailableTask(void){
	for (uint8_t ii = 0; ii < MAXIMUM_TASKS; ii++)
	{
		j=ii%MAXIMUM_TASKS;
		if (taskStorage[j].id==kALUNotTaskEvent)
		{			
			task=&taskStorage[j];
			return;
			
		}
	}
	task=&taskStorage[0];

}

bool	TaskHandler_Class::IsNotDeclaredTask(ControllerTask& tk){
	for (uint8_t ii = 0; ii < MAXIMUM_TASKS; ii++)
	{
		if (taskStorage[ii].id==tk.id)
		{
			task=&taskStorage[ii];
			return false;
			
		}
	}
	return true;


}

bool	TaskHandler_Class::IsListedTask(ALUTaskEnum at){
	for (uint8_t ii = 0; ii < MAXIMUM_TASKS; ii++)
	{
		if (taskStorage[ii].id==at)
		{
		//	task=&taskStorage[ii];
			return true;
			
		}
	}
	return false;


}