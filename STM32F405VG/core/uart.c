#include "uart.h"

/**
* This library provides simple functions for using UART.
*	Note that all send and receive functions are blocking (only return when finished).
*
* Rex Cheng
*/

static OnRxListener *rxListener[COM_COUNT] = {0};

/** Init a UART port.
*		@param COM: Which port to initialize
*		@param baud_rate: The baud rate to be used.
*/
void uart_init(SerialPort COM, u32 br){
	USART_InitTypeDef USART_InitStructure;
	
	const UARTStruct* uart = &UARTPorts[COM];
	
	gpio_rcc_init(uart->tx_port);
	gpio_rcc_init(uart->rx_port);
	
	if (IS_RCC_APB1_PERIPH(uart->rcc)){
		RCC_APB1PeriphClockCmd(uart->rcc, ENABLE);
		
	}else if(IS_RCC_APB2_PERIPH(uart->rcc)){
		RCC_APB2PeriphClockCmd(uart->rcc, ENABLE);
		
	}else{
		//Error
		while(1);
	}
	
	gpio_af_init(uart->tx_port, GPIO_OType_PP, GPIO_PuPd_UP, uart->af);
	gpio_af_init(uart->rx_port, GPIO_OType_PP, GPIO_PuPd_UP, uart->af);
	
	/* USART configuration */
	USART_InitStructure.USART_BaudRate = br;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UARTPorts[COM].uart, &USART_InitStructure);
	USART_Cmd(UARTPorts[COM].uart, ENABLE);
}

//Enable interrupt for specific UART
void uart_interrupt(SerialPort COM){
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = UARTPorts[COM].irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enables the USART receive interrupt */
	USART_ITConfig(UARTPorts[COM].uart,USART_IT_RXNE,ENABLE);
}

/** Register a listener for UART receive interrupt.
*		@param COM: Which port to use
*		@param listener: A function pointer of void return type and single u8 param
*/
void uart_interrupt_init(SerialPort COM, OnRxListener *listener){
	rxListener[COM] = listener;
	uart_interrupt(COM);
}

/** Send a single byte to the target port.
*		@param COM: Which port to use
*		@param data: The content to be sent
*/
void uart_tx_byte(SerialPort COM, uint8_t data){
	while (USART_GetFlagStatus(UARTPorts[COM].uart, USART_FLAG_TC) == RESET); 
	USART_SendData(UARTPorts[COM].uart, (uint16_t)data);
}

/** Send multiple bytes to the target port.
*		@param COM: Which port to use
*		@param data: The content to be sent
*/
void uart_tx_printf(SerialPort COM, const uint8_t * data, ...){
	va_list arglist;
	u8 buf[255], *fp;
	
	va_start(arglist, data);
	vsprintf((char*)buf, (const char*)data, arglist);
	va_end(arglist);
	
	fp = buf;
	while (*fp){
		uart_tx_byte(COM, *fp++);
	}
}

/** Send an array of data to the target port.
*		@param COM: Which port to use
*		@param data: The pointer to the first element
*		@param len: Length of the array (in bytes)
*/
void uart_tx(SerialPort COM, const uint8_t * data, u16 len){
	while(len--){
		uart_tx_byte(COM, *data);
		data++;
	}
}

/** Block the program until received one byte.
*		@param COM: Which port to use
*		@return One btye of data contained.
*/
u8 uart_rx_byte(SerialPort COM){
	while (USART_GetFlagStatus(UARTPorts[COM].uart, USART_FLAG_TC) == RESET); 
	return (u8)USART_ReceiveData(UARTPorts[COM].uart);
}

//Implementing all those IRQ handlers here

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		if (rxListener[COM1]){
			(*rxListener[COM1])(USART_ReceiveData(USART1));
		}
	}
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		if (rxListener[COM2]){
			(*rxListener[COM2])(USART_ReceiveData(USART2));
		}
	}
}

void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		if (rxListener[COM3]){
			(*rxListener[COM3])(USART_ReceiveData(USART3));
		}
	}
}

void UART4_IRQHandler(void){
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		if (rxListener[COM4]){
			(*rxListener[COM4])(USART_ReceiveData(UART4));
		}
	}
}

void UART5_IRQHandler(void){
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		if (rxListener[COM5]){
			(*rxListener[COM5])(USART_ReceiveData(UART5));
		}
	}
}

void USART6_IRQHandler(void){
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		if (rxListener[COM6]){
			(*rxListener[COM6])(USART_ReceiveData(USART6));
		}
	}
}
