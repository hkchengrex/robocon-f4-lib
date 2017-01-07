#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "motor.h"

/*** TX ***/
#define SPI_MOTOR_VEL_CMD							0xAA
#define SPI_MOTOR_POS_CMD 						0xBB
#define SPI_MOTOR_ACCEL_CMD						0x44
#define SPI_MOTOR_LOCK_CMD						0xEE	

void spi_motor_init(void);
void spi_tx_byte(uc8 data);
void spi_reset_motor_pins(void);
void spi_select_motor(MOTOR_ID motor_id);
void spi_motor_set_vel(MOTOR_ID motor_id, s32 vel, CLOSE_LOOP_FLAG close_loop_flag);

#endif
