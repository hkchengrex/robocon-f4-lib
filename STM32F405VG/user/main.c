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

void penu_all_toggle(){
	for (u8 i=0; i<PNEU_COUNT; i++){
		pneu_toggle((PneuID)i);
	}
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	
	ticks_init();
	adc_init();

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
	btn_reg_OnClickListener(BUTTON_1, penu_all_toggle);

	int32_t last_loop1_ticks = 0, last_loop2_ticks = 0;
	while(1){
		int32_t this_ticks = get_ticks();
		
		if (this_ticks - last_loop1_ticks >= LOOP1_MS){
			btn_update();
			last_loop1_ticks = this_ticks;
		}
		
		if (this_ticks - last_loop2_ticks >= LOOP2_MS){
			tft_clear();
			tft_println("%d", SystemCoreClock);
			tft_println("%d", get_ticks());
			tft_println("%d %u", comm_get_handshake_state(), last_data);
			tft_println("%d %d", get_adc(TEMPERATURE_ADC), get_adc(VOLTAGE_ADC));
			tft_println("%d.%d", get_mcu_temp()/10, get_mcu_temp()%10);
			tft_println("%d %d %d", get_adc(ADC_PORT_1), get_adc(ADC_PORT_2), get_adc(ADC_PORT_3));
			tft_println("%d %d %d", get_adc(ADC_PORT_4), get_adc(ADC_PORT_5), get_adc(ADC_PORT_6));
			tft_println("%d %d %d", get_adc(ADC_PORT_7), get_adc(ADC_PORT_8), get_adc(ADC_PORT_9));
			tft_println("%d", get_adc(ADC_PORT_10));
			tft_update();
			
			led_blink(LED_1);
			
			comm_tx_pos();
			comm_tx_motor();
			
			last_loop2_ticks = this_ticks;
		}
	}
}
