/*
 * RoundRobin.c
 *
 * Created: 1/3/2025 12:22:26 PM
 * Author : itsLydt
 */ 


#include "sam.h"
#include "ATSAMD21/drivers/gpio/gpio.h"

#define LED_PORT GPIOB
#define LED1 4
#define LED2 5
#define LED3 6

int main(void)
{
	uint32_t pin_mask = (1 << LED1) | (1 << LED2) | (1 << LED3);
	GPIO_SetPortDirection(LED_PORT, pin_mask, GPIO_OUT);			// set LED pins as outputs
	GPIO_WritePort(LED_PORT, pin_mask, 1);							//turn off all LEDs	
	
    while (1) 
    {
    }
}
