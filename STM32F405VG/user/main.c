/**
** H K U S T
** Robocon 2017
**9
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
//	
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
//	led_init();
//	buzzer_init();
//	button_init();
//	encoder_init();
//	servo_init();
//	uart_init(COM1, 115200);
//	
//	tft_put_logo(85, 120);
	
	spi_xbc_mb_init();
	
	while(1){
		tft_clear();
		tft_prints(0, 0, "%d", get_ticks());
		tft_prints(0, 1, "DG: %04x", spi_xbc_get_digital());
		tft_prints(0, 2, "LT: %d", spi_xbc_get_joy(XBC_JOY_LT));
		tft_prints(0, 3, "RT: %d", spi_xbc_get_joy(XBC_JOY_RT));
		tft_prints(0, 4, "LX: %d", spi_xbc_get_joy(XBC_JOY_LX));
		tft_prints(0, 5, "LY: %d", spi_xbc_get_joy(XBC_JOY_LY));
		tft_prints(0, 6, "RX: %d", spi_xbc_get_joy(XBC_JOY_RX));
		tft_prints(0, 7, "RY: %d", spi_xbc_get_joy(XBC_JOY_RY));
		tft_prints(0, 8, "BK: %02x", spi_xbc_get_back_buttons());
		tft_update();
	}
}
