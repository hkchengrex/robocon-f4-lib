#ifndef _PNEUMATIC_H
#define _PNEUMATIC_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "gpio.h"

#define PNEUMATIC_COUNT 5

typedef enum{
	PNEUMATIC_1 = 0,
	PNEUMATIC_2, 
	PNEUMATIC_3, 
	PNEUMATIC_4, 
	PNEUMATIC_5
} PNEUMATIC_ID;

#if (PNEUMATIC_ID+1)!=PNEUMATIC_COUNT
	#error Pneumatic count error :(
#endif

#define PNEUMATIC_1_GPIO PC4
#define PNEUMATIC_2_GPIO PC4
#define PNEUMATIC_3_GPIO PC4
#define PNEUMATIC_4_GPIO PC4
#define PNEUMATIC_5_GPIO PC4

/**
	Initialize all pneumatic ports.
*/
void pneumatic_init(void);

/**
	Control pneumatic valve.
	@param id: the pneumatic to be controlled
	@param state: Bit_RESET/Bit_SET, where Bit_RESET refers to original(uncontrolled) state 
*/
void pneumatic_control(PNEUMATIC_ID id, BitAction state);

/**
	Toggle the pneumatic valve.
	@param id: the pneumatic to be controlled
*/
void pneumatic_toggle(PNEUMATIC_ID id);

#endif
