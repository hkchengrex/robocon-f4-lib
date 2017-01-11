#include "comm.h"

static SerialPort COMPort;

static void comm_receiver(const u8 data);

/** Initiate the communication
* @param COM: Which serial port to use
* @param baud_rate: The baud rate
*/
void comm_init(SerialPort COM, u32 baud_rate){
	uart_init(COM, baud_rate);
	uart_interrupt_init(COM, comm_receiver);
	
	COMPort = COM;
}

/**
* The followings are for receiving commands from upper-level machine
*/


//Buffer and variable for receiving messages
static bool handling_command = false;
static CommandCode command;
static u8 buffer_index = 0;
static u8 buffer[50];

typedef void(*MessageHandler)(const u8* data);
//Function pointers to functions that resolve the message
static MessageHandler handlers[COMMAND_COUNT] = {0};

//The UART receive handler
static void comm_receiver(const u8 data){
	if (!handling_command){
		//Accept a new command code
		if (data < COMMAND_COUNT){
			command = data;
			handling_command = true;
		}else{
			//Error
		}
		
	}else{
		//Receive the content of the command
		buffer[buffer_index++] = data;
		if (buffer_index >= CommandLength[command]){
			//Resolve message
		}
	}
}




/**
* The followings are for sending feedback to upper-level machine
*/



/** Transmit current position data
*		4 btyes(ticks) + 2*3 bytes (X, Y, Theta) = 10 bytes
*/
void comm_tx_pos(){
	char data[11];
	data[0] = POS_FEEDBACK;
	data[1] = (get_X() >> 8) & 0xFF;
	data[2] = get_X() & 0xFF;
	data[3] = (get_Y() >> 8) & 0xFF;
	data[4] = get_Y() & 0xFF;
	data[5] = (get_angle() >> 8) & 0xFF;
	data[6] = get_angle() & 0xFF;
	data[7] = (get_ticks() >> 24) & 0xFF;
	data[8] = (get_ticks() >> 16) & 0xFF;
	data[9] = (get_ticks() >> 8) & 0xFF;
	data[10] = get_ticks() & 0xFF;
	
	uart_tx_array(COMPort, data, 11);
}

/** Transmit current motor velocity
*		2 btyes for each motor (14 bits for speed, 1 bit for open/close loop, 1 unused bit for possible state)
*/
void comm_tx_motor(){
}

/** Transmit an error indicator as a single byte
*		1 btye data
*/
void comm_tx_error(uint8_t feedback_code){
}

