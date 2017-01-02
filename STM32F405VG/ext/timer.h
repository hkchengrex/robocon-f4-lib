#ifndef	_TIMER_H
#define	_TIMER_H

#include "stm32f4xx_tim.h"
#include <stdbool.h>

/***************************************************************************************************************************************
** TIMER - STM32F4
**
** This library provides functions for scheduling tasks using timer.
** I guess it will be faster when implementated with priority queue but well I'm too lazy.
**
** ROBOCON 2017
** H K U S T
**
** Author:	Rex Cheng
** Contact:	hkchengad@connect.ust.hk
**
** v1.0 January 2016
**
** "I've seen the future. You're not in it." ~Faceless void
****************************************************************************************************************************************/

#define TIMER_SIZE 10 //Max number of actions that can be pended

#define TIMER_TIM 				TIM7
#define TIMER_RCC 				RCC_APB1Periph_TIM7
#define TIMER_IRQ 				TIM7_IRQn
#define TIMER_IRQ_HANDLER	TIM7_IRQHandler

//+1 count every 0.1 ms
#define TIMER_PRESCALER (SystemCoreClock/1000/10/2)

typedef void(*TimerAction)(void);

typedef struct{
	TimerAction action;
	u32 ms;
}TimerActionStruct;

//Init timer
void timer_init(void);

/**
* Register a event call that will happen after some time.
* @param action: The function to be called
* @param ms: The time to be waited (0~65535)
*/
void do_after(TimerAction action, u32 ms);

#endif
