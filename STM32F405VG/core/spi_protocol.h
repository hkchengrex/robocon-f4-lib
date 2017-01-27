#ifndef SPI_PROTOCOL_H
#define SPI_PROTOCOL_H

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "motor.h"

typedef void SpiListener(uc8 byte);

typedef enum {
	SPI_1,
	SPI_2,
	SPI_3
} SPI_ID;

void spi_init(void);
void spi_tx_byte_master(SPI_ID id, u8 data);
void spi_rx_set_handler(SPI_ID id, SpiListener * listener);

#endif
