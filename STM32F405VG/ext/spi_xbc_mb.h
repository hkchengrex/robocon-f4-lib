#ifndef __SPI_XBC_MB_H
#define __SPI_XBC_MB_H

#include "stm32f4xx.h"
#include "xbc_mb.h"
#include "ticks.h"
#include "lcd_main.h"

//Min time between interrupts triggered to count as disconnection
#define SPI_XBC_CONNECTION_TIMEOUT_MS   200

//SPI XBC command list
#define SPI_REPLY		0x50
#define SPI_NO_USB	0x51
#define SPI_GET_SCREEN	0x52

//SPI XBC receiver states
typedef enum {
	SPI_RX_XBC_CMD,	//Incoming command
	SPI_RX_XBC_DATA	//Incoming XBC button data
} SPI_RX_XBC_STATE;

//SPI connection states
typedef enum {
  SPI_XBC_DISCONNECTED,		//XBC board disconnected
  SPI_XBC_USB_DISCONNECTED,	//XBC board connected, but its USB port is disconnected
  SPI_XBC_ALL_CONNECTED		//XBC board and controller connected
} SPI_XBC_CONNECTION_MODE;

typedef struct {
  u16 color, bg_color;
  char text;
} XBC_LCD_DATA;

void spi_xbc_mb_init(void);
SPI_XBC_CONNECTION_MODE spi_xbc_get_connection(void);
u32 spi_xbc_get_digital(void);
s16 spi_xbc_get_joy_raw(XBC_JOY j);
s16 spi_xbc_get_joy(XBC_JOY j);
u16 spi_xbc_get_back_buttons(void);

#endif  /** __SPI_XBC_MB_H **/
