#include "encoder.h"

//Init encoder
void encoder_init(void){
	//Init RCC Clock
	RCC_APB2PeriphClockCmd(ENCODER_TIMER1_CLOCK_SOURCE , ENABLE);
	RCC_AHB1PeriphClockCmd(ENCODER_TIMER1_GPIO_CLOCK_SOURCE, ENABLE);
	
	//AF Config
	GPIO_PinAFConfig(ENCODER_TIMER1_GPIOx, ENCODER_TIMER1_GPIO_PINSOURCE1, ENCODER_TIMER1_AF);
	GPIO_PinAFConfig(ENCODER_TIMER1_GPIOx, ENCODER_TIMER1_GPIO_PINSOURCE2, ENCODER_TIMER1_AF);
	
	//GPIO Init
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ENCODER_TIMER1_PORT1 | ENCODER_TIMER1_PORT2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(ENCODER_TIMER1_GPIOx, &GPIO_InitStructure);
	
	//Timer init
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(ENCODER_TIMER1);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(ENCODER_TIMER1, &TIM_TimeBaseStructure);

	//Encoder interface init
	TIM_EncoderInterfaceConfig(ENCODER_TIMER1, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	//Let's go!
	TIM_SetCounter(ENCODER_TIMER1, 0);
	TIM_Cmd(ENCODER_TIMER1, ENABLE);
}

s16 get_count(){
	return TIM_GetCounter(ENCODER_TIMER1);
}

void reset_encoder(){
	TIM_SetCounter(ENCODER_TIMER1, 0);
}

