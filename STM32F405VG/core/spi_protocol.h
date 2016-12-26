#ifndef SPI_H
#define SPI_H

typedef enum {
	SPI_MASTER,
	SPI_SLAVE
} SPI_MODE_FLAG;

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "misc.h"

void spi_motor_init(void);
void spi_tx_byte(uc8 data);

#endif
