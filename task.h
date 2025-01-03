/*
 * task.h
 *
 * Created: 1/3/2025 12:35:21 PM
 *  Author: itsLydt
 */ 


#ifndef TASK_H_
#define TASK_H_

enum TaskState {
	READY,
	BLOCKED
};


struct Task_t {
	uint32_t stack_addr;		// stack address for this task
	uint32_t resumption_tick;	// tick at which this task can be resumed 	
	uint8_t current_state;		// the current state of the task, which is a TaskState 
	void (*task_handler)(void);	// function pointer for the code to be run by this task
};


#endif /* TASK_H_ */