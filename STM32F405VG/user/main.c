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

	volatile s32 result[10] = {0};
	s32 math_h_result[10];
	s32 original[10] = {0};
	for (u16 i=0;i<10;i++){
		float tmp = (float)rand() /(float)RAND_MAX *1000000.0f;
		original[i] = (s32)roundf(tmp);
		result[i] = (s32)roundf(tmp);
		math_h_result[i] = (s32)u32_sqrt(original[i]);
	}
	
	s32 starting_ticks = get_full_ticks();
	
	for (u16 i=0;i<10;i++){
		result[i] = Sqrt(original[i])/1000;
		//result[i] = u32_sqrt(original[i]);
		//result[i] = (s32)roundf(sqrtf(original[i]));
		//result[i] = (s32)roundf(_sqrtf(original[i]));
	}
	
	s32 end_ticks = get_full_ticks();
	
	s32 total_error = 0;
	for (u16 i=0;i<10;i++){
		total_error += result[i] - math_h_result[i];
	}
	
	while (1) {
		tft_clear();
		tft_println("%d", get_ticks());
		tft_println("%d", total_error);
		tft_println("%d", end_ticks - starting_ticks);
		
//		tft_println("%d %d", original[10], result[10]);
//		tft_println("%d %d", original[20], result[20]);
//		tft_println("%d %d", original[50], result[50]);
//		tft_println("%d %d", original[90], result[90]);
//		tft_println("%d %d", original[99], result[99]);
		
		tft_update();
	}

}
