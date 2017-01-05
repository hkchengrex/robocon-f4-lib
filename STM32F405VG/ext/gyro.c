/**
  ******************************************************************************
  * @file    gyro.c
  * @brief   Interface for communication with the gyroscope board.
	* @modify  Hong Wing Pang
  * @date    Jul 2016
  *
  ******************************************************************************
	*/
	
//Usage notes

//The gyro board program transmits X/Y coordinates and angle towards the mainboard, and
//this program is for manipulating the above values so that they are usable. It primarily
//performs the functions below.

//1. Rotation correction
//This adds an 2d vector dragging the X/Y value back to the center of the robot, so that
//when it self rotates, the adjusted X/Y value will theoretically remain unchanged.
//This is done using the SHIFT_X and SHIFT_Y values which represents the vector when
//angle == 0d.

//2. Scaling
//Positioning values are easily affected by environment factors. This gives an ultimate
//scaling factor, X_SCALING and Y_SCALING, so that positioning can be tuned easily.

//Note: needs minor update to work with F4 library (Dec 2016)

#include "gyro.h"

s32 SHIFT_X = -6;
s32 SHIFT_Y = 298;

static POSITION gyro_pos = {0, 0, 0};
static u8 rx_state = 0;
static u8 rx_command = 0;
static u8 buf_rec = 0;
static u8 buf_data[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static u8 rx_command_arr[GYRO_COMMAND_LENGTH] = {GYRO_UPDATED, GYRO_REPLY};
static u8 buf_len[GYRO_COMMAND_LENGTH] = {0x06, 0x01};

volatile u8 reply_flag = 0;
volatile u8 gyro_available = 0;


void plus_x(void)
{
	SHIFT_X++;
}
void minus_x(void)
{
	SHIFT_X--;
}
void plus_y(void)
{
	SHIFT_Y++;
}
void minus_y(void)
{
	SHIFT_Y--;
}

s32 gyro_get_shift_x(void)
{
	return SHIFT_X;
}

s32 gyro_get_shift_y(void)
{
	return SHIFT_Y;
}

/**
  * @brief  Initializes the gyro UART connection.
  * @param  None
  * @retval None
  */
void gyro_init(void)
{
	uart_init(GYRO_UART, 115200);
	uart_interrupt_init(GYRO_UART, &gyro_rx_handler);
}

/**
	* @brief  Gets the raw position object.
	* @param  None
	* @retval Raw position object
	*/
const POSITION* get_pos(void)
{
	return &gyro_pos;
}

/**
  * @brief   Calls for gyro to update position information.
  * @details Used when the gyro board program does not sends information by itself.
  * @param   None
  * @retval  None
  */
void gyro_pos_update(void)
{
	uart_tx_byte(GYRO_UART, GYRO_WAKEUP);
	uart_tx_byte(GYRO_UART, GYRO_UPDATE);
	uart_tx_byte(GYRO_UART, 0);
}

/**
  * @brief  Calls for gyro to recalibrate itself.
  * @param  None
  * @retval 1 = successful, 0 = failed
  */
u8 gyro_cal(void)
{
	u16 ticks_last = get_ticks();
	reply_flag &= ~GYRO_FLAG_CAL;
	
	uart_tx_byte(GYRO_UART, GYRO_WAKEUP);
	uart_tx_byte(GYRO_UART, GYRO_CAL);
	uart_tx_byte(GYRO_UART, 0);
	
	while (!(reply_flag & GYRO_FLAG_CAL)) {
		if ((get_ticks()+1000-ticks_last) % 1000 >= 20)			// 20 ms timeout
			return 0;
	}
	return 1;
}

/**
  * @brief  Sets a specified set of coordinates and angle to the gyro.
  * @param  x: X coordinate to be set
  * @param  y: Y coordinate to be set
  * @param  a: angle to be set
  * @retval 1 = successful, 0 = failed
  */
u8 gyro_pos_set(s16 x, s16 y, s16 a)
{
	u16 ticks_last = get_ticks();
	reply_flag &= ~GYRO_FLAG_SET_POS;
	
	uart_tx_byte(GYRO_UART, GYRO_WAKEUP);
	uart_tx_byte(GYRO_UART, GYRO_POS_SET);
	uart_tx_byte(GYRO_UART, 0x06);
	uart_tx_byte(GYRO_UART, x >> 8);
	uart_tx_byte(GYRO_UART, x & 0xFF);
	uart_tx_byte(GYRO_UART, y >> 8);
	uart_tx_byte(GYRO_UART, y & 0xFF);
	uart_tx_byte(GYRO_UART, a >> 8);
	uart_tx_byte(GYRO_UART, a & 0xFF);
	
	u16 timeout = 100;
	while (!(reply_flag & GYRO_FLAG_SET_POS)) {
		if (!(--timeout)) {
			return 0;
		}
	}
	return 1;
}

/**
  * @brief   Interrupt handler for USART3.
  * @param   None
  * @retval  None
  */
void gyro_rx_handler(u8 rx_data)
{
	u8 i;
	u16 x, y, a;
		
	switch (rx_state) {
		case 0:	// wakeup
			if (rx_data == GYRO_WAKEUP) {
				rx_command = 0xFF;
				buf_rec = 0;
				rx_state++;
			}
			break;
		case 1:	// command
			for (i = 0; i < GYRO_COMMAND_LENGTH; i ++) {
				if (rx_data == rx_command_arr[i]) {
					rx_command = i;
					rx_state++;
					break;
				}
			}
			if (rx_command == 0xFF)	// command not in list		
				rx_state = 0;
			break;
		case 2: // confirm command
			if (rx_data != buf_len[rx_command]) {		// wrong data length
				rx_state = 0;
				break;
			}
			rx_state++;
			if (buf_len[rx_command] > 0) {
				break;
			}
		case 3: // receive data
			if (buf_len[rx_command] == 0) {
				rx_state++;
			} else {
				buf_data[buf_rec++] = rx_data;
				if (buf_rec >= buf_len[rx_command]) {
					rx_state++;
				} else {
					break;
				}
			}
		case 4:
			switch (rx_command) {
				case 0:		// GYRO_UPDATED
					x = buf_data[0];
					x <<= 8;
					x |= buf_data[1];
					y = buf_data[2];
					y <<= 8;
					y |= buf_data[3];
					a = buf_data[4];
					a <<= 8;
					a |= buf_data[5];
					
					if (a < 3600) {
						gyro_available = 1;
						
						gyro_pos.x = (s16) x;
						gyro_pos.y = (s16) y;
						gyro_pos.angle = (s16) a;
						
					} else {
						gyro_available = 0;
					}
					break;
				case 1:		// GYRO_REPLY for gyro_cal and gyro_pos_set
					reply_flag |= (1 << buf_data[0]);
					break;
			}
			rx_state = 0;
			break;
	}
}

/**
  * @brief  Gets the X coordinate.
  * @param  None
  * @retval X coordinate
  */
s16 get_X(void)
{
	s16 angle = gyro_pos.angle;
	s32 pos_x = (gyro_pos.x*X_FLIP*10000-SHIFT_X*10000+SHIFT_X*int_cos(angle)+SHIFT_Y*int_sin(angle))/10000;
	return (s16)(pos_x * X_SCALING);
}

/**
  * @brief  Gets the Y coordinate.
  * @param  None
  * @retval Y coordinate
  */
s16 get_Y(void)
{
	s16 angle = gyro_pos.angle;
	s32 pos_y = (gyro_pos.y*Y_FLIP*10000-SHIFT_Y*10000+SHIFT_Y*int_cos(angle)-SHIFT_X*int_sin(angle))/10000;
	return (s16)(pos_y * Y_SCALING);	
}

/**
  * @brief  Gets the angle.
  * @param  None
  * @retval Angle (0-3599)
  */
u16 get_angle(void)
{
	return gyro_pos.angle;
}
