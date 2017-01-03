#include "timer.h"

static TimerActionStruct actions[TIMER_SIZE] = {0};
static u16 index = 0;
static u16 size = 0;

static bool running = false;

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
	
	//Disable preload as to disable shadow register
	TIM_ARRPreloadConfig(TIMER_TIM, DISABLE);
	TIM_Cmd(TIMER_TIM, DISABLE);
	running = false;
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
	actions[index].quantum = ms*QUANTUM_MULTIPLER;
	
	index = (index+1)%TIMER_SIZE;
	size++;
	
	if (running == false){
		TIM_SetCounter(TIMER_TIM, 0);
		if (ms*QUANTUM_MULTIPLER >= 65535){
			TIM_SetAutoreload(TIMER_TIM, 65535);
		}else{
			TIM_SetAutoreload(TIMER_TIM, ms*QUANTUM_MULTIPLER);
		}
		TIM_Cmd(TIMER_TIM, ENABLE);

		running = true;
		
	}else if (TIMER_TIM->ARR - TIM_GetCounter(TIMER_TIM) > ms*QUANTUM_MULTIPLER){
		u32 old_count = TIM_GetCounter(TIMER_TIM);
		TIM_SetAutoreload(TIMER_TIM, old_count+ms*QUANTUM_MULTIPLER);
	}
}

void TIMER_IRQ_HANDLER(void){
	if (TIM_GetITStatus(TIMER_TIM, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIMER_TIM, TIM_IT_Update);
		u32 min_quantum = (u32)-1;
		for (u16 i=0; i<TIMER_SIZE; i++){
			if (actions[i].action != 0){
				//If such function exist
				if (actions[i].quantum <= TIMER_TIM->ARR){
					//Execute function when time's up
					actions[i].action();
					
					//Remove the function
					size--;
					actions[i].action = 0;
				}else{
					//Otherwise keep counting
					actions[i].quantum -= TIMER_TIM->ARR;
					if (actions[i].quantum < min_quantum){
						min_quantum = actions[i].quantum;
					}
				}
			}
		}
		
		if (min_quantum == (u32)-1){
			//If no function remains
			TIM_Cmd(TIMER_TIM, DISABLE);
			running = false;
			index = 0;
		}else{
			//else setup a new timer
			if (min_quantum >= 65535){
				//If the target period > 16bit
				min_quantum = 65535;
			}
			TIM_SetCounter(TIMER_TIM, 0);
			TIM_SetAutoreload(TIMER_TIM, min_quantum);
		}
		
//		if (size == 0){
//			TIM_Cmd(TIMER_TIM, DISABLE);
//			running = false;
//			index = 0;
//		}
	}
}
