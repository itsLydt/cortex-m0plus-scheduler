/*
 * scheduler.c
 *
 * Created: 1/3/2025 3:21:20 PM
 *  Author: itsLydt
 */ 

#include "scheduler.h"

uint32_t current_task_ticks = 0;
uint8_t current_task = 1;
uint8_t last_task = 1;

uint32_t MAX_TICKS = 0;
/* frequency of the systick clock - TODO this can probably be read out from clock module */
#define SYSTICK_CLK_HZ SYSTEM_CLOCK_FREQ		// default clock speed of MCU on boot
/* frequency of systick interrupt */
#define TICK_HZ 1U // interrupt once per second

void initialize_tasks(task_signature idle_task, task_signature* tasks);

uint32_t get_stack_address(uint8_t task_number);
void save_stack_address(uint8_t task_number, uint32_t address);

void init_systick_timer(uint32_t tick_hz);

void schedule();

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
	init_systick_timer(TICK_HZ);
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
	
	// store task number in R0 (first arg) per C calling convention
	__asm volatile("MOV R0, #1");
	__asm volatile("BL get_stack_address");		// branch to get_stack_address
	__asm volatile("MSR PSP, R0");				// return value of get_stack_address is stored in R0. Write this value to PSP
	__asm volatile("POP {R1}");					//	restore LR to R1
	
	// set PSP as current stack pointer for thread mode
	__set_CONTROL(2);							// bit[1] of CONTROL register 1: use PSP, 0: use MSP
	
	__asm volatile("BX R1 ");					// branch according to LR value stored in R1
}

void init_systick_timer(uint32_t tick_hz){
	// value passed in must be <= 0xFFFFFF
	if (SysTick_Config( SYSTICK_CLK_HZ / tick_hz) == 0){
		MAX_TICKS = TICK_HZ * 5; // 5 seconds before context switch
	}
}

void SysTick_Handler(){
	
	if(++current_task_ticks >= MAX_TICKS && current_task > 0){
		current_task_ticks = 0;
		/* set next task to ready */
		uint8_t next_task = (last_task + 1) % (NUM_TASKS + 1);
		if(next_task == 0){
			next_task = 1;
		}
		user_tasks[next_task].current_state = READY;
		user_tasks[last_task].current_state = BLOCKED;
		
		current_task = next_task;

		/* pend the context switch */
		schedule();
	}
	else {
		// check if any tasks can be unblocked 
	}
}

void schedule(){
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/* this handler performs the context switch */
void PendSV_Handler(){
	/* 1. save state of current task */
	__asm volatile("MRS R0, PSP"); 	// get the current task's stack pointer value
	__asm volatile("SUB R0, #0x10"); //decrement

	/* 1a. save register values not automatically stacked */
	__asm volatile("STM R0!, {R4-R7}"); // push r4 - r7 to task private stack NOTE: STM stores in ascending order
	// move upper registers to lower
	__asm volatile("MOV R4, R8");
	__asm volatile("MOV R5, R9");
	__asm volatile("MOV R6, R10");
	__asm volatile("MOV R7, R11");
	__asm volatile("SUB R0, #0x20"); // decrement for next push
	__asm volatile("STM R0!, {R4-R7}"); //push r4 - r7 to task private stack
	__asm volatile("SUB R0, #0x10"); //decrement stack pointer after STM

	// 1b. save value of PSP
	__asm volatile("PUSH {LR}"); //save value of link register
	__asm volatile("BL save_psp_value"); //passes value in R0 to save_psp_value
	
	/* 2. retrieve context of next task */
}