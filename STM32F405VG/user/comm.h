#ifndef	_COMM_H
#define	_COMM_H

#include "stm32f4xx_tim.h"
#include <stdbool.h>
#include "uart.h"
#include "ticks.h"
#include "gyro.h"
#include "robot_control.h"

/***************************************************************************************************************************************
** COMMUNICATION - STM32F4
**
** This file provides the protocol for communicating with higher-end (usually PC) machine.
** This protocol is to be customized based on actual need.
** 
** ROBOCON 2017
** H K U S T
**
** Author:	Rex Cheng
** Contact:	hkchengad@connect.ust.hk
**
** v1.0 January 2017 > PC side - AEGIS System <
** 
** Is that you that never listened to me, 
** or is that me never talked to you?...
****************************************************************************************************************************************/

#define NUMBER_OF_MOTOR 3


/**
* The followings are for receiving commands from upper-level machine
*/


//Length does not count the command itself
#define CMD_TABLE \
X(HALT_COMMAND = 0			, 0) \
//X(MOTOR_VEL = 1				, NUMBER_OF_MOTOR*2) \
//X(HARD_FAULT = 2			, 0)

#define X(a, b) a,
typedef enum{
	CMD_TABLE
}CommandCode;
#undef X

#define X(a, b) b, 
static const uint8_t CommandLength[] = {CMD_TABLE};
#undef X

#define COMMAND_COUNT (sizeof(CommandLength)/sizeof(uint8_t))


/**
* The followings are for sending feedback to upper-level machine
*/


#define FEEDBACK_TABLE \
X(POS_FEEDBACK) \
X(MOTOR_FEEDBACK) \
X(HARD_FAULT_FEEDBACK)

#define X(a) a,
typedef enum{
	FEEDBACK_TABLE
}FeedbackCode;
#undef X

#define FEEDBACK_COUNT 3

/** Transmit current position data
*		1 btye flag + 4 btyes(ticks) + 2*3 bytes (X, Y, Theta) = 11 bytes
*/
void comm_tx_pos(void);

/** Transmit current motor velocity
*		1 btye flag + 2 btyes for each motor (14 bits for speed, 1 bit for open/close loop, 1 unused bit for possible state)
*/
void comm_tx_motor(void);

/** Transmit an error indicator as a single byte
*		1 btye data
*/
void comm_tx_error(uint8_t feedback_code);

/** Initiate the communication
* @param COM: Which serial port to use
* @param baud_rate: The baud rate
*/
void comm_init(SerialPort COM, u32 baud_rate);

#endif
