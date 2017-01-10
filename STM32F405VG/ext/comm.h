#ifndef	_COMM_H
#define	_COMM_H

#include "stm32f4xx_tim.h"
#include <stdbool.h>
#include "uart.h"

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

//Length does not count the command itself
#define CMD_TABLE \
X(POS, 10) \
X(MOTOR_VEL, NUMBER_OF_MOTOR*2) \
X(HARDFAULT, 0)

#define X(a, b) a,
typedef enum{
	CMD_TABLE
}CommandCode;
#undef X

#define X(a, b) b, 
static const uint8_t CommandLength[] = {CMD_TABLE};
#undef X

/** Initiate the communication
* @param COM: Which serial port to use
* @param baud_rate: The baud rate
*/
void comm_init(SerialPort COM, u32 baud_rate);

#endif
