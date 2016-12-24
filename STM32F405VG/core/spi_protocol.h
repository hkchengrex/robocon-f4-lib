#ifndef SPI_H
#define SPI_H

typedef enum {
	SPI_MASTER,
	SPI_SLAVE
} SPI_MODE_FLAG;

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "misc.h"

void spi_init(SPI_MODE_FLAG mode);
void spi_tx_byte(uc8 data);
u16 spi_return_count(void);

#endif
