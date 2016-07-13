#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include "led.h"

/**
	PIN_ON_TOP = 		0
	PIN_ON_LEFT = 	1
	PIN_ON_BOTTOM =	2
	PIN_ON_RIGHT = 	3
*/
#define ORIENTATION_SETTING 2

#include "stm32f4xx_gpio.h"
#include "ticks.h"
#include "usart.h"
#include "can_protocol.h"
#include "encoder.h"
#include "buzzer.h"
#include "buzzer_song.h"
#include "adc.h"
#include "servo.h"

#include "button.h"
#include "lcd_main.h"

#ifndef ARM_MATH_CM4
	#define ARM_MATH_CM4
#endif
#include "dsp_math.h"
#include "approx_math.h"
#include "quick_math.h"

#endif 
