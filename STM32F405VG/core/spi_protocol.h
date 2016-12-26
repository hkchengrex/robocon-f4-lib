#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "gpio.h"
#include "misc.h"

const GPIO* MOTOR_SS_PINS[4] = {&PG5, &PG6, &PG7, &PG8};

typedef enum {
	MOTOR1 = 0,
	MOTOR2,
	MOTOR3,
	MOTOR4,
	MOTOR5,
	MOTOR6,
	MOTOR7,
	MOTOR8,
	MOTOR9,
	MOTOR10,
	MOTOR11,
	MOTOR12,
	MOTOR13,
	MOTOR14,
	MOTOR15,
	MOTOR16
} MOTOR_ID;

void spi_motor_init(void);
void spi_tx_byte(uc8 data);
void spi_reset_motor_pins(void);
void spi_select_motor(u8 motor_id);

#endif
