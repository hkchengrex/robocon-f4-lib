#include "button.h"

static u16 pressedTime[BTN_COUNT] = {0};

static OnClickListener clickListeners[BTN_COUNT] = {0};
static OnReleaseListener releaseListeners[BTN_COUNT] = {0};

typedef struct{
	OnHoldListener holdListener;
	s16 holdThreshold;
	s16 trigFrequency;
}HoldListenerStruct;

static HoldListenerStruct holdListeners[BTN_COUNT] = {0};

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
			if (pressedTime[i]==0 && clickListeners[i] !=0 ){
				//Capture the instant when the button is pressed
				clickListeners[i]();
			}
			
			if (holdListeners[i].holdListener != 0){
				if (pressedTime[i] > holdListeners[i].holdThreshold && (pressedTime[i]%holdListeners[i].trigFrequency==0)){
					//Trigger the hold listener
					holdListeners[i].holdListener();
				}
			}
			
			pressedTime[i]++;
			
		}else{
			//If the button is not pressed
			if (pressedTime[i] > 0 && releaseListeners[i] != 0){
				//Capture the instant when the button is released
				releaseListeners[i]();
			}
			pressedTime[i] = 0;
		}
	}
}

//Called when the button is first pressed
void btn_reg_OnClickListener(ButtonID button_id, OnClickListener listener){
	clickListeners[button_id] = listener;
}

//Called when the button is released 
void btn_reg_OnReleaseListener(ButtonID button_id, OnReleaseListener listener){
	releaseListeners[button_id] = listener;
}

//Called every "frequency" after "threshold" have been reached. Actual time depends on frequency of @button_update()
void btn_reg_OnHoldListener(ButtonID button_id, u16 threshold, u16 frequency, OnHoldListener listener){
	holdListeners[button_id].holdListener = listener;
	holdListeners[button_id].holdThreshold = threshold;
	holdListeners[button_id].trigFrequency = frequency;
}

void btn_disable_OnClickListener(ButtonID button_id){
	clickListeners[button_id] = 0;
}

void btn_disable_OnReleaseListener(ButtonID button_id){
	releaseListeners[button_id] = 0;
}

void btn_disable_OnHoldListener(ButtonID button_id){
	holdListeners[button_id].holdListener = 0;
}

