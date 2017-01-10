#include "comm.h"

static void comm_receiver(const u8 data);

/** Initiate the communication
* @param COM: Which serial port to use
* @param baud_rate: The baud rate
*/
void comm_init(SerialPort COM, u32 baud_rate){
	uart_init(COM, baud_rate);
	uart_interrupt(COM, data);
}

//The UART receive handler
void comm_receiver(const u8 data){
	
}