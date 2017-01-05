#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"

/**
* This library handles encoder count with interrupt to count beyond 65535
* Note that this library is poorly written (by me) so you also need to change the interrupt handler if you want to change to another timer
*
* Rex Cheng
*/

#define ENCODER1_TIMER										TIM4
#define ENCODER1_AF                       GPIO_AF_TIM4
#define ENCODER1_TIMER_RCC_INIT						RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
#define ENCDOER1_IRQ											TIM4_IRQn
#define ENCODER1_IRQ_HANDLER							TIM4_IRQHandler

#define ENCODER1_GPIO_RCC_INIT			    	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
#define ENCODER1_PORT1										GPIO_Pin_12
#define ENCODER1_PORT2										GPIO_Pin_13
#define ENCODER1_GPIOx										GPIOD
#define ENCODER1_GPIO_PINSOURCE1          GPIO_PinSource12
#define ENCODER1_GPIO_PINSOURCE2          GPIO_PinSource13

#define ENCODER1_MODE											GPIO_OType_OD //Open-drain / Push-Pull

void encoder_init(void);

/** Get the encoder reading. Overflow after 65535.
	@return encoder count
*/
s32 get_encoder_count(void);

// Reset the encoder count
void reset_encoder(void);

#endif
