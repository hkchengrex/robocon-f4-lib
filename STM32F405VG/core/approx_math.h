#ifndef __APPROX_MATH_H
#define __APPROX_MATH_H

#include "stm32f4xx.h"
#include <stdlib.h>

extern s16 cos_val[91];



/**
  * @brief  Approximation of sin function
  * @param  a: angle in degree scaled by 10 (0-3600)
  * @retval sin(a) scaled by 10000 (0-10000)
  */
s32 int_sin(s32 a);

/**
  * @brief  Approximation of cos function
  * @param  a: angle in degree scaled by 10 (0-3600)
  * @retval cos(a) scaled by 10000 (0-10000)
  */
s32 int_cos(s32 a);

/**
  * @brief  Approximation of tan function
  * @param  a: angle in degree WITHOUT scaling
  * @retval tan(a) scaled by 100
  */
s16 int_tan(s16 a);

/**
  * @brief  Quick asin function approximation with 
  * @param  sin_val: input scaled by 10000 (-10000-10000)
  * @retval asin(sin_val) in degree WITHOUT scaling (-90-90)
  */
s16 int_arc_sin(s16 sin_val);

/**
  * @brief  Approximation of acos function
  * @param  cos_val: input scaled by 10000 (0-10000)
  * @retval acos(cos_val) in degree WITHOUT scaling
  */
s16 int_arc_cos(s16 cos_val);

/**
  * @brief  Approximation of atan function
  * @param  tan_val: input scaled by 100
  * @retval atan(y/x) in degree from -89 to 90 (ignore quarter)
  */
s16 int_arc_tan(s32 tan_val);

/**
  * @brief  Approximation of atan2 function
  * @param  y: input y
  * @param  x: input x
  * @retval atan(y/x) in degree from 0 to 359 WITHOUT scaling (correct quarter)
  */
s16 int_arc_tan2(s32 y, s32 x);

/**
  * @brief  Rapid sqrt approximation with maximum 0.297944% deviation at sqrt(2) and average 0.0184811% deviation
  * @param  v:	Input limited to 2^31 by variable type
  * @retval Scaled value of 1000*sqrt(v)
  */
u32 Sqrt(s32 v);

#endif		/*  __APPROX_MATH_H */
