#ifndef _GP2Y0A41SK_H
#define _GP2Y0A41SK_H

/**
* A library for GP2Y0A41SK ADC IR sensor
* Range: 4~30 cm, out of range reading is unreliable
*	Within range, voltage level has linear relationship with 1/distance
**/

#include "stm32f4xx.h"
#include "adc.h"

/**
	Return a range that lies between 4~30 cm, out of range reading is unreliable.
	The output signal has some weird noise, consider adding capacitor.
	@return measured IR distance in mm
*/
u16 get_ir_dis(ADC_ID adc_id);

#endif
