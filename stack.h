/*
 * stack.h
 *
 * Created: 1/13/2025 1:03:02 PM
 *  Author: itsLydt
 */ 


#ifndef STACK_H_
#define STACK_H_

/* the Cortex-M0+ uses a full descending stack, meaning that when a new item is to be placed on the stack, 
	the stack pointer is decremented and the item is written to the new memory location. 
	Ergo, the stack pointer points to the last item added to the stack.
*/ 

#define SRAM_START HMCRAMC0_ADDR	/* starting address of SRAM in memory map */
#define SRAM_END	(SRAM_START + HMCRAMC0_SIZE) /* size of SRAM (16KB or 32KB likely) */

/* allocate each task and the scheduler itself a stack with size 1KB */
#define TASK_STACK_LIMIT 1024U				// 1KB
#define SCHED_STACK_LIMIT TASK_STACK_LIMIT	// give scheduler same stack capacity as task, mainly out of convenience

#define MAX_TASKS ((HMCRAMC0_SIZE / TASK_STACK_LIMIT) - 1)	// number of tasks capable of fitting in SRAM along with the scheduler
#endif /* STACK_H_ */