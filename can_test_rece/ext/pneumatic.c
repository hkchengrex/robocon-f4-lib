#include "pneumatic.h"

static const GPIO* PNEUMATIC_GPIO_ARRAY[PNEUMATIC_COUNT] = {&PNEUMATIC_1_GPIO, &PNEUMATIC_2_GPIO, &PNEUMATIC_3_GPIO, &PNEUMATIC_4_GPIO,&PNEUMATIC_5_GPIO};

void pneumatic_init(){
	for (u8 i=0; i<PNEUMATIC_COUNT; i++){
		gpio_output_init(PNEUMATIC_GPIO_ARRAY[i], GPIO_OType_PP, GPIO_PuPd_DOWN);
	}
}

void pneumatic_control(PNEUMATIC_ID id, BitAction state){
	gpio_write(PNEUMATIC_GPIO_ARRAY[id], state);
}

void pneumatic_toggle(PNEUMATIC_ID id){
	gpio_toggle(PNEUMATIC_GPIO_ARRAY[id]);
}
