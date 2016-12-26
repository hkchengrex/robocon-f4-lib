//SPI protocol for motor ports
//Peripheral used: SPI2

/*-- Pin assignment --*/
//SPI Pins
//SPI2_NSS: Unused (software mode)
//SPI2_SCK: PB13
//SPI2_MISO: PB14
//SPI2_MOSI: PB15

//SPI NSS GPIO Pins
//Motor_SS_1: PG5
//Motor_SS_2: PG6
//Motor_SS_3: PG7
//Motor_SS_4: PG8
//The above 4 pins are fed into a decoder to pull down one line at a time from 14 lines.

//SPI RX Interrupt GPIO Pins
//Motor_int_1: PG10
//Motor_int_2: PG11
//Motor_int_3: PG12
//Motor_int_4: PG13
//Motor_int_5: PB8
//Motor_int_6: PB9
//MOTOR1-7 lines are fed into an encoder leading to Motor_int_1-3.
//MOTOR8-14 lines are fed into an encoder leading to Motor_int_4-6.
//When multiple motors give an interrupt, precedence is given to motor with higher index.

#include "gpio.h"
#include "spi_protocol.h"

u16 receive_count = 0;

void spi_motor_init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel= SPI2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Init clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	//Init pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_Init(SPI2, &SPI_InitStructure);
  SPI_Cmd(SPI2, ENABLE);
  SPI_CalculateCRC(SPI2, DISABLE);		// Disable the CRC checking
  SPI_SSOutputCmd(SPI2, DISABLE);
	
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
	spi_reset_motor_pins();
}

void spi_tx_byte(uc8 data) {
	SPI_I2S_SendData(SPI2, data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
}

void spi_reset_motor_pins() {
	for (u8 i=0; i<3; i++) GPIO_SetBits(MOTOR_SS_PINS[i]->gpio, MOTOR_SS_PINS[i]->gpio_pin);
}

void spi_select_motor(u8 motor_id) {
	u8 bit = 0x01;
	spi_reset_motor_pins();
	if (motor_id < 0 || motor_id >= 14) return;
	for (u8 i=0; i<3; i++) {
		if (!(motor_id & bit)) GPIO_ResetBits(MOTOR_SS_PINS[i]->gpio, MOTOR_SS_PINS[i]->gpio_pin);
		bit <<= 2;			
	}
}

//void spi_motor_set_vel(MOTOR_ID motor_id, s32 vel, bool close_loop)

void SPI2_IRQHandler(void) {
	//u8 data;
	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET) {
		//data = (u8)SPI_I2S_ReceiveData(SPI2);
		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
	}
}
