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

#define GET_TIMER_AND_AF(a) TIM##a, GPIO_AF_TIM##a

#define SERVO_TABLE \
X(SERVO1, GET_TIMER_AND_AF(3), PC6, 1) \
X(SERVO2, GET_TIMER_AND_AF(3), PC7, 2) \
X(SERVO3, GET_TIMER_AND_AF(3), PC8, 3) \
X(SERVO4, GET_TIMER_AND_AF(3), PC9, 4) \
X(SERVO5, GET_TIMER_AND_AF(11), PF7, 1) \
X(SERVO6, GET_TIMER_AND_AF(13), PF8, 1) \

typedef struct {
	u16 tim;
	u8 tim_af;
} SERVO_STRUCT;

typedef enum {
  SERVO1
} SERVO_ID;

void servo_init(void);

/** Control the servo with a pwm.
	@param servo_id The servo to be controlled
	@param CCR value out of 20000. Period 20ms.
*/
void servo_control(SERVO_ID servo_id , u16 ccr_val);

#endif
