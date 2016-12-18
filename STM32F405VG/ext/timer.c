#include "timer.h"

void timer_init(){
	RCC_APB1PeriphClockCmd(TIMER_RCC, ENABLE);
	
	//Setup timer
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;	
  TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 1000; 
  TIM_TimeBaseStructure.TIM_Period = 1;	                    
  TIM_TimeBaseInit(TIMER_TIM, &TIM_TimeBaseStructure);
	
	//Setup interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  TIM_ARRPreloadConfig(TIMER_TIM, DISABLE);
  TIM_Cmd(TIMER_TIM, ENABLE);
}
