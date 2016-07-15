/**
** H K U S T
** Robocon 2016
**
** F4 Library used for camera project
**
** @Author Rex Cheng
** @Contact hkchengad@connect.ust.hk
*/

#include "main.h"

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	
	ticks_init();
	adc_init();
	
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	led_init();
	buzzer_init();
	button_init();
	encoder_init();
	servo_init();
	
	tft_put_logo(85, 120);

	#define SAMPLES 100
	
	volatile s32 result[SAMPLES] = {0};
	s32 original[SAMPLES] = {0};
	
	for (u16 i=0;i<SAMPLES;i++){
		float32_t tmp = (float32_t)rand() /(float32_t)RAND_MAX *1000000.0f;
		original[i] = (s32)roundf(tmp);
	}
	
	s32 starting_ticks = get_full_ticks();
	
	for (u16 i=0;i<SAMPLES;i++){
		//result[i] = Sqrt(original[i]);
		//result[i] = s32_sqrt(original[i]);
		result[i] = s32_sqrt2(original[i]);
	}
	
	s32 end_ticks = get_full_ticks();
	
	float total_error = 0;
	for (u16 i=0;i<SAMPLES;i++){
		total_error += (result[i] - sqrt(original[i])*1024.0f)/1024.0f*100.0f/original[i];
	}
	
	while (1) {
		tft_clear();
		tft_println("%d", get_ticks());
		tft_println("%f", total_error/SAMPLES);
		tft_println("%d", end_ticks - starting_ticks);
		
//		tft_println("%d %d", original[10], result[10]);
//		tft_println("%d %d", original[20], result[20]);
//		tft_println("%d %d", original[50], result[50]);
//		tft_println("%d %d", original[90], result[90]);
//		tft_println("%d %d", original[99], result[99]);
		
		tft_update();
	}

}
