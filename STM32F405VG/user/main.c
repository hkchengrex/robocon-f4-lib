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
		//while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
		tft_clear();
		tft_prints(0, 0, "%d", get_ticks());
		tft_prints(0, 1, "%d", spi_get_count());
		tft_prints(0, 2, "%d", spi_get_count2());
		tft_update();
	}
}
