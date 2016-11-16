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

s32 last_data = 0;
bool time_error = false;
bool con_error = false;
s32 te_count = 0;
s32 ce_count = 0;
void rece_hand(CanRxMsg msg){
	u8 first_data = msg.Data[0];
	for (int i=1; i<8; i++){
		if (first_data != msg.Data[i]){
			con_error = true;
			ce_count++;
		}
	}
	
	if (first_data != (last_data+1)){
		time_error = true;
		te_count++;
	}else{
		last_data = first_data;
	}
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	ticks_init();
	
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	buzzer_init();
	
	can_init();
	can_rx_init();
	can_rx_add_filter(0x00, 0x00, rece_hand);
	
	tft_put_logo(85, 120);
	
	u32 last_ticks = 0;
	u32 data_since = 0;
	while(1){
		u32 this_ticks = get_full_ticks();
		if ((this_ticks - last_ticks) > 500){
			s32 data_diff = last_data - data_since;
			data_since = last_data;
			tft_clear();
			tft_println("T: %d", get_ticks());
			tft_println("D: %d", last_data);
			tft_println("R: %6.2f KB/s", data_diff*8.0f/2.0f/1024.0f);
			tft_println("Time error: %d", te_count);
			tft_println("Cont error: %d", ce_count);
			
			if (time_error){
				tft_set_text_color(RED);
				tft_println("TIME ERROR");
				time_error = false;
			}else{
				tft_set_text_color(GREEN);
				tft_println("TIME OK");
			}
			
			if (con_error){
				tft_set_text_color(RED);
				tft_println("CONT ERROR");
				con_error = false;
			}else{
				tft_set_text_color(GREEN);
				tft_println("CONT OK");
			}
			
			tft_update();
		}
	}
}
