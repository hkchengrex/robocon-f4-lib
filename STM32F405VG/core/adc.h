#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx_tim.h"

#define ADC_COUNT 5

typedef enum{
	ADC_PORT_1 = 0,
	ADC_PORT_2, 
	ADC_PORT_3, 
	ADC_PORT_4, 
	ADC_PORT_5
} ADC_ID;

//Initalize all adc ports, and start taking readings automatically
void adc_init(void);

/**
** Get the latest adc reading (voltage level)
** @return Unsigned numerical reading representing voltage level
*/
u16 adc_get(ADC_ID adc_id);

#endif
