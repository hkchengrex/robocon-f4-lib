/**
** H K U S T
** Robocon 2017
**
** F4 Library
**
** @Author Rex Cheng
** @Contact hkchengad@connect.ust.hk
*/

#include "main.h"

s32 start_ticks = 0;

void servo_move_1(){
	servo_ccr_control(SERVO1, 20);
}

void servo_move_2(){
	servo_deg_control(SERVO1, 40);
}

void servo_move_3(){
	servo_deg_control(SERVO1, 60);
}

void servo_move_4(){
	servo_deg_control(SERVO1, 80);
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	
	ticks_init();
//	adc_init();

	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	led_init();
//	buzzer_init();
	btn_init();
	timer_init();
//	encoder_init();
	servo_init();
//	uart_init(COM1, 115200);

	tft_put_logo(85, 120);
	
	btn_reg_OnClickListener(BUTTON_1, servo_move_1);
	btn_reg_OnClickListener(BUTTON_2, servo_move_4);
	
	s32 last_loop1_ticks = 0, last_loop2_ticks;
	while(1){
		s32 this_ticks = get_ticks();
		
		if (btn_pressed(BUTTON_1)){
			servo_ccr_control(SERVO1, 1000);
		}else{
			servo_ccr_control(SERVO1, 2000);
		}
		
		if (this_ticks - last_loop1_ticks >= LOOP1_MS){
			btn_update();
			last_loop1_ticks = this_ticks;
		}
		
		if (this_ticks - last_loop2_ticks >= LOOP2_MS){
			tft_clear();
			tft_println("%d", this_ticks);
			tft_println("%d %d", btn_pressed(BUTTON_1), btn_pressed(BUTTON_2));
			tft_println("%d", TIM_GetCounter(TIM7));
			tft_update();
			led_blink(LED_1);
			
			last_loop2_ticks = this_ticks;
		}
	}
}
