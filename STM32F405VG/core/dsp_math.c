#include "dsp_math.h"

/**
	Calcute an approximated cosine value.
	@param angle: 0 ~ 35999 (Scaled by 100)
	@return cos(angle) scaled by 32768 (-32768 ~ 32767)
*/
__INLINE int16_t dsp_cos(int16_t angle){
	angle %= 36000;
	while(angle<0){
		angle += 36000;
	}
	return arm_cos_q15((q15_t)((int32_t)angle*32767/35999));
}

/**
	Calcute an approximated sine value.
	@param angle: 0 ~ 35999 (Scaled by 100)
	@return sin(angle) scaled by 32768 (-32768 ~ 32767)
*/
__INLINE int16_t dsp_sin(int16_t angle){
	angle %= 36000;
	while(angle<0){
		angle += 36000;
	}
	return arm_sin_q15((q15_t)((s32)angle*32767/35999));
}
