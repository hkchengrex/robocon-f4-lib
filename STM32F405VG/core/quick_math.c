#include "quick_math.h"

#define ELEMENT_TYPE int32_t
ELEMENT_TYPE s32_median_of_3(ELEMENT_TYPE num1, ELEMENT_TYPE num2, ELEMENT_TYPE num3){
	if (num1 > num2){
		if (num2 > num3){
			return num2;
		}else if(num1 > num3){
			return num3;
		}else{
			return num1;
		}
	}else{
		if (num1 > num3){
			return num1;
		}else if(num2 > num3){
			return num3;
		}else{
			return num2;
		}
	}
}
#undef ELEMENT_TYPE

