#include "button.h"

static u16 btn_press_time[BTN_COUNT] = {0};

static void(*click_listeners[BTN_COUNT])(void)  = {0};
static void(*release_listeners[BTN_COUNT])(void) = {0};

typedef struct{
	void(*hold_listener)(void);
	s16 hold_thre;
	s16 trig_freq;
}HoldListenerStruct;

static HoldListenerStruct hold_listeners[BTN_COUNT] = {0};

//Init buttons
void btn_init(){
	for (u8 i=0; i<BTN_COUNT; i++){
		gpio_input_init(BUTTONS[i].gpio, BUTTONS[i].PuPd);
	}
}

//Return the state of the selected button, 1 if pressed
bool btn_pressed(ButtonID button_id){
	//For pull up, 1 is normal state, 0 is pressed
	if (BUTTONS[button_id].PuPd == GPIO_PuPd_UP){
		return !gpio_read_input(BUTTONS[button_id].gpio);
	}else{
		return gpio_read_input(BUTTONS[button_id].gpio);
	}
}

//To be called at a regular time interval. Button event will be triggered in this function
void btn_update(){
	for (u8 i=0; i<BTN_COUNT; i++){
		if (btn_pressed((ButtonID)i)){
			//If the button is pressed
			if (btn_press_time[i]==0 && click_listeners[i] !=0 ){
				//Capture the instant when the button is pressed
				click_listeners[i]();
			}
			
			if (hold_listeners[i].hold_listener != 0){
				if (btn_press_time[i] > hold_listeners[i].hold_thre && (btn_press_time[i]%hold_listeners[i].trig_freq==0)){
					//Trigger the hold listener
					hold_listeners[i].hold_listener();
				}
			}
			
			btn_press_time[i]++;
			
		}else{
			//If the button is not pressed
			if (btn_press_time[i] > 0 && release_listeners[i] != 0){
				//Capture the instant when the button is released
				release_listeners[i]();
			}
			btn_press_time[i] = 0;
		}
	}
}

//Called when the button is first pressed
void btn_reg_onClickListener(ButtonID button_id, onClickListener listener){
	click_listeners[button_id] = listener;
}

//Called when the button is released 
void btn_reg_onReleaseListener(ButtonID button_id, onReleaseListener listener){
	release_listeners[button_id] = listener;
}

//Called every "frequency" after "threshold" have been reached. Actual time depends on frequency of @button_update()
void btn_reg_onHoldListener(ButtonID button_id, u16 threshold, u16 frequency, onHoldListener listener){
	hold_listeners[button_id].hold_listener = listener;
	hold_listeners[button_id].hold_thre = threshold;
	hold_listeners[button_id].trig_freq = frequency;
}

void btn_disable_onClickListener(ButtonID button_id){
	click_listeners[button_id] = 0;
}

void btn_disable_onReleaseListener(ButtonID button_id){
	release_listeners[button_id] = 0;
}

void btn_disable_onHoldListener(ButtonID button_id){
	hold_listeners[button_id].hold_listener = 0;
}

