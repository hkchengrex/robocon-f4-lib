#include "gpio.h"
#include "spi_protocol.h"

static SpiListener * SpiRxListenerList[3];
static u8 SpiRxListenerIsEmpty[3] = {1};

static u32 temp = 0;
static u8 tx_in_process = 0;

void spi_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	/*-- Init clocks --*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/*-- Init GPIO Pins --*/
	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//SCK, MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//NSS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//Slave TX interrupt
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*-- Init EXTI for NSS pin --*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	
	/*-- Init NVIC interrupt --*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*-- Init GPIO AF Congifuration --*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	
	/*-- Init SPI3 --*/ 
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_Init(SPI3, &SPI_InitStructure);
  SPI_Cmd(SPI3, ENABLE);
  SPI_CalculateCRC(SPI3, DISABLE);
  SPI_SSOutputCmd(SPI3, DISABLE);
	
	SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void spi_tx_byte_master(SPI_ID id, u8 data) {
	tx_in_process = 1;
	while (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_TXE) == RESET);
	SPI_I2S_SendData(SPI3, data);
	tx_in_process = 0;
}

void spi_rx_set_handler(SPI_ID id, SpiListener * listener) {
	SpiRxListenerList[id] = listener;
}

void EXTI4_IRQHandler(void) {
	u8 data;	
	if (EXTI_GetITStatus(EXTI_Line4) == SET) {
		if (tx_in_process) {
			EXTI_ClearITPendingBit(EXTI_Line4);
			return;
		}
		
		SPI_I2S_SendData(SPI3, 0x00);
		while (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == RESET);
		data = (u8)SPI_I2S_ReceiveData(SPI3);
		(*SpiRxListenerList[2])(data);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
