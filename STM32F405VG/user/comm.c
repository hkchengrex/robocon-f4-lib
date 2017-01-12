#include "comm.h"

static SerialPort COMPort;
static uint8_t handshake_state = 0;

static void comm_receiver(const uint8_t data);

/** Initiate the communication
* @param COM: Which serial port to use
* @param baud_rate: The baud rate
*/
void comm_init(SerialPort COM, uint32_t baud_rate){
	uart_init(COM, baud_rate);
	uart_interrupt_init(COM, comm_receiver);
	
	COMPort = COM;
	handshake_state = 0;
}

/**
* The followings are for receiving commands from upper-level machine
*/


//Buffer and variable for receiving messages
static bool handling_command = false;
static CommandCode command;
static uint8_t buffer_index = 0;
static uint8_t buffer[50];

typedef void(*MessageHandler)(const uint8_t* data);
//Function pointers to functions that resolve the message
static MessageHandler handlers[COMMAND_COUNT] = {0};

//The UART receive handler
static void comm_receiver(const uint8_t data){
	if (handshake_state <= COMM_HANDSHAKE_DONE_STATE){
		//Continue the handshake
		switch (handshake_state){
			case 0:
				if (data == COMM_FIRST_HANDSHAKE){
					handshake_state = 1;
					uart_tx_byte(COMPort, COMM_SECOND_HANDSHAKE);
				}else{
					handshake_state = 0;
				}
				break;
			case 1:
				if (data == COMM_THIRD_HANDSHAKE){
					handshake_state = 2;
				}else{
					handshake_state = 0;
				}
				break;
		}
		
	}else if (!handling_command){
		//Accept a new command code
		if (data < COMMAND_COUNT){
			command = (CommandCode)data;
			handling_command = true;
		}else{
			//Error
		}
		
	}else{
		//Receive the content of the command
		buffer[buffer_index++] = data;
		if (buffer_index >= CommandLength[command]){
			handlers[command](buffer);
			buffer_index = 0;
		}
	}
}




/**
* The followings are for sending feedback to upper-level machine
*/



/** Transmit current position data
*		1 btye flag + 4 btyes(ticks) + 2*3 bytes (X, Y, Theta) = 11 bytes
*/
void comm_tx_pos(){
	if (handshake_state <= COMM_HANDSHAKE_DONE_STATE) return;
	uint8_t data[11];
	data[0] = POS_FEEDBACK;
//	data[1] = (get_X() >> 8) & 0xFF;
//	data[2] = get_X() & 0xFF;
//	data[3] = (get_Y() >> 8) & 0xFF;
//	data[4] = get_Y() & 0xFF;
//	data[5] = (get_angle() >> 8) & 0xFF;
//	data[6] = get_angle() & 0xFF;
//	data[7] = (get_ticks() >> 24) & 0xFF;
//	data[8] = (get_ticks() >> 16) & 0xFF;
//	data[9] = (get_ticks() >> 8) & 0xFF;
//	data[10] = get_ticks() & 0xFF;
	data[1] = get_X() >> 8;
	data[2] = get_X();
	data[3] = get_Y() >> 8;
	data[4] = get_Y();
	data[5] = get_angle() >> 8;
	data[6] = get_angle();
	data[7] = get_ticks() >> 24;
	data[8] = get_ticks() >> 16;
	data[9] = get_ticks() >> 8;
	data[10] = get_ticks();
	
	uart_tx_array(COMPort, data, 11);
}

/** Transmit current motor velocity
*		1 btye flag + 2 btyes for each motor (14 bits for speed, 1 bit for open/close loop, 1 unused bit for possible state)
*/
void comm_tx_motor(){
	if (handshake_state <= COMM_HANDSHAKE_DONE_STATE) return;
	uint8_t data[1+NUMBER_OF_MOTOR*2];
	data[0] = MOTOR_FEEDBACK;
	for (uint8_t i=0; i<NUMBER_OF_MOTOR; i++){
		data[1+i*2] = ((get_motor_vel(i) >> 8) & 0x3F) | (get_motor_flag(i)<<7);
		data[2+i*2] = get_motor_vel(i);
	}
	
	uart_tx_array(COMPort, data, 1+NUMBER_OF_MOTOR*2);
}

/** Transmit an error indicator as a single byte
*		1 btye data
*/
void comm_tx_error(uint8_t feedback_code){
	if (handshake_state <= COMM_HANDSHAKE_DONE_STATE) return;
}

