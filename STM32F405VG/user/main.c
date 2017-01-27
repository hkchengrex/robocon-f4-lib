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

u32 ticks_img = 0;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	
	ticks_init();
//	adc_init();
//	
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	
	led_init();
	led_control(LED_2, Bit_SET);
	led_control(LED_1, Bit_RESET);
//	buzzer_init();
	btn_init();
	
//	encoder_init();
//	servo_init();
	spi_xbc_mb_init();
//	
//	tft_put_logo(85, 120);
	
	while(1){
		if (ticks_img != get_ticks()) {
			ticks_img = get_ticks();
						
			if (ticks_img % 50 == 5) {
				tft_clear();
				tft_set_text_color(WHITE);
				tft_prints(0, 0, "%d", get_ticks());
				
				switch(spi_xbc_get_connection()) {
					case SPI_XBC_DISCONNECTED:
						tft_set_text_color(GREY);
						tft_prints(0, 7, "[Disconnected]");
						break;
					case SPI_XBC_USB_DISCONNECTED:
						tft_set_text_color(GREY);
						tft_prints(0, 7, "[No XBC]");
						break;
					default:
						break;
				}
				
				//spi_xbc_mb_lcd_tx();
				tft_prints(0, 1, "DG: %04X %X", spi_xbc_get_digital(), spi_xbc_get_back_buttons());
				tft_prints(0, 2, "LT: %d", spi_xbc_get_joy(XBC_JOY_LT));
				tft_prints(10, 2, "RT: %d", spi_xbc_get_joy(XBC_JOY_RT));
				tft_prints(0, 3, "LX: %d", spi_xbc_get_joy(XBC_JOY_LX));
				tft_prints(10, 3, "LY: %d", spi_xbc_get_joy(XBC_JOY_LY));
				tft_prints(0, 4, "RX: %d", spi_xbc_get_joy(XBC_JOY_RX));
				tft_prints(10, 4, "RY: %d", spi_xbc_get_joy(XBC_JOY_RY));
				
				tft_update();
				btn_update();
			}
			
			if (ticks_img % 2000 == 0) {
				led_control(LED_2, Bit_SET);
				led_control(LED_1, Bit_RESET);
			}
			
			if (ticks_img % 2000 == 1000) {
				led_control(LED_1, Bit_SET);
				led_control(LED_2, Bit_RESET);
			}
				
		}
	}
}
