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
	uart_init(COM1, 115200);
	
	tft_put_logo(85, 120);
	
	volatile s32 result[360] = {0};

	s32 starting_ticks = get_full_ticks();
	u8 end_counter = 0;
	for (u16 i=0;i<360;i++){
		u16 testing_angle = i*100 + end_counter;
		//result[i] = int_tan(testing_angle/100);
		result[i] = app_tan(testing_angle);
		end_counter = (end_counter+1)%100;
	}
	
	s32 end_ticks = get_full_ticks();
	
	#define scale 16384.0
	end_counter = 0;
	double total_error = 0;
	for (u16 i=0;i<360;i++){
		tft_clear();
		tft_println("%d %d", i, 0);
		u16 testing_angle = i*100 + end_counter;
		float error = (result[i] - tan(testing_angle*PI/180.0/100.0)*scale)/scale*1000.0;
		total_error += fabs(error);
		end_counter = (end_counter+1)%100;
		tft_println("%d", testing_angle);
		tft_println("%d", result[i]);
		tft_println("%f", tan(testing_angle*PI/180.0/100.0)*scale);
		tft_println("%f", error);
		tft_update();
		//while(!button_pressed(BUTTON_1));
		//while(button_pressed(BUTTON_1));
	}
	
	while (1) {
		tft_clear();
		tft_println("%d", get_ticks());
		tft_println("%f", total_error/360);
		tft_println("%d", end_ticks - starting_ticks);
		
		tft_update();
	}

}
