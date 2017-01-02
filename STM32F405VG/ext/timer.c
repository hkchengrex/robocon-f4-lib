#include "timer.h"

static TimerActionStruct actions[TIMER_SIZE] = {0};
static u16 index = 0;
static u16 size = 0;
static u32 currCounting = 0;

static bool running = false;

static volatile bool fake_irq = false;

void timer_init(){
	RCC_APB1PeriphClockCmd(TIMER_RCC, ENABLE);
	
	//Setup timer
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
  TIM_TimeBaseStructure.TIM_Prescaler = TIMER_PRESCALER;
  TIM_TimeBaseStructure.TIM_Period = 1;
  TIM_TimeBaseInit(TIMER_TIM, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIMER_TIM, TIM_IT_Update, ENABLE);
	
	//Setup interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ARRPreloadConfig(TIMER_TIM, DISABLE);
	TIM_Cmd(TIMER_TIM, DISABLE);
	running = false;
	fake_irq =  false;
}

/**
* Register a event call that will happen after some time.
* @param action: The function to be called
* @param ms: The time to be waited
*/
void do_after(TimerAction action, u32 ms){
	if (size >= TIMER_SIZE){
		//Error
		while(1);
	}
	
	while(actions[index].action != 0){
		//Find an unused location
		index = (index+1)%TIMER_SIZE;
	}
	
	actions[index].action = action;
	actions[index].ms = ms;
	
	index = (index+1)%TIMER_SIZE;
	size++;
	
	if (running == false){
		TIM_SetCounter(TIMER_TIM, 0);
		TIM_SetAutoreload(TIMER_TIM, ms*10);
		TIM_Cmd(TIMER_TIM, ENABLE);
		currCounting = ms;
		
		fake_irq = true;
		TIM_GenerateEvent(TIMER_TIM, TIM_EventSource_Update);
		running = true;
		
	}else if (currCounting - TIM_GetCounter(TIMER_TIM) > ms){
		//u32 old_count = TIM_GetCounter(TIMER_TIM);
		//TIM_SetCounter(TIMER_TIM, old_count);
		TIM_SetAutoreload(TIMER_TIM, currCounting+ms);
		currCounting = currCounting+ms;
		
		fake_irq = true;
		TIM_GenerateEvent(TIMER_TIM, TIM_EventSource_Update);
	}
}

void TIMER_IRQ_HANDLER(void){
	if (TIM_GetITStatus(TIMER_TIM, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIMER_TIM, TIM_IT_Update);
		if (!fake_irq){
			for (u16 i=0; i<TIMER_SIZE; i++){
				if (actions[i].action != 0){
					//If such function exist
					if (actions[i].ms <= currCounting){
						//Execute function when time's up
						actions[i].action();
						
						//Remove the function
						size--;
						actions[i].action = 0;
					}else{
						//Otherwise keep counting
						actions[i].ms -= currCounting;
					}
				}
			}
			
			if (size == 0){
				TIM_Cmd(TIMER_TIM, DISABLE);
				running = false;
				index = 0;
			}
		}else{
			fake_irq = false;
		}
	}
}
