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
#include "gpio.h"

#define SERVO_TABLE \
X(SERVO1, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC6, 1) \
X(SERVO2, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC7, 2) \
X(SERVO3, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC8, 3) \
X(SERVO4, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC9, 4) \
X(SERVO5, TIM11, GPIO_AF_TIM11, RCC_APB2Periph_TIM11, PF7, 1) \
X(SERVO6, TIM13, GPIO_AF_TIM13, RCC_APB1Periph_TIM13, PF8, 1) \

typedef struct {
	TIM_TypeDef* tim;
	u8 tim_af;
	u32 tim_rcc;
	const GPIO* gpio;
	u8 channel;
} ServoStruct;

#define X(a, b, c, d, e, f) a, 
typedef enum {
  SERVO_TABLE
} ServoID;
#undef X

#define X(a, b, c, d, e, f) {b, c, d, &e, f},
static const ServoStruct SERVO[] = {SERVO_TABLE};
#undef X

#define SERVO_SIZE (sizeof(SERVO)/sizeof(ServoStruct))

void servo_init(void);

/** Control the servo with a pwm.
	@param servo_id The servo to be controlled
	@param CCR value out of 20000. Period 20ms.
*/
void servo_control(ServoID servo_id , u16 ccr_val);

#endif
