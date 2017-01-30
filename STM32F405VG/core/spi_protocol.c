#include "gpio.h"
#include "spi_protocol.h"

static SpiListener * SpiRxListenerList[3];
static u8 SpiRxListenerNotEmpty[3] = {0};

static u8 tx_in_process = 0;

u8 spi_get_id(SPI_TypeDef * port) {
	if (port == SPI1) return 0;
	if (port == SPI2) return 1;	
	if (port == SPI3) return 2;
}

void spi_init(SPI_TypeDef * port) {
	u8 id = spi_get_id(port);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(gpio_rcc[id], ENABLE);
	
	gpio_rcc_init(mosi[id]);
	gpio_rcc_init(miso[id]);
	gpio_rcc_init(sclk[id]);
	
	gpio_af_init(mosi[id], GPIO_OType_PP, GPIO_PuPd_NOPULL, gpio_af[id]);
	gpio_af_init(miso[id], GPIO_OType_PP, GPIO_PuPd_NOPULL, gpio_af[id]);
	gpio_af_init(sclk[id], GPIO_OType_PP, GPIO_PuPd_NOPULL, gpio_af[id]);
	
	NVIC_InitStructure.NVIC_IRQChannel = gpio_irq[id];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_I2S_DeInit(port);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_Init(port, &SPI_InitStructure);
  SPI_Cmd(port, ENABLE);
  SPI_CalculateCRC(port, DISABLE);
  SPI_SSOutputCmd(port, DISABLE);
	
	SPI_I2S_ITConfig(port, SPI_I2S_IT_RXNE, ENABLE);
}

void spi_tx_byte_master(SPI_TypeDef * port, u8 data) {
	tx_in_process = 1;
	while (SPI_I2S_GetITStatus(port, SPI_I2S_IT_TXE) == RESET);
	SPI_I2S_SendData(port, data);
	tx_in_process = 0;
}

void spi_rx_set_handler(SPI_TypeDef * port, SpiListener * listener) {
	u8 id = spi_get_id(port);
	SpiRxListenerList[id] = listener;
	SpiRxListenerNotEmpty[id] = 1;
}

void SPI1_IRQHandler(void) {
	u8 data;
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) {
		data = (u8)SPI_I2S_ReceiveData(SPI1);
		if (SpiRxListenerNotEmpty[0])
			(*SpiRxListenerList[0])(data);
	}
}

void SPI2_IRQHandler(void) {
	u8 data;
	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET) {
		data = (u8)SPI_I2S_ReceiveData(SPI2);
		if (SpiRxListenerNotEmpty[1])
			(*SpiRxListenerList[1])(data);
	}
}

void SPI3_IRQHandler(void) {
	u8 data;
	if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET) {
		data = (u8)SPI_I2S_ReceiveData(SPI3);
		if (SpiRxListenerNotEmpty[2])
			(*SpiRxListenerList[2])(data);
	}
}
