#include "uart.h"

uint8_t rx_buffer[256] = {0};
uint8_t rx_full = 0;
on_receive_listener *uart_rx_listener[COM_COUNT];
uint8_t uart_listener_empty[COM_COUNT] = {1};

/** Init a UART port.
*		@param COM: Which port to initialize
*		@param baud_rate: The baud rate to be used.
*/
void uart_init(SerialPort COM, u32 br){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);
	if (COM == COM1){
		RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);//Connect PA9 to USART1_Tx
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);//Connect PA10 to USART1_Rx
	}else if(COM == COM2){
		RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);//Connect PA2 to USART2_Tx
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);//Connect PA3 to USART2_Rx
	}else{
		RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);//Connect PB10 to USART3_Tx
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);//Connect PB11 to USART3_Rx
	}
	
	GPIO_PinAFConfig(COM_TX_PORT[COM], GPIO_PinSource10, GPIO_AF_USART3);//Connect PB10 to USART3_Tx
	GPIO_PinAFConfig(COM_TX_PORT[COM], GPIO_PinSource11, GPIO_AF_USART3);//Connect PB11 to USART3_Rx

	/* Configure USART Tx & USART Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM]|COM_RX_PIN[COM];
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(COM_TX_PORT[COM] , &GPIO_InitStructure);
	
	/* USART configuration */
	USART_InitStructure.USART_BaudRate = br;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(COM_USART[COM], &USART_InitStructure);
	USART_Cmd(COM_USART[COM], ENABLE);
}

//Enable interrupt for specific UART
void uart_interrupt(SerialPort COM){
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = COM_IRQ[COM];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enables the USART receive interrupt */
	USART_ITConfig(COM_USART[COM],USART_IT_RXNE,ENABLE);
}

/** Register a listener for UART receive interrupt.
*		@param COM: Which port to use
*		@param listener: A function pointer of void return type and single u8 param
*/
void uart_interrupt_init(SerialPort COM, on_receive_listener *listener){
	uart_rx_listener[COM] = listener;
	uart_listener_empty[COM] = 0;
	uart_interrupt(COM);
}

/** Send a single byte to the target port.
*		@param COM: Which port to use
*		@param data: The content to be sent
*/
void uart_tx_byte(SerialPort COM, uc8 data){
	while (USART_GetFlagStatus(COM_USART[COM], USART_FLAG_TC) == RESET); 
	USART_SendData(COM_USART[COM], (uint16_t)data);
}

/** Send multiple bytes to the target port.
*		@param COM: Which port to use
*		@param data: The content to be sent
*/
void uart_tx_printf(SerialPort COM, const uc8 * tx_buf, ...){
	va_list arglist;
	u8 buf[255], *fp;
	
	va_start(arglist, tx_buf);
	vsprintf((char*)buf, (const char*)tx_buf, arglist);
	va_end(arglist);
	
	fp = buf;
	while (*fp)
		uart_tx_byte(COM, (uint16_t)*fp++);
}

/** Block the program until received one byte.
*		@param COM: Which port to use
*		@return One btye of data contained.
*/
u8 uart_rx_byte(SerialPort COM){
	while (USART_GetFlagStatus(COM_USART[COM], USART_FLAG_TC) == RESET); 
	return (u8)USART_ReceiveData(COM_USART[COM]);
}
	
void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ // check RX interrupt
		if (!uart_listener_empty[COM1]){
			(*uart_rx_listener[COM1])(USART_ReceiveData(USART1));
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ // check RX interrupt
		if (!uart_listener_empty[COM2]){
			(*uart_rx_listener[COM2])(USART_ReceiveData(USART2));
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ // check RX interrupt
		if (!uart_listener_empty[COM3]){
			(*uart_rx_listener[COM3])(USART_ReceiveData(USART3));
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

void UART4_IRQHandler(void){
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{ // check RX interrupt
		if (!uart_listener_empty[COM4]){
			(*uart_rx_listener[COM4])(USART_ReceiveData(UART4));
		}
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}

void UART5_IRQHandler(void){
	if(USART_GetITStatus(UART5,USART_IT_RXNE) != RESET)
	{ // check RX interrupt
		if (!uart_listener_empty[COM5]){
			(*uart_rx_listener[COM5])(USART_ReceiveData(UART5));
		}
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
	}
}

