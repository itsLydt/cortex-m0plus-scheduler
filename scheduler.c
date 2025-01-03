/*
 * scheduler.c
 *
 * Created: 1/3/2025 3:21:20 PM
 *  Author: itsLydt
 */ 

#include "scheduler.h"

void initialize_tasks(task_signature idle_task, task_signature* tasks){
	/* task 0 is always the idle task */
	user_tasks[0].task_handler = idle_task;
	user_tasks[0].current_state = READY;
	
	for(int i = 1; i < NUM_TASKS + 1; i++){
		user_tasks[i].task_handler = tasks[i - 1];
		
		user_tasks[i].current_state = BLOCKED;
	}
}

void start(){
	/* Enable the systick timer, which effectively starts the scheduler */

	/* Start the first task */
	user_tasks[1].current_state = READY;
	user_tasks[1].task_handler();
}