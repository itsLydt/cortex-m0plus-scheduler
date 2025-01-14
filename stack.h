/*
 * stack.h
 *
 * Created: 1/13/2025 1:03:02 PM
 *  Author: itsLydt
 */ 


#ifndef STACK_H_
#define STACK_H_

#include "sam.h"

/* the Cortex-M0+ uses a full descending stack, meaning that when a new item is to be placed on the stack, 
	the stack pointer is decremented and the item is written to the new memory location. 
	Ergo, the stack pointer points to the last item added to the stack and the "top" of the stack moves 
	closer to the start address when items are added. When items are removed, the stack pointer is incremented
	and thus the "bottom" of the stack is closer to the end address.
*/ 

#define SRAM_START HMCRAMC0_ADDR	/* starting address of SRAM in memory map */
#define SRAM_END	(SRAM_START + HMCRAMC0_SIZE) /* size of SRAM (16KB or 32KB likely) */

/* allocate each task and the scheduler itself a stack with size 1KB */
#define TASK_STACK_LIMIT 1024U				// 1KB
#define SCHED_STACK_LIMIT TASK_STACK_LIMIT	// give scheduler same stack capacity as task, mainly out of convenience

#define STACK_START (SRAM_END - (NUM_TASKS*TASK_STACK_LIMIT + SCHED_STACK_LIMIT))	// starting address of entire stack (all tasks + scheduler) in SRAM

#define SCHED_STACK_END		SRAM_END												// maximum address of scheduler stack space, same as end of SRAM
#define SCHED_STACK_START	(SCHED_STACK_END - SCHED_STACK_LIMIT)					// minimum address of scheduler stack space

#define IDLE_STACK_END		SCHED_STACK_START										// maximum address of idle task stack space, same as min address of scheduler stack
#define IDLE_STACK_START	(IDLE_STACK_END - TASK_STACK_LIMIT)						// minimum address of idle task stack space

#define USER_STACK_END		IDLE_STACK_START										// maximum address of user task stack space 

#define MAX_TASKS ((HMCRAMC0_SIZE / TASK_STACK_LIMIT) - 2)	// number of tasks capable of fitting in SRAM along with the scheduler and the idle task
#endif /* STACK_H_ */