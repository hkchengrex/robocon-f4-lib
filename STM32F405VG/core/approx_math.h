#ifndef __APPROX_MATH_H
#define __APPROX_MATH_H

#include "stm32f4xx.h"
#include <stdlib.h>

extern s16 cos_val[91];

struct Cartesian {
	s32 x, y;
};

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
  * @brief  Matrix rotation
  * @param  x-coor (Pass by REF), y-coor (Pass by REF), angle to rotate (anti-clockwise)
  * @retval none
  */
void xy_rotate(s32 *x, s32 *y, s32 w);

/**
  * @brief A proper modulus (result is always positive, which ((-n) % p) =/= -(n % p)
  * @param dividor: n
  * @param divisor: p
  * @retval The modulus (n % p)
  * @example 24 % 7 return 3, -24 % 7 returns 4
  */
s32 p_mod(s32 dividor, s32 divisor);

/**
  * @brief Square of x
  * @param x: input
  * @retval x^2 
  */
s32 Sqr (s32 x);

/**
  * @brief  Sqrt calculation using binary search, run-time = O(log n)
  * @param  num: the integer inside the root
  * @retval square root of num
  */
u32 Sqrt(u32 num);

#endif		/*  __APPROX_MATH_H */
