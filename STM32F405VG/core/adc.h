#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx_tim.h"

typedef enum{
	ADC_1 = 0,
	ADC_2, 
	ADC_3, 
	ADC_4, 
	ADC_5
} ADC_ID;

//Initalize all adc ports, and start taking readings automatically
void adc_init(void);

/**
** Get the latest adc reading (voltage level)
** @return Unsigned numerical reading representing voltage level
*/
u16 adc_get(ADC_ID adc_id);

#endif
