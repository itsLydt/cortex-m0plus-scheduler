/*
 * scheduler.h
 *
 * Created: 1/3/2025 3:18:49 PM
 *  Author: itsLydt
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "task.h"
#include "stack.h"

/* the number of tasks to schedule */
#define NUM_TASKS 4

#if (NUM_TASKS > MAX_TASKS)
#error "ERROR: device cannot support the specified number of tasks"
#endif

#define SYSTEM_CLOCK_FREQ 1000000U

struct Task_t user_tasks[NUM_TASKS + 1];

__attribute__((naked)) void init_scheduler_stack(uint32_t stack_address);
__attribute__((naked)) void switch_to_psp();

/* initialize scheduler and configure tasks  using the specified function pointers. For now, assume that user provides enough tasks (foolish) */
void initialize_scheduler(task_signature idle_task, task_signature* tasks);

void start();



#endif /* SCHEDULER_H_ */