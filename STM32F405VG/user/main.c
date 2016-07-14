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

	s32 starting_ticks = get_full_ticks();
	
	volatile float result[200] = {0.0f};
	for (u16 i=0;i<200;i++){
		result[i] = (float)rand() / (float)RAND_MAX;
	}
	
	for (u16 i=1;i<199;i++){
		result[i] = ((result[i]*65.37f*result[i-1] - result[i-2])+14.589f)/562.3458f;
	}

	for (u16 i=198;i>1;i--){
		result[i] = result[i]/result[i+1]/result[i-1];
	}
	
	s32 end_ticks = get_full_ticks();
	
	while (1) {
		tft_clear();
		tft_println("%d", get_ticks());
		tft_println("%d", end_ticks - starting_ticks);
		tft_println("%f", result[10]);
		tft_println("%f", result[30]);
		tft_println("%f", result[50]);
		tft_println("%f", result[70]);
		tft_println("%f", result[128]);
		tft_update();
	}

}


//__FPU_USED
//__FPU_PRESENT