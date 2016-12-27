#include "led.h"

/**
	Initialize all LEDs.
*/
void led_init(){
	for (u8 i=0; i<LED_COUNT; i++){
		gpio_output_init(LEDs[i], GPIO_OType_PP, GPIO_PuPd_DOWN);
	}
}

/**
	Control LED on/off
	@param id: the led to be controlled
	@param state: Bit_RESET: Off / Bit_SET: On
*/
void led_control(LedID id, BitAction state){
	gpio_write(LEDs[id], state);
}

/**
	Make the led blink
	@param id: the led to be controlled
*/
void led_blink(LedID id){
	gpio_toggle(LEDs[id]);
}
