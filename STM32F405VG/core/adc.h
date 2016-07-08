#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx_tim.h"

//Initalize all adc ports, and start taking readings automatically
void adc_init(void);

/**
** Get the latest adc reading (voltage level)
** @return Unsigned numerical reading
*/
u16 adc_get(u8 i);

#endif
