#ifndef _TRIGON_MATH_H
#define _TRIGON_MATH_H

/******************************************************
** TRIGON MATH - STM32F4
**
** A math library using lookup table and linear interpolation
** to estimate value of trigonometric functions
**
** ROBOCON 2016
** H K U S T
**
** Author:	Rex Cheng
** Contact:	hkchengad@connect.ust.hk
**
** v1.0 July 2016
**
** Geometry is not true, it is advantageous.  ~Henri Poincare
*******************************************************/

#include "stm32f4xx.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>

/** Calcute an approximated tan value.
	Be careful value around multiple of 90 deg will be very inaccurate.
	@param angle: 0 ~ 35999 (Scaled by 100)
	@return tan(angle) scaled by 16384
*/
int32_t app_tan(int32_t in);

#endif
