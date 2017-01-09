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

void on_pneu(){
	pneu_toggle(CLIMBING_PNEU);
}

s32 m1 = 0, m2 = 0;

void add_m1(){
	m1++;
}	

void add_m2(){
	m2++;
}

void go(){
	motor_set_vel(MOTOR_1, m1, CLOSE_LOOP);
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	
	ticks_init();
//	adc_init();

	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	//gpio_test();
	
	led_init();
	buzzer_init();
	btn_init();
	timer_init();
	encoder_init();
	servo_init();
	can_init();
	can_rx_init();
	motor_init();
	pneu_init();

	tft_put_logo(85, 120);
	
	//do_after_for(buzzer_on, 1, 300, 3);
	//do_after_for(buzzer_off, 150, 300, 3);
	
	btn_reg_OnClickListener(BUTTON_1, go);
	btn_reg_OnClickListener(JOYSTICK_N, add_m1);
	btn_reg_OnClickListener(JOYSTICK_S, add_m2);

	s32 last_loop1_ticks = 0, last_loop2_ticks = 0;
	while(1){
		s32 this_ticks = get_ticks();
		
		if (this_ticks - last_loop1_ticks >= LOOP1_MS){
			btn_update();
			last_loop1_ticks = this_ticks;
		}
		
		if (this_ticks - last_loop2_ticks >= LOOP2_MS){
			tft_clear();
			tft_println("%d", SystemCoreClock);
			tft_println("%d", this_ticks);
			//tft_println("%d %d", btn_pressed(JOYSTICK_N), btn_pressed(JOYSTICK_S));
			tft_println("%d %d", m1, m2);
			tft_println("%d", get_encoder_value(MOTOR_1));
			tft_update();
			
			motor_set_vel(MOTOR_1, m1, CLOSE_LOOP);
			//motor_set_vel(MOTOR_2, m2, OPEN_LOOP);
			led_blink(LED_1);
			
			last_loop2_ticks = this_ticks;
		}
	}
}
