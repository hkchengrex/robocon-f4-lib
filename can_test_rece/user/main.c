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
bool time_err = false;
bool cont_err = false;
s32 te_count = 0;
s32 ce_count = 0;
void recv_hand(CanRxMsg msg){
	u32 first_data = (u32)msg.Data[0]<<24 | (u32)msg.Data[1]<<16 | (u32)msg.Data[2]<<8 | (u32)msg.Data[3];

	u32 sec_data = (u32)msg.Data[4]<<24 | (u32)msg.Data[5]<<16 | (u32)msg.Data[6]<<8 | (u32)msg.Data[7];
	
	if (first_data != sec_data){
		cont_err = true;
		ce_count++;
	}
	
	if (first_data != (last_data+1)){
		time_err = true;
		te_count++;
	}
	last_data = first_data;
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	gpio_rcc_init_all();
	
	ticks_init();
	
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	buzzer_init();
	
	can_init();
	can_rx_init();
	can_rx_add_filter(0x00, 0x00, recv_hand);
	
	tft_put_logo(85, 120);
	
	u32 last_ticks = 0;
	u32 data_since = 0;
	
	while(1){
		u32 this_ticks = get_full_ticks();
		
		if ((this_ticks - last_ticks) > 500){
			
			s32 data_diff = last_data - data_since;
			data_since = last_data;
			tft_set_text_color(WHITE);
			tft_clear();
			tft_println("Build: ");
			tft_println("%s %s", __TIME__, __DATE__);
			tft_println("T: %d", get_full_ticks());
			tft_println("D: %d", last_data);
			tft_println("R: %6.2f KB/s", data_diff*8.0f*1000/(this_ticks - last_ticks)/1024);
			tft_println("TC: %d", te_count);
			tft_println("CC: %d", ce_count);
			
			if (time_err){
				tft_set_text_color(RED);
				tft_println("TIME ERROR");
				time_err = false;
			}else{
				tft_set_text_color(GREEN);
				tft_println("TIME OK");
			}
			
			if (cont_err){
				tft_set_text_color(RED);
				tft_println("CONT ERROR");
				cont_err = false;
			}else{
				tft_set_text_color(GREEN);
				tft_println("CONT OK");
			}
			
			tft_update();
			
			last_ticks = this_ticks;
		}
	}
}
