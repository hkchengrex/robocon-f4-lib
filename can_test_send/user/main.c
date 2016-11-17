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

u32 last_data = 0;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	gpio_rcc_init_all();
	
	ticks_init();
	
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	buzzer_init();
	
	button_init();
	can_init();
	
	tft_put_logo(85, 120);
	
	u32 last_ticks = 0;
	u32 last_long_ticks = 0;
	
	float speed = 5.0f; //In KB/s
	float unsend_byte = 0.0f;
	while(1){
		u32 this_ticks = get_full_ticks();
		
		if (this_ticks != last_ticks){
			
			if (button_pressed(BUTTON_1)){
				speed += 0.02f;
			}else if(button_pressed(BUTTON_3)){
				speed -= 0.02f;
			}

			unsend_byte += (this_ticks - last_ticks) * speed  * 1024 / 1000;
			while(unsend_byte > 8.0f){
				CAN_MESSAGE msg;
				msg.id = 0;
				msg.length = 8;
				for (int i=0; i<2; i++){
					msg.data[i*4] = last_data >> 24;
					msg.data[i*4+1] = last_data >> 16;
					msg.data[i*4+2] = last_data >> 8;
					msg.data[i*4+3] = last_data;
				}
				can_tx_enqueue(msg);
				last_data++;
				unsend_byte -= 8.0f;
			}
			
			if ((this_ticks - last_long_ticks) > 500){
				tft_set_text_color(WHITE);
				tft_clear();
				tft_println("Build: ");
				tft_println("%s %s", __TIME__, __DATE__);
				tft_println("T: %d", get_full_ticks());
				tft_println("D: %d", last_data);
				tft_println("R: %6.2f KB/s", speed);
				
				tft_update();
				
				last_long_ticks = this_ticks;
			}
			
			last_ticks = this_ticks;
		}
	}
}
