#include "dsp_math.h"

s32 dsp_cos(s32 angle){
	arm_cos_q31(angle*65535/360*4294967295);
}
