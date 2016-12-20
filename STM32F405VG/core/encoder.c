#include "encoder.h"

static TIM_TypeDef* encoder_timer[ENCODER_NO] = {ENCODER_TIMER1};

/**
  * @brief  Initialization of encoder
  * @param  None
  * @retval None
  */
void encoder_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//Alternate function enable: Encoder 1
	RCC_APB2PeriphClockCmd(ENCODER_TIMER1_CLOCK_SOURCE , ENABLE);
	RCC_AHB1PeriphClockCmd(ENCODER_TIMER1_GPIO_CLOCK_SOURCE, ENABLE);
	// Enable Timer Clock Source
	GPIO_PinAFConfig(ENCODER_TIMER1_GPIOx, ENCODER_TIMER1_GPIO_PINSOURCE1, ENCODER_TIMER1_AF);
	GPIO_PinAFConfig(ENCODER_TIMER1_GPIOx, ENCODER_TIMER1_GPIO_PINSOURCE2, ENCODER_TIMER1_AF);
	
	// GPIO init Encoder 1
	GPIO_StructInit(&GPIO_InitStructure);																				
	GPIO_InitStructure.GPIO_Pin = ENCODER_TIMER1_PORT1 | ENCODER_TIMER1_PORT2;						
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(ENCODER_TIMER1_GPIOx, &GPIO_InitStructure);	
	
	// Timer init Encoder 1
	TIM_ClearITPendingBit(ENCODER_TIMER1, TIM_IT_Update);			
	TIM_DeInit(ENCODER_TIMER1);													
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00; 		
	TIM_TimeBaseStructure.TIM_Period = 0xffff;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(ENCODER_TIMER1, &TIM_TimeBaseStructure);

	// Setting to Rising edge mode
	// Use TIM_EncoderMode_TI1 to count only B pin reading
	// TI2 to count only A pin
	// TI12 = TI3 to count both

	//Enable counting for Encoder 1
	TIM_EncoderInterfaceConfig(ENCODER_TIMER1, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetCounter(ENCODER_TIMER1, 0);
	TIM_Cmd(ENCODER_TIMER1, ENABLE);	
}

//Return and update the encoder reading, need to call frequently, at least within a encoder cycle
s16 get_count(ENCODER ENCODERx){ 
	return TIM_GetCounter(encoder_timer[ENCODERx]);
}

void reset_encoder(ENCODER ENCODERx){
	TIM_SetCounter(encoder_timer[ENCODERx], 0);
}

