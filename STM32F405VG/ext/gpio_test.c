#include "gpio_test.h"

//Start the test
void gpio_test(){
	for (u32 curr=0; curr<GPIO_SIZE; curr++){
		gpio_output_init(GPIOArray[curr], GPIO_OType_OD, GPIO_PuPd_NOPULL);
		for (u32 other=0; other<GPIO_SIZE; other++){
			if (curr == other){
				continue;
			}
			gpio_input_init(GPIOArray[other], GPIO_PuPd_UP);
		}
		
		_delay_ms(5);
		
		for (u32 other=0; other<GPIO_SIZE; other++){
			if (curr == other){
				continue;
			}
			tft_clear();
			tft_println("%d", get_ticks());
			tft_println("Current output: ");
			tft_println("Port: %c", (curr/15 + 65));
			tft_println("Pin:  %d", curr%15);
			tft_println("");
			tft_println("Current input: ");
			tft_println("Port: %c", (other/15 + 65));
			tft_println("Pin:  %d", other%15);
			
			u8 result = gpio_read_input(GPIOArray[other]);
			tft_println("Result: %d", result);
			tft_update();
			
			if (result == 0){
				while(!btn_pressed(BUTTON_1));
				while(btn_pressed(BUTTON_1));
			}
		}
	}
}
