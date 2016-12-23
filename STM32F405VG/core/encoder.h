#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"

#define ENCODER_TIMER1													TIM4
#define ENCODER_TIMER1_AF                       GPIO_AF_TIM4
#define ENCODER_TIMER1_CLOCK_SOURCE							RCC_APB1Periph_TIM4	
#define ENCODER_TIMER1_GPIO_CLOCK_SOURCE	    	RCC_AHB1Periph_GPIOD			
#define ENCODER_TIMER1_PORT1										GPIO_Pin_12
#define ENCODER_TIMER1_PORT2										GPIO_Pin_13
#define ENCODER_TIMER1_GPIOx										GPIOD
#define ENCODER_TIMER1_GPIO_PINSOURCE1          GPIO_PinSource12
#define ENCODER_TIMER1_GPIO_PINSOURCE2          GPIO_PinSource13

void encoder_init(void);

/** Get the encoder reading. Overflow after 65535.
	@return encoder count
*/
s16 get_count(void);

// Reset the encoder count
void reset_encoder(void);

#endif
