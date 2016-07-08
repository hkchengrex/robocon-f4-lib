#ifndef _QUICK_MATH_H
#define _QUICK_MATH_H

#include "stm32f4xx.h"
#include <stdbool.h>
#include <string.h>

extern __inline int16_t s16_cap(int16_t in_num, int16_t upper_bound, int16_t lower_bound);
extern __inline uint16_t u16_cap(uint16_t in_num, uint16_t upper_bound, uint16_t lower_bound);
extern __inline uint8_t u16_sqrt(uint16_t in_num);
extern __inline uint16_t u32_sqrt(uint32_t in_num);

#endif
