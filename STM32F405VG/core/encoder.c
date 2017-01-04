#include "encoder.h"

static s16 loopCount = 0;

//Init encoder
void encoder_init(void){
	//Init RCC Clock
	RCC_APB2PeriphClockCmd(ENCODER1_CLOCK_SOURCE , ENABLE);
	RCC_AHB1PeriphClockCmd(ENCODER1_GPIO_CLOCK_SOURCE, ENABLE);
	
	//AF Config
	GPIO_PinAFConfig(ENCODER1_GPIOx, ENCODER1_GPIO_PINSOURCE1, ENCODER1_AF);
	GPIO_PinAFConfig(ENCODER1_GPIOx, ENCODER1_GPIO_PINSOURCE2, ENCODER1_AF);
	
	//GPIO Init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ENCODER1_PORT1 | ENCODER1_PORT2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(ENCODER1_GPIOx, &GPIO_InitStructure);
	
	//Timer init
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(ENCODER1_TIMER, &TIM_TimeBaseStructure);

	//Interrupt init
	TIM_ITConfig(ENCODER1_TIMER, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_InitStructure.NVIC_IRQChannel = ENCDOER1_IRQ;
	NVIC_Init(&NVIC_InitStructure);

	//Encoder interface init
	TIM_EncoderInterfaceConfig(ENCODER1_TIMER, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	//Let's go!
	TIM_SetCounter(ENCODER1_TIMER, 0);
	loopCount = 0;
	TIM_Cmd(ENCODER1_TIMER, ENABLE);
}

void ENCODER1_IRQ_HANDLER(){
	if (TIM_GetITStatus(ENCODER1_TIMER, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(ENCODER1_TIMER, TIM_IT_Update);
		if (TIM_GetCounter(ENCODER1_TIMER) < 30000){
			//Overflow
			loopCount++;
		}else{
			//Underflow
			loopCount--;
		}
	}
}

s32 get_count(){
	return loopCount*0xFFFF + TIM_GetCounter(ENCODER1_TIMER);
}

void reset_encoder(){
	loopCount = 0;
	TIM_SetCounter(ENCODER1_TIMER, 0);
}

