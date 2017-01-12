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
	
	comm_init(COM1, 115200);
	
	//do_after_for(buzzer_on, 1, 300, 3);
	//do_after_for(buzzer_off, 150, 300, 3);

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
			tft_println("%d", get_ticks());
			tft_println("%d", get_encoder_value(MOTOR_1));
			tft_println("%d", get_buf_size(COM1));
			tft_update();
			
			led_blink(LED_1);
			
			comm_tx_pos();
			comm_tx_motor();
			
			last_loop2_ticks = this_ticks;
		}
	}
}
