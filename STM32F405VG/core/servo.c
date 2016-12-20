#include "servo.h"

void servo_init(void){

	GPIO_InitTypeDef SERVO_GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(SERVO_TIM_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(SERVO_GPIO_RCC, ENABLE);	

	SERVO_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			
	SERVO_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	for (servo_id = 0; servo_id < 1; ++servo_id){
		SERVO_GPIO_InitStructure.GPIO_Pin=servo_pwm[servo_id].servo_pin;
		GPIO_Init(servo_pwm[servo_id].GPIOx , &SERVO_GPIO_InitStructure);	
		GPIO_PinAFConfig(servo_pwm[servo_id].GPIOx, servo_pwm[servo_id].GPIO_PinSource, GPIO_AF_TIM1);
	}

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			//0 to FFFF
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Prescaler = 167;												//clk=84M/(83+1)=1 MHz, Freq = 1000000 / 20000 = 50Hz Interval = 20ms
	TIM_TimeBaseStructure.TIM_Period = 20000;												//pulse cycle= 20000 
	
	TIM_TimeBaseInit(SERVO_TIM, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   		// set "high" to be effective output
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;   		// set "high" to be effective output
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	           		// produce output when counter < CCR
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;		// Reset OC Idle state
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	// Reset OC NIdle state
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  	// this part enable the output
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable; // this part disable the Nstate

	TIM_OCInitStructure.TIM_Pulse = 1000;														// this part sets the initial CCR value

	TIM_OC1Init(SERVO_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(SERVO_TIM, ENABLE);
	
	TIM_ARRPreloadConfig(SERVO_TIM, ENABLE);
	TIM_Cmd(SERVO_TIM, ENABLE);	
	TIM_CtrlPWMOutputs(SERVO_TIM, ENABLE);
}

/**
  * @brief  Controlling the PWM for servos
  * @param  servo_id: Port of Motor to be used (SERVO1, SERVO2, SERVO3, SERVO4)
  * @param  val: Any value from 0~20000. Safeguard elsewhere.
  * @retval None
  */
void servo_control(SERVO_ID servo_id , u16 ccr_val) {
	
  if (((u8) servo_id) < SERVO_COUNT) {
    servo_pwm[servo_id].TIM_SetCompare(SERVO_TIM, ccr_val);
  }
  
}
