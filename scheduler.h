/*
 * scheduler.h
 *
 * Created: 1/3/2025 3:18:49 PM
 *  Author: itsLydt
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "task.h"

/* the number of tasks to schedule */
#define NUM_TASKS 4

struct Task_t user_tasks[NUM_TASKS + 1];

/* initialize tasks using the specified function pointers. For now, assume that user provides enough tasks (foolish) */
void initialize_tasks(task_signature idle_task, task_signature* tasks);

void start();


#endif /* SCHEDULER_H_ */