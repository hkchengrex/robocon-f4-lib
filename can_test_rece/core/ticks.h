#ifndef __TICKS_H
#define __TICKS_H

#include "stm32f4xx_tim.h"
#include "buzzer.h"

#define TICKS_TIM							TIM5
#define TICKS_RCC							RCC_APB1Periph_TIM5
#define TICKS_CLKFreq					84000000
#define TICKS_IRQn						TIM5_IRQn
#define TICKS_IRQHandler			void TIM5_IRQHandler(void)

void ticks_init(void);

/** @brief  Get the ticks(ms) passed from 0-999
  * @retval ticks passed
  */
u16 get_ticks(void);

/** @brief  Get the seconds passed
  * @retval seconds passed
  */
u16 get_seconds(void);

/** @brief  Get the total ticks(seconds*1000 + current ticks) passed
  * @retval ticks passed
  */
u32 get_full_ticks(void);

/** @brief  Generate a delay (in us)
  * @param  nus: us to be delayed
  */
void _delay_us(u32 nus);

/** @brief  Generate a delay (in us)
  * @param  nus: us to be delayed
  */
void _delay_ms(u16 nms);

/** @brief  A single 1ms delay
  * @param  None
  * @retval None
  */
void simple_delay1_ms(void);

/** @brief  A single 10us delay written in assembly
  * @param  None
  * @retval None
  */
void simple_delay10_us(void);

#endif
