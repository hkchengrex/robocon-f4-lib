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
int16_t s16_cap(int16_t in_num, int16_t upper_bound, int16_t lower_bound);
uint16_t u16_cap(uint16_t in_num, uint16_t upper_bound, uint16_t lower_bound);
int32_t s32_sqrt(int32_t in_num);

/**
** A quick select algorithm from N. Wirth's "Algorithms + data structures = programs"
** It is used to find the kth smallest element in the array
** It can be used to find median (median filter), ~25 times faster than quicksort method
** Feel free to change ELEMENT_TYPE to implement versions for different variable type, as long as they are comparable
**
** @param list: Array of items to be searched.
** @param len: Length of array.
** @param k: which element to be searched (kth smallest)
** @param copy_array: whether this method should change the original array or not, if it is set to TRUE, 
** the array will be copied and the original array will NOT be modified as a side effect
*/
int16_t s16_kth_smallest(int16_t list[], u16 len, u16 k, bool copy_array);

#endif
