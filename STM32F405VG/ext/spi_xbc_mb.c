/*
can_xbc_mb_init(void);
can_xbc_mb_tx_enable(bool flag);
can_xbc_get_connection(void);
can_xbc_get_digital(void);

can_xbc_get_joy_raw(XBC_JOY j);
can_xbc_get_joy(XBC_JOY j);
can_xbc_mb_lcd_tx(void);
//TX Display
//TX Control
//Listener functions
*/

#include "spi_xbc_mb.h"

//For receiving data
static SPI_RX_XBC_STATE spi_state = SPI_RX_XBC_CMD;
static u8 spi_rx_xbc_count = 0;
static u8 spi_rx_xbc_buffer[14] = {0};

//Storing button states
static u32 xbc_digital = 0;
static s16 xbc_joy[XBC_JOY_COUNT] = {0};
static u16 xbc_back_buttons = 0;

//XBC connection states
static u32 last_spi_connection = 0;
static SPI_XBC_CONNECTION_MODE xbc_connection = SPI_XBC_DISCONNECTED;

//XBC TFT buffer
static XBC_LCD_DATA xbc_lcd_data[CHAR_MAX_X_VERTICAL][CHAR_MAX_Y_VERTICAL],
  xbc_lcd_data_prev[CHAR_MAX_X_VERTICAL][CHAR_MAX_Y_VERTICAL];

void spi_xbc_mb_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//Init clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Init pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
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
}

SPI_XBC_CONNECTION_MODE spi_xbc_get_connection(void) {
	if (get_ticks() - last_spi_connection > SPI_XBC_CONNECTION_TIMEOUT_MS) {
    xbc_connection = SPI_XBC_DISCONNECTED;
  }
  return xbc_connection;
}

u32 spi_xbc_get_digital(void) {
  if (spi_xbc_get_connection() != SPI_XBC_ALL_CONNECTED) return 0;
	return xbc_digital;
}

s16 spi_xbc_get_joy_raw(XBC_JOY j) {
  if (spi_xbc_get_connection() != SPI_XBC_ALL_CONNECTED) return 0;
  return xbc_joy[j];
}

s16 spi_xbc_get_joy(XBC_JOY j) {
  if (spi_xbc_get_connection() != SPI_XBC_ALL_CONNECTED) return 0;
  switch (j) {
    case XBC_JOY_LT:
    case XBC_JOY_RT:
      return xbc_joy[j];

    case XBC_JOY_LX:
    case XBC_JOY_LY:
    case XBC_JOY_RX:
    case XBC_JOY_RY:
      if (xbc_joy[j] >= -XBC_JOY_DEADZONE_MIN && xbc_joy[j] <= XBC_JOY_DEADZONE_MIN) {
        return 0;
      } else if (xbc_joy[j] < -XBC_JOY_DEADZONE_MAX) {
        return -XBC_JOY_SCALE;
      } else if (xbc_joy[j] > XBC_JOY_DEADZONE_MAX) {
        return XBC_JOY_SCALE;
      } else {
        if (xbc_joy[j] > 0) {
          return (xbc_joy[j] - XBC_JOY_DEADZONE_MIN) * XBC_JOY_SCALE / (XBC_JOY_DEADZONE_MAX - XBC_JOY_DEADZONE_MIN);
        } else {
          return (xbc_joy[j] - -XBC_JOY_DEADZONE_MIN) * XBC_JOY_SCALE / (XBC_JOY_DEADZONE_MAX - XBC_JOY_DEADZONE_MIN);
        }
      }
  }
  return 0;
}

u16 spi_xbc_get_back_buttons(void) {
  if (spi_xbc_get_connection() != SPI_XBC_ALL_CONNECTED) return 0;
  return xbc_back_buttons;
}

void SPI3_IRQHandler(void) {
	u8 data;
	if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET) {
		data = (u8)SPI_I2S_ReceiveData(SPI3);
		
		if (get_ticks() - last_spi_connection > SPI_XBC_CONNECTION_TIMEOUT_MS) {
			spi_state = SPI_RX_XBC_CMD;
			spi_xbc_mb_init();
		}
		
		switch(spi_state) {
			
			//Receive command
			case SPI_RX_XBC_CMD:
				//Act according to command
				switch (data) {
					case SPI_REPLY:
						spi_rx_xbc_count = 0;
						xbc_connection = SPI_XBC_ALL_CONNECTED;
						spi_state = SPI_RX_XBC_DATA;
						break;
					case SPI_NO_USB:
						xbc_connection = SPI_XBC_USB_DISCONNECTED;
						break;
				}
				break;
			
			//Receive data
			case SPI_RX_XBC_DATA:
				spi_rx_xbc_buffer[spi_rx_xbc_count] = data;
				
				spi_rx_xbc_count++;
				if (spi_rx_xbc_count == 14) {
					xbc_digital = spi_rx_xbc_buffer[0] + (spi_rx_xbc_buffer[1] << 8);
					xbc_joy[XBC_JOY_LT] = spi_rx_xbc_buffer[2];
					xbc_joy[XBC_JOY_RT] = spi_rx_xbc_buffer[3];
					xbc_joy[XBC_JOY_LX] = spi_rx_xbc_buffer[4] + (spi_rx_xbc_buffer[5] << 8); 
					xbc_joy[XBC_JOY_LY] = spi_rx_xbc_buffer[6] + (spi_rx_xbc_buffer[7] << 8);
					xbc_joy[XBC_JOY_RX] = spi_rx_xbc_buffer[8] + (spi_rx_xbc_buffer[9] << 8); 
					xbc_joy[XBC_JOY_RY] = spi_rx_xbc_buffer[10] + (spi_rx_xbc_buffer[11] << 8);
					xbc_back_buttons = spi_rx_xbc_buffer[13];
					
					spi_state = SPI_RX_XBC_CMD;
				}
		}
		
		last_spi_connection = get_ticks();
		SPI_I2S_ClearITPendingBit(SPI3, SPI_I2S_IT_RXNE);
	}
}
