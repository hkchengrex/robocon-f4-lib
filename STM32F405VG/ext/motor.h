#ifndef _MOTOR_H
#define _MOTOR_H

#include <stm32f4xx.h>
#include <stdbool.h>

#define USING_CAN_MOTOR
//#define USING_SPI_MOTOR

#ifdef USING_CAN_MOTOR
	#ifdef USING_SPI_MOTOR
		#error Both CAN and SPI Motor enabled!
	#endif
#else
	#ifndef USING_SPI_MOTOR
		#error Please enable either one of CAN or SPI Motor!
	#endif
#endif

typedef enum {
	MOTOR_1 = 0,
	MOTOR_2,
	MOTOR_3,
	MOTOR_4,
	MOTOR_5,
	MOTOR_6,
	MOTOR_7,
	MOTOR_8,
	MOTOR_9,
	MOTOR_10,
	MOTOR_11,
	MOTOR_12,
	MOTOR_13,
	MOTOR_14,
	MOTOR_15,
	MOTOR_16
} MotorID;

typedef enum{
	OPEN_LOOP = 0,
	CLOSE_LOOP = 1
} CloseLoopFlag;

#define get_motor_id(motor_id)	(CAN_MOTOR_BASE + (u8)motor_id)

/**********
* TX Side
***********/

void motor_init(void);
void motor_set_vel(MotorID id, s16 vel, CloseLoopFlag loop);
void motor_set_pos(MotorID id, u16 vel, s32 pos);
void motor_set_accel(MotorID id, u16 accel);
void motor_lock(MotorID id);
s32 get_encoder_value(MotorID id);

//Init CAN/SPI Motor
#ifdef USING_CAN_MOTOR
	#define motor_init() can_motor_init()
#else
	#define motor_init() spi_motor_init()
#endif

/**
* @brief Set motor velocity (CAN/SPI)
* @param motor_id: MOTORx, which motor to control
* @param vel: Open loop: (-1799~1799); Close loop: (-150~150);
* @param loop: Open loop or close loop control
*/
#ifdef USING_CAN_MOTOR
	#define motor_set_vel(id, vel, loop) can_motor_set_vel(id, vel, loop)
#else
	#define motor_set_vel(id, vel, loop) spi_motor_set_vel(id, vel, loop)
#endif

/**
* @brief Set motor position (CAN/SPI)
* @param motor_id: MOTORx, which motor to control
* @param vel (vel of close_loop is not corresponded to open_loop)
* @param pos: The position need to move to relative to current encoder value.
*/
#ifdef USING_CAN_MOTOR
	#define motor_set_pos(id, vel, pos) can_motor_set_pos(id, vel, pos)
#else
	#define motor_set_pos(id, vel, pos) spi_motor_set_pos(id, vel, pos)
#endif

/**
* @brief Set motor acceleration (CAN/SPI)
* @param motor_id: MOTORx, which motor to control
* @param accel: acceleration parameter of motor
*/
#ifdef USING_CAN_MOTOR
	#define motor_set_accel(id, accel) can_motor_set_accel(id, accel)
#else
	#define motor_set_accel(id, accel) spi_motor_set_accel(id, accel)
#endif

/**
* @brief Lock and stop motor immediately (CAN/SPI)
* @param motor_id: MOTORx, which motor to control
*/
#ifdef USING_CAN_MOTOR
	#define motor_lock(id) can_motor_lock(id)
#else
	#define motor_lock(id) spi_motor_lock(id)
#endif



/**********
* RX Side
***********/


/**
* @brief Get the motor encoder value (based on CAN/SPI rx result)
* @param motor_id: MOTORx, which motor to control
*/
#ifdef USING_CAN_MOTOR
	#define get_encoder_value(id) can_get_encoder_value(id)
#else
	#define get_encoder_value(id) spi_get_encoder_value(id)
#endif

#endif
