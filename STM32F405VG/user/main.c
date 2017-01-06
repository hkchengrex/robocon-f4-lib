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

volatile char recv[6] = {0};
volatile u32 count = 0;

void recv0 (const u8 data){
	recv[0] = data;
	count++;
}

void recv1 (const u8 data){
	recv[1] = data;
	count++;
}

void recv2 (const u8 data){
	recv[2] = data;
	count++;
}

void recv3 (const u8 data){
	recv[3] = data;
	count++;
}

void recv4 (const u8 data){
	recv[4] = data;
	count++;
}

void recv5 (const u8 data){
	recv[5] = data;
	count++;
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	
	ticks_init();
//	adc_init();

	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	led_init();
	buzzer_init();
	btn_init();
	timer_init();
	encoder_init();
	servo_init();
	uart_init(COM1, 115200);
	uart_init(COM2, 115200);
	uart_init(COM3, 115200);
	uart_init(COM4, 115200);
	uart_init(COM5, 115200);
	uart_init(COM6, 115200);
	uart_interrupt_init(COM1, recv0);
	uart_interrupt_init(COM2, recv1);
	uart_interrupt_init(COM3, recv2);
	uart_interrupt_init(COM4, recv3);
	uart_interrupt_init(COM5, recv4);
	uart_interrupt_init(COM6, recv5);
//	can_init();
//	can_rx_init();
//	motor_init();

	tft_put_logo(85, 120);
	
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
			tft_println("%d", this_ticks);
			tft_println("%d", get_encoder_count());
			tft_println("%d", count);
			tft_println("%c %c %c", recv[0], recv[1], recv[2]);
			tft_println("%c %c %c", recv[3], recv[4], recv[5]);
			tft_update();
			led_blink(LED_1);
			
			uart_tx(COM1, "%d", get_ticks());
			uart_tx(COM2, "%d", get_ticks());
			uart_tx(COM3, "%d", get_ticks());
			uart_tx(COM4, "%d", get_ticks());
			uart_tx(COM5, "%d", get_ticks());
			uart_tx(COM6, "%d", get_ticks());
			
			last_loop2_ticks = this_ticks;
		}
	}
}
