#ifndef __CAN_MOTOR_H
#define __CAN_MOTOR_H

#include "can_protocol.h"
#include <stdbool.h>

#define MOTOR_CAN CAN_1

#define CAN_MOTOR_COUNT								16
#define	CAN_MOTOR_BASE								0x0B0

/*** TX ***/
#define	CAN_MOTOR_VEL_LENGTH					6
#define CAN_MOTOR_VEL_CMD							0xAA

#define CAN_MOTOR_POS_LENGTH 					7
#define CAN_MOTOR_POS_CMD 						0xBB

#define CAN_MOTOR_PARAMETER_LENGTH		3
#define CAN_MOTOR_PARAMETER_CMD				0x44

#define CAN_MOTOR_LOCK_LENGTH	 		  	1
#define CAN_MOTOR_LOCK_CMD						0xEE	

/*** RX ***/
#define CAN_ENCODER_FEEDBACK_LENGTH		5
#define CAN_ENCODER_FEEDBACK					0x22


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

/*** TX ***/

//Motor (through CAN protocol) initialization 
void can_motor_init(void);

/**
	* @brief Set motor velocity (CAN)
	* @param motor_id (MOTOR_ID enum)
	* @param velocity. Open loop (-1799~1799); Close loop (-150~150);
	* @param close_loop: true if close loop control is used
	*/
void motor_set_vel(MOTOR_ID motor_id, s32 vel, bool close_loop);

/**
	* @brief Set motor acceleration (CAN)
	* @param motor_id (MOTOR_ID enum)
	* @param accel: acceleration parameter of motor
	*/
void motor_set_acceleration(MOTOR_ID motor_id, u16 accel);

/**
	* @brief Lock and stop motor immediately (CAN)
	* @param motor_id (MOTOR_ID enum)
	*/
void motor_lock(MOTOR_ID motor_id);



/*** RX ***/

/**
  * @brief Get the motor encoder value (based on CAN rx result)
  * @param motor_id: The can motor ID
  */
s32 get_encoder_value(MOTOR_ID motor_id);

#endif
