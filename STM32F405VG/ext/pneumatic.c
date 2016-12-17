#include "pneumatic.h"

void pneu_init(){
	for (u8 i=0; i<(sizeof(PNEU_GPIO_ARRAY)/sizeof(GPIO*)); i++){
		gpio_output_init(PNEU_GPIO_ARRAY[i], GPIO_OType_PP, GPIO_PuPd_DOWN);
	}
}

void pneu_control(PNEU_ID id, BitAction state){
	gpio_write(PNEU_GPIO_ARRAY[id], state);
}

void pneu_toggle(PNEU_ID id){
	gpio_toggle(PNEU_GPIO_ARRAY[id]);
}
