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

#define SERVO_INIT_TABLE \
X(SERVO1, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC6, 1) \
X(SERVO2, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC7, 2) \
X(SERVO3, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC8, 3) \
X(SERVO4, TIM3, GPIO_AF_TIM3, RCC_APB1Periph_TIM3, PC9, 4) \
X(SERVO5, TIM11, GPIO_AF_TIM11, RCC_APB2Periph_TIM11, PF7, 1) \
X(SERVO6, TIM13, GPIO_AF_TIM13, RCC_APB1Periph_TIM13, PF8, 1) \

//ServoID, min ccr, max ccr, min deg(x10), max deg(x10)
#define SERVO_CONFIG_TABLE \
C(SERVO1, 900, 2100, 0, 1800) \
C(SERVO2, 900, 2100, 0, 1800) \
C(SERVO3, 900, 2100, 0, 1800) \
C(SERVO4, 900, 2100, 0, 1800) \
C(SERVO5, 900, 2100, 0, 1800) \
C(SERVO6, 900, 2100, 0, 1800) \

typedef struct {
	TIM_TypeDef* tim;
	const u8 tim_af;
	const u32 tim_rcc;
	const GPIO* gpio;
	const u8 channel;
} ServoStruct;

typedef struct{
	const u16 min_ccr;
	const u16 max_ccr;
	const s16 min_deg;
	const s16 max_deg;
} ServoConfig;

#define X(a, b, c, d, e, f) a, 
typedef enum {
  SERVO_INIT_TABLE
} ServoID;
#undef X

#define X(a, b, c, d, e, f) {b, c, d, &e, f},
static const ServoStruct SERVO_STRUCT[] = {SERVO_INIT_TABLE};
#undef X

#define C(a, b, c, d, e) {b, c, d, e},
static const ServoConfig SERVO_CONFIG[] = {SERVO_CONFIG_TABLE};
#undef C

#define SERVO_SIZE (sizeof(SERVO_STRUCT)/sizeof(ServoStruct))

void servo_init(void);

/**
  * @brief  Control the ccr value of the servo
  * @param  servo_id: The servo id to be used
  * @param  val: Any value from 0~20000.
  * @retval None
  */
void servo_ccr_control(ServoID servo_id , u16 ccr_val);

/**
  * @brief  Control the degree of the servo
  * @param  servo_id: The servo id to be used
  * @param  val: Any value from MIN_DEG to MAX_DEG (defined in @SERVO_CONFIG_TABLE)
  * @retval None
  */
void servo_deg_control(ServoID servo_id , s16 degree);

#endif
