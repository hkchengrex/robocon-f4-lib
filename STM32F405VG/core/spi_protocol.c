//Placeholder for SPI init for motor boards and controller
//Need to convert to STM32F4 code before use

#include "spi.h"

//F103 test assignment
//SPI1_NSS - PA4 (ADC4)
//SPI1_SCK - PA5 (ADC5)
//SPI1_MISO - PA6 (ADC6)
//SPI1_MOSI - PA7 (ADC7)

u16 receive_count = 0;

void spi_init(SPI_MODE_FLAG mode) {
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel= SPI1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Init clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	//Init GPIO: SCK, MOSI
	//AF Output if master, Floating Input if slave
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = (mode == SPI_MASTER ? GPIO_Mode_AF_PP : GPIO_Mode_IN_FLOATING);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//Init GPIO: MISO
	//AF Output if slave, Floating Input if master
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = (mode == SPI_SLAVE ? GPIO_Mode_AF_PP : GPIO_Mode_IN_FLOATING);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = (mode == SPI_MASTER ? SPI_Mode_Master : SPI_Mode_Slave);
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);
  SPI_CalculateCRC(SPI1, DISABLE);		// Disable the CRC checking
  SPI_SSOutputCmd(SPI1, DISABLE);
}

void spi_tx_byte(uc8 data) {
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
}

//For testing only
u16 spi_return_count() {
	return receive_count;
}

void SPI1_IRQHandler(void) {
	u8 data;
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET) {
		data = (u8)SPI_I2S_ReceiveData(SPI1);
		if (data == 'A')
			receive_count++;
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
	}
}
