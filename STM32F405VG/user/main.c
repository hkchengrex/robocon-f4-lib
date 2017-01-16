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

void reset(void) {
	//SPI_Cmd(SPI3, DISABLE);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	//SPI_Cmd(SPI3, ENABLE);
}

void pull_down(void) {
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void pull_up(void) {
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

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
	btn_init();
	//btn_reg_OnClickListener(BOARD_BNT_1, &reset);
	
	btn_reg_OnClickListener(BOARD_BNT_2, &pull_down);
	btn_reg_OnReleaseListener(BOARD_BNT_2, &pull_up);
	pull_up();
	
//	encoder_init();
//	servo_init();
	spi_xbc_mb_init();
//	
//	tft_put_logo(85, 120);
	
	while(1){
		tft_clear();
		tft_set_text_color(WHITE);
		tft_prints(0, 0, "%d", get_ticks());
		
		switch(spi_xbc_get_connection()) {
			case SPI_XBC_DISCONNECTED:
				tft_set_text_color(GREY);
				tft_prints(0, 9, "[Disconnected]");
				break;
			case SPI_XBC_USB_DISCONNECTED:
				tft_set_text_color(GREY);
				tft_prints(0, 9, "[No XBC]");
				break;
			default:
				break;
		}
		
		//SPI_I2S_SendData(SPI3, 'A');
		//SPI_I2S_SendData(SPI3, 'A');
		//SPI_I2S_SendData(SPI3, 'A');
		//SPI_I2S_SendData(SPI3, 'A');
		
		//if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
		//	SPI_I2S_SendData(SPI3, 0x00);
		
		tft_prints(0, 1, "DG: %04X %X", spi_xbc_get_digital(), spi_xbc_get_back_buttons());
		tft_prints(0, 2, "LT: %d", spi_xbc_get_joy(XBC_JOY_LT));
		tft_prints(0, 3, "RT: %d", spi_xbc_get_joy(XBC_JOY_RT));
		tft_prints(0, 4, "LX: %d", spi_xbc_get_joy(XBC_JOY_LX));
		tft_prints(0, 5, "LY: %d", spi_xbc_get_joy(XBC_JOY_LY));
		tft_prints(0, 6, "RX: %d", spi_xbc_get_joy(XBC_JOY_RX));
		tft_prints(0, 7, "RY: %d", spi_xbc_get_joy(XBC_JOY_RY));
		tft_prints(0, 8, "%d", spi_get_temp());
		
		tft_update();
		btn_update();
	}
}
