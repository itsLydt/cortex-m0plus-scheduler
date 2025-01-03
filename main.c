/*
 * RoundRobin.c
 *
 * Created: 1/3/2025 12:22:26 PM
 * Author : itsLydt
 */ 


#include "sam.h"
#include "ATSAMD21/drivers/gpio/gpio.h"

#include "task.h"

/* GPIO pin / port assignments */
#define LED_PORT GPIOB
#define LED1 4
#define LED2 5
#define LED3 6
const uint32_t led_mask = (1 << LED1) | (1 << LED2) | (1 << LED3);

/* the number of tasks to schedule */
#define NUM_TASKS 4
struct Task_t user_tasks[NUM_TASKS + 1];

/* frequency of the systick clock - TODO this can probably be read out from clock module */
#define SYSTICK_CLK_HZ 1000000U
/* frequency of systick interrupt */
#define TICK_HZ 1000U

/* number of cpu cycles in one millisecond */
const uint32_t CYCLES_PER_MS = SYSTICK_CLK_HZ / 1000;

/* declarations of sample tasks */
void task1();
void task2();
void task3();
void task4();

int main(void)
{
	/* initialize GPIO */
	GPIO_SetPortDirection(LED_PORT, led_mask, GPIO_OUT);			// set LED pins as outputs
	GPIO_WritePort(LED_PORT, led_mask, 1);							//turn off all LEDs	
	
	
	/* Initialize stack space for the scheduler */
	
	/* Initialize stack, control block info for each task */
	
	/* Switch the scheduler over to PSP (program stack pointer) */
	
	/* Enable the systick timer, which effectively starts the scheduler */
	
	/* Kick off the first task */
	task1();
	
	/* This will not execute, as the scheduler will execute the idle_task_handler function if there are no ready tasks */
    while (1);
}

/* wait at least delay_ms milliseconds, approximately */
void wait(uint32_t delay_ms){
	delay_ms *= CYCLES_PER_MS / 6; //appx number of instructions in the loop body below
	for(; delay_ms > 0; --delay_ms){
		asm("nop");
	}
}

/* blink the specified LEDs with a specified delay between blinks */
void blink(uint32_t leds, uint32_t delay){
	while (1){
		GPIO_WritePort(LED_PORT, leds, 0);	// LEDs on
		wait(delay);
		GPIO_WritePort(LED_PORT, leds, 1);	// LEDs off
		wait(delay);
	}
}

/* this task will run when there are no tasks ready to run */
void idle_task_handler(){
	GPIO_WritePort(LED_PORT, led_mask, 1);	//turn off all LEDs
	while(1);
}

/* example task implementations */
void task1(){
	blink((1 << LED1), 500);
}

void task2(){
	blink((1 << LED2), 500);
}

void task3(){
	blink((1 << LED3), 500);
}

void task4(){
	blink((1 << LED1) | (1 << LED3), 500);
}