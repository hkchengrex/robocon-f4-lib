#ifndef _QUICK_MATH_H
#define _QUICK_MATH_H

/******************************************************
** QUICK MATH - STM32F4
**
** ROBOCON 2016
** H K U S T
**
** Author:	Rex Cheng
** Contact:	hkchengad@connect.ust.hk
**
** v1.0 March 2016
** v1.1 July 2016
**
** Speed is the heart of battle. ~Dark Seer
*******************************************************/

#include "stm32f4xx.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>

/**
** These function tries to cap a variable within a specific range.
** The inline property tries to remove the function call and make it runs faster
**/
inline int16_t s16_cap(int16_t in_num, int16_t upper_bound, int16_t lower_bound){
	return in_num>upper_bound?upper_bound:in_num<lower_bound?lower_bound:in_num;
}


inline uint16_t u16_cap(uint16_t in_num, uint16_t upper_bound, uint16_t lower_bound){
	return in_num>upper_bound?upper_bound:in_num<lower_bound?lower_bound:in_num;
}

inline int32_t s32_sqrt(int32_t in_num){
	return (s32)(__sqrtf(in_num)*1024.0f + 0.5f);
}

int32_t s32_median_of_3(s32 num1, s32 num2, s32 num3);

#endif
