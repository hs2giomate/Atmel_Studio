/* 
* TaskHandler_Class.h
*
* Created: 10/5/2020 6:48:01 PM
* Author: GMateusDP
*/


#ifndef __TASKHANDLER_CLASS_H__
#define __TASKHANDLER_CLASS_H__
#include "utils_list.h"

//struct ControllerTask;
//typedef void (*task_cb_t)(ControllerTask *);
enum ALUTaskEnum
{
	kALUNotTaskEvent=0,
	kALUSwitchTaskEvent,
	kALUTaskArincTXMessage,
	kALUTaskCheckCommunication,
	kALUTaskReadARINCR1,
	kALUTaskReadARINCR2,
	kALUTaskRunHAVAC,
	kALUSimpleStart,
	kALUSimpleStop,
	kALUSimpleResume,
	kALUTaskUpdateTemperatures,
	kALUTaskUpdateFlapperValves,
	kALUNumberTasks
};
#define MAXIMUM_TASKS	kALUNumberTasks
struct ControllerTask
{
	 list_element elem;  
	 ALUTaskEnum id;
	uint8_t		prio;
	
};

//static	ControllerTask	taskStorage[MAXIMUM_TASKS];

class TaskHandler_Class
{
//variables
public:
	
	list_descriptor* taskList;
protected:
private:
	ControllerTask	*task;
	
	uint8_t i,j,k;
	list_descriptor* taskListPrivate;
//functions
public:
	TaskHandler_Class();
	~TaskHandler_Class();
	void			InitTaskArray(void);
	int				AddTask(ControllerTask& tk );
	int				AddTask(ALUTaskEnum& at );
	int32_t			RemoveTask(ALUTaskEnum& at);
	int32_t			RemoveTask(ControllerTask& tk);
	ControllerTask	GetHighPrioTask(void);
	bool			IsListedTask(ALUTaskEnum at);
protected:
private:
	TaskHandler_Class( const TaskHandler_Class &c );
	TaskHandler_Class& operator=( const TaskHandler_Class &c );
	void AddALUTask(list_descriptor *list,ControllerTask* pct);
	void	ChooseAvailableTask(void);
	bool	IsNotDeclaredTask(ControllerTask& tk);


}; //TaskHandler_Class
//extern list_descriptor		taskStorage;
#endif //__TASKHANDLER_CLASS_H__
