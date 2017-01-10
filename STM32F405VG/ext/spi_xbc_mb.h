#ifndef __SPI_XBC_MB_H
#define __SPI_XBC_MB_H

#include "stm32f4xx.h"
#include "xbc_mb.h"

typedef enum {
	SPI_RX_XBC_IDLE,
	SPI_RX_XBC_CMD,
	SPI_RX_XBC_DATA
} SPI_RX_XBC_STATE;

void spi_xbc_mb_init(void);
u32 spi_xbc_get_digital(void);
s16 spi_xbc_get_joy_raw(XBC_JOY j);
s16 spi_xbc_get_joy(XBC_JOY j);
u16 spi_xbc_get_back_buttons(void);

#endif  /** __SPI_XBC_MB_H **/
