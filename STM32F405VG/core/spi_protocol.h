#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "can_motor.h"

/*** TX ***/
#define SPI_MOTOR_VEL_CMD							0xAA
#define SPI_MOTOR_POS_CMD 						0xBB
#define SPI_MOTOR_ACCEL_CMD						0x44
#define SPI_MOTOR_LOCK_CMD						0xEE	

typedef enum {
	OPEN_LOOP = 0,
	CLOSE_LOOP = 1
} CLOSE_LOOP_FLAG;

/*
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
*/

void spi_motor_init(void);
void spi_tx_byte(uc8 data);
void spi_reset_motor_pins(void);
void spi_select_motor(MOTOR_ID motor_id);
void spi_motor_set_vel(MOTOR_ID motor_id, s32 vel, CLOSE_LOOP_FLAG close_loop_flag);

#endif
