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

/**
	Perform a square root operation.
	Although it takes and returns signed value, they are all positive.
	@param input value, 0 ~ 0x7FFF
	@return sqrt(in)
*/
__INLINE int16_t dsp_sqrt_16(int16_t in){
	int16_t out;
	arm_sqrt_q15(in, &out);
	return out;
}

/**
	Perform a square root operation.
	Although it takes and returns signed value, they are all positive.
	@param input value, 0 ~ 0x7FFFFFFF
	@return sqrt(in)
*/
__INLINE int32_t dsp_sqrt_32(int32_t in){
	int32_t out;
	arm_sqrt_q31(in, &out);
	return out;
}
