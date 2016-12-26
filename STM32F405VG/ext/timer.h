#ifndef	_TIMER_H
#define	_TIMER_H

#include "stm32f4xx_tim.h"
#include <stdbool.h>

#define TIMER_SIZE 30 //Max number of actions that can be pended

#define TIMER_TIM 				TIM7
#define TIMER_RCC 				RCC_APB1Periph_TIM7
#define TIMER_IRQ 				TIM7_IRQn
#define TIMER_IRQ_HANDLER	TIM7_IRQHandler

//First /4 because APB1, then /4 by ClockDivision, /1000 because in 10 ms, so 1ms = 1 count
#define TIMER_PRESCALER (SystemCoreClock/4/4/100) 

typedef void(*TimerAction)(void);

typedef struct{
	TimerAction action;
	u32 ms;
}TimerActionStruct;

/**
* Register a event call that will happen after some time.
* @param action: The function to be called
* @param ms: The time to be waited (0~655350) Scaled up by 10
*/
void do_after(TimerAction action, u32 ms);

#endif
