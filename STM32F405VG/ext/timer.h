#ifndef	_TIMER_H
#define	_TIMER_H

#include "stm32f4xx_tim.h"

#define TIMER_TIM TIM6
#define TIMER_RCC RCC_APB1Periph_TIM6
#define TIMER_IRQ 

//First /4 because APB1, then /4 by ClockDivision, /1000 because in ms, so 1ms = 1 count
#define TIMER_PRESCALER (SystemCoreClock/4/4/1000) 

#endif
