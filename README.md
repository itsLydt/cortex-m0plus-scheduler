# Cortex M0+ Scheduler
A simple round robin scheduler implemented on an ATSAMD21 MCU

The scheduler uses the systick timer and service calls, features of the Cortex M0+ processor, to switch between tasks.
This example illustrates how to manually stack and unstack frames on the Cortex M0+ in order to safely context switch, 
as well as how to utilize the two stack pointers available in the CPU. 
