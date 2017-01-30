/*
To Do list for later on
//TX Display
//TX Control
//Listener functions
*/

#include "spi_xbc_mb.h"

const GPIO * xbc_nss = &PC13;
const GPIO * xbc_tx_it = &PB4;

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

static u8 tx_in_process = 0;

void spi_xbc_mb_init(void) {	
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	spi_init(SPI_XBC_PORT);
	
	gpio_rcc_init(xbc_nss);
	gpio_rcc_init(xbc_tx_it);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	gpio_output_init(xbc_nss, GPIO_OType_PP, GPIO_PuPd_NOPULL);
	gpio_input_init(xbc_tx_it, GPIO_PuPd_DOWN);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	
	spi_rx_set_handler(SPI_XBC_PORT, &spi_xbc_rx_handler);
	gpio_write(xbc_nss, Bit_RESET);
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

void spi_xbc_mb_lcd_tx(void) {
	static u8 x = 0;
	spi_tx_byte_master(SPI_XBC_PORT, x++);
}

void spi_xbc_rx_handler(uc8 data) {
	if (get_ticks() - last_spi_connection > SPI_XBC_CONNECTION_TIMEOUT_MS) {
		spi_state = SPI_RX_XBC_CMD;
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
			last_spi_connection = get_ticks();
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
			last_spi_connection = get_ticks();
			break;
		default:
			break;
	}
}

void EXTI4_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line4) == SET) {
		if (tx_in_process) {
			EXTI_ClearITPendingBit(EXTI_Line4);
			return;
		}
		
		SPI_I2S_SendData(SPI_XBC_PORT, 0x00);
		while (SPI_I2S_GetITStatus(SPI_XBC_PORT, SPI_I2S_IT_RXNE) == RESET);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
