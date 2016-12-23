#ifndef	_BUTTON_H
#define	_BUTTON_H

/******************************************************
** BUTTON - STM32F4
**
** This library provides functions for reading from buttons (or switches)
** It provides btn_pressed function to read input from buttons, adapted with the respective pull-up/pull-down
** It also provides some listener functions, which helps to respond to input easily
** Feel free to add more "buttons" down there in the BTN_TABLE, it works for all GPIO switches, like limit switches or IR switches
**
** ROBOCON 2017
** H K U S T
**
** Author:	Rex Cheng
** Contact:	hkchengad@connect.ust.hk
**
** v1.0 December 2016
**
** "You press the button, we do the rest." ~ Slogan of Kodak Camera
*******************************************************/

#include "stm32f4xx.h"
#include <stdbool.h>
#include "gpio.h"

//BUTTON       PIN  Pull-up/Pull-down
#define BTN_TABLE \
X(BOARD_BNT_1, PD6, GPIO_PuPd_UP) \
X(BOARD_BNT_2, PD7, GPIO_PuPd_UP) \
X(JS_BUT_A, PD0, GPIO_PuPd_UP) \
X(JS_BUT_B, PD1, GPIO_PuPd_UP) \
X(JS_BUT_C, PD3, GPIO_PuPd_UP) \
X(JS_BUT_D, PD4, GPIO_PuPd_UP) \
X(JS_BUT_M, PD5, GPIO_PuPd_UP) \
X(LIM_SW_1, PE0, GPIO_PuPd_DOWN)

#define X(a, b, c) a, 
typedef enum{
	BTN_TABLE
}ButtonID;
#undef X

typedef struct{
	const GPIO* gpio;
	const GPIOPuPd_TypeDef PuPd;
}ButtonStruct;

#define X(a, b, c) {&b, c},
static const ButtonStruct BUTTONS[] = {BTN_TABLE};
#undef X

#define BTN_COUNT (sizeof(BUTTONS)/sizeof(GPIO*))

typedef void(*onClickListener)(void);
typedef void(*onReleaseListener)(void);
typedef void(*onHoldListener)(void);

//Init buttons
void btn_init(void);

//To be called at a regular time interval. Button event will be triggered in this function
void btn_update(void);

//These functions are for registering listener for buttons
void btn_reg_onClickListener(ButtonID button_id, onClickListener listener); //Called when the button is first pressed
void btn_reg_onReleaseListener(ButtonID button_id, onReleaseListener listener); //Called when the button is released 
//Called every "frequency" after "threshold" have been reached. Actual time depends on frequency of @button_update()
void btn_reg_onHoldListener(ButtonID button_id, u16 threshold, u16 frequency, onHoldListener listener); 

//These functions are to disable registered listener
void btn_disable_onClickListener(ButtonID button_id);
void btn_disable_onReleaseListener(ButtonID button_id);
void btn_disable_onHoldListener(ButtonID button_id);

//Return the state of the selected button, 1 if pressed
bool btn_pressed(ButtonID button_id);

#endif /* __LED_H */
