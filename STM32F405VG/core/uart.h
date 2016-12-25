#ifndef _UART_H
#define _UART_H

/**
* This library provides simple functions for using UART.
*	Note that all send and receive functions are blocking (only return when finished).
*
* Rex Cheng
*/

#include "stm32f4xx_usart.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "gpio.h"

//COM   UART    TX   RX    RCC                    AF              Interrupt    Priority
#define UART_TABLE \
X(COM1, USART1, PA9, PA10, RCC_APB2Periph_USART1, GPIO_AF_USART1, USART1_IRQn) \
X(COM2, USART2, PA2, PA3, RCC_APB1Periph_USART2, GPIO_AF_USART2, USART2_IRQn) \
X(COM3, USART3, PB10, PB11, RCC_APB1Periph_USART3, GPIO_AF_USART3, USART3_IRQn) \
X(COM4, UART4, PA0, PA1, RCC_APB1Periph_UART4, GPIO_AF_UART4, UART4_IRQn) \
X(COM5, UART5, PC12, PD2, RCC_APB1Periph_UART5, GPIO_AF_UART5, UART5_IRQn) \
X(COM6, USART6, PG14, PG9, RCC_APB2Periph_USART6, GPIO_AF_USART6, USART6_IRQn)

#define X(a, b, c, d, e, f, g) a,
typedef enum {
	UART_TABLE
} SerialPort;
#undef X

typedef struct{
	USART_TypeDef* uart;
	const GPIO* tx_port;
	const GPIO* rx_port;
	const u32 rcc;
	const u8 af;
	const u16 irq;
} UARTStruct;

#define X(a, b, c, d, e, f, g) {b, &c, &d, e, f, g},
static const UARTStruct UARTPorts[] = {UART_TABLE};
#undef X

#define COM_COUNT (sizeof(UARTPorts)/sizeof(UARTStruct))

extern USART_TypeDef* COM_USART[COM_COUNT];

/** Init a UART port.
*		@param COM: Which port to initialize
*		@param baud_rate: The baud rate to be used.
*/
void uart_init(SerialPort COM, u32 baud_rate);

typedef void OnRxListener(const uint8_t byte);

/** Register a listener for UART receive interrupt.
*		@param COM: Which port to use
*		@param listener: A function pointer of void return type and single u8 param
*/
void uart_interrupt_init(SerialPort COM, OnRxListener *listener);

/** Send a single byte to the target port.
*		@param COM: Which port to use
*		@param data: The content to be sent
*/
void uart_tx_byte(SerialPort COM, uint8_t data);

/** Send multiple bytes to the target port.
*		@param COM: Which port to use
*		@param data: The content to be sent
*/
void uart_tx_printf(SerialPort COM, const uint8_t * data, ...);

/** Send an array of data to the target port.
*		@param COM: Which port to use
*		@param data: The pointer to the first element
*		@param len: Length of the array (in bytes)
*/
void uart_tx(SerialPort COM, const uint8_t * data, u16 len);

/** Block the program until received one byte.
*		@param COM: Which port to use
*		@return One btye of data contained.
*/
uint8_t uart_rx_byte(SerialPort COM);

#endif		/* __UART_H */
