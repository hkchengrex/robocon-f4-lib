/**
** H K U S T
** Robocon 2016
**
** F4 Library used for camera project
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
	adc_init();
	
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	led_init();
	buzzer_init();
	button_init();
	encoder_init();
	servo_init();
	uart_init(COM1, 115200);
	
	tft_put_logo(85, 120);
	
	volatile s32 result[360] = {0};

	s32 starting_ticks = get_full_ticks();
	u8 end_counter = 0;
	for (u16 i=0;i<360;i++){
		u16 testing_angle = i*100 + end_counter;
		//result[i] = cosf(testing_angle*PI/180.0f/100.0f)*10000;
		//result[i] = dsp_cosf(testing_angle*PI/180.0f/100.0f)*10000;
		//result[i] = int_cos(testing_angle/10);
		result[i] = dsp_cos(testing_angle);
		end_counter = (end_counter+1)%100;
	}
	
	s32 end_ticks = get_full_ticks();
	
	#define scale 32768.0
	end_counter = 0;
	double total_error = 0;
	for (u16 i=0;i<36;i++){
		float error[10];
		//tft_clear();
		//tft_println("%d %d", i, 0);
		for (u16 j=0;j<5;j++){
			u16 testing_angle = i*1000 + j*100 + end_counter;
			error[j] = (result[i*10+j] - cos(testing_angle*PI/180.0/100.0)*scale)/scale*1000.0;
			total_error += fabs(error[j]);
			end_counter = (end_counter+1)%100;
			//tft_println("%f", error[j]);
		}
		//tft_update();
		//while(!button_pressed(BUTTON_1));
		//while(button_pressed(BUTTON_1));
		//tft_clear();
		//tft_println("%d %d", i, 1);
		for (u16 j=5;j<10;j++){
			u16 testing_angle = i*1000 + j*100 + end_counter;
			error[j] = (result[i*10+j] - cos(testing_angle*PI/180.0/100.0)*scale)/scale*1000.0;
			total_error += fabs(error[j]);
			end_counter = (end_counter+1)%100;
			//tft_println("%f", error[j]);
		}
		//tft_update();
		//while(!button_pressed(BUTTON_1));
		//while(button_pressed(BUTTON_1));
	}
	
//	while(1){
//		#define k 18
//		tft_clear();
//		for (u8 i=0;i<10;i++){
//			u16 testing_angle = (k*10+i)*10;
//			tft_println("%f",  (result[k*10+i] - sin(testing_angle*PI/180.0f/10.0f)*32768.0f)/32768.0f*100.0f);
//		}
//		tft_update();
//	}
	
	while (1) {
		tft_clear();
		tft_println("%d", get_ticks());
		tft_println("%f", total_error/360);
		tft_println("%d", end_ticks - starting_ticks);
		
//		tft_println("%d", dsp_sin(35999));
//		tft_println("%d", dsp_sin(0));
//		tft_println("%d", dsp_sin(18000));
//		tft_println("%d", dsp_sin(27000));
//		tft_println("%d", dsp_sin(9000));
		
//		tft_println("%d %f", result[10], sin(10*PI/180.0f)*32768);
//		tft_println("%f", (result[10] - sin(100*PI/180.0f/10.0f)*32768.0f)/32768.0f*100.0f);
//		tft_println("%d %f", result[5], sin(5*PI/180.0f)*32768);
//		tft_println("%d %f", result[60], sin(60*PI/180.0f)*32768);
//		tft_println("%d %f", result[180], sin(180*PI/180.0f)*32768);
//		tft_println("%d %f", result[359], sin(359*PI/180.0f)*32768);
		
		tft_update();
	}

}
