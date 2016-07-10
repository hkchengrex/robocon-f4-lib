#ifndef _SERVO_H_
#define _SERVO_H_

/**
	This file is to control analog servo (or some digital servos).
	It generates PWM signal with 20ms period (Frequency 50Hz)
	Normal servo would have their middle point at 1.5ms high time (CCR = 1500)
	And both ends around +- 0.6ms (Try yourself)
*/

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"	  
#include "stm32f4xx_tim.h"

#define SERVO_COUNT     1
#define SERVO_TIM				TIM1
#define SERVO_AF_TIM 		GPIO_AF_TIM1
#define SERVO_PORT			GPIOA
#define SERVO_TIM_RCC		RCC_APB2Periph_TIM1
#define SERVO_GPIO_RCC	RCC_AHB1Periph_GPIOA

typedef struct {
	u16 servo_tim_ch;
	GPIO_TypeDef* GPIOx;
	u16 servo_pin;
	uint16_t GPIO_PinSource;
	FunctionalState state;
	void (*oc_init_function)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
  void (*TIM_SetCompare) (TIM_TypeDef* TIMx, uint32_t Compare1);
} SERVO_PWM_STRUCT[];

typedef enum {
  SERVO1
} SERVO_ID;

void servo_init(void);

/**
	Control the servo with a pwm.
	@param servo_id The servo to be controlled
	@param CCR value out of 20000. Period 20ms.
*/
void servo_control(SERVO_ID servo_id , u16 ccr_val);

#endif
