/*
 * scheduler.c
 *
 * Created: 1/3/2025 3:21:20 PM
 *  Author: itsLydt
 */ 

#include "scheduler.h"


void initialize_tasks(task_signature idle_task, task_signature* tasks);

uint32_t get_stack_address(uint8_t task_number);
void save_stack_address(uint8_t task_number, uint32_t address);

void initialize_scheduler(task_signature idle_task, task_signature* tasks){
	
	/* initialize stack space for user tasks and idle tasks */
	initialize_tasks(idle_task, tasks);
}


void initialize_tasks(task_signature idle_task, task_signature* tasks){	
	/* task 0 is always the idle task */
	user_tasks[0].task_handler = idle_task;
	user_tasks[0].current_state = READY;
	
	for(int i = 0; i < NUM_TASKS + 1; i++){
		if(i > 0){
			// set task handlers and state for user tasks
			user_tasks[i].task_handler = tasks[i - 1];
			user_tasks[i].current_state = BLOCKED;
		}
		
		save_stack_address(i, IDLE_STACK_END - (i*TASK_STACK_LIMIT));
				
		// pointer to program stack pointer for the given task
		uint32_t* psp = (uint32_t*)get_stack_address(i);
		
		// decrement stack pointer and store XPSR (thumb mode = always set for this CPU)
		--psp;	
		*psp = xPSR_T_Msk;	// this ensures thumb mode bit is set
		
		// decrement stack pointer and store initial PC (program counter), which is the address of task handler
		--psp;
		*psp = (uint32_t)user_tasks[i].task_handler;
		
		// decrement stack pointer and store initial LR (link register)
		--psp;
		*psp = EXC_RETURN_THREAD_PSP;	// this value means return to thread mode and use the PSP (process stack pointer) as SP
		
		//store value of registers (R0-R12)
		for(int j = 0; j < 13; j++){
			--psp;
			*psp = 0;
		}
		
		// store new value of stack pointer
		save_stack_address(i, (uint32_t)psp);
	}	
}

void start(){
	/* Enable the systick timer, which effectively starts the scheduler */

	/* Start the first task */
	user_tasks[1].current_state = READY;
	user_tasks[1].task_handler();
}

__attribute__((naked)) void init_scheduler_stack(uint32_t stack_address){
	__set_MSP(stack_address - 8*4);	// set main stack pointer value 
	__ISB();
	__asm volatile("BX LR");	// return
}

uint32_t get_stack_address(uint8_t task_number){
	return user_tasks[task_number].stack_addr;
}

void save_stack_address(uint8_t task_number, uint32_t address){
	user_tasks[task_number].stack_addr = address;
}

__attribute__((naked)) void switch_to_psp(){
	__asm volatile("PUSH {LR}");				// save value of link register so we can return to calling function

	/* configure value of PSP to point to stack of first task */
	
	// store task number in R0 per C calling convention
	__asm volatile("MOV R0, #1");
	__asm volatile("BL get_stack_address");		// branch to get_stack_address
	__asm volatile("MSR PSP, R0");				// return value of get_stack_address is stored in R0. Write this value to PSP
	__asm volatile("POP {R1}");					//	restore LR to R1
	
	// set PSP as current stack pointer for thread mode
	__set_CONTROL(2);							// bit[1] of CONTROL register 1: use PSP, 0: use MSP
	
	__asm volatile("BX R1 ");					// branch according to LR value stored in R1
}