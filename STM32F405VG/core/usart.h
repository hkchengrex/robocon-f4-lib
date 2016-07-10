#ifndef __USART_H
#define __USART_H

#include "stm32f4xx_usart.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

typedef enum 
{
	COM_NULL = -1,		//disabled
  COM1 = 0,   		//USART1
	COM2 = 1,   		//USART2
  COM3 = 2,				//USART3
	COM4 = 3,				//UART4
	COM5 = 4,				//UART5
} COM_TypeDef;

#define COM_COUNT 5

// Definition for USART1 
#define COM1_CLK                    RCC_APB2Periph_USART1
#define COM1_TX_PIN                 GPIO_Pin_9
#define COM1_TX_GPIO_PORT           GPIOA
#define COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define COM1_RX_PIN                 GPIO_Pin_10
#define COM1_RX_GPIO_PORT           GPIOA
#define COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define COM1_IRQn                   USART1_IRQn

// Definition for USART2
#define COM2_CLK                    RCC_APB1Periph_USART2
#define COM2_TX_PIN                 GPIO_Pin_2
#define COM2_TX_GPIO_PORT           GPIOA
#define COM2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define COM2_RX_PIN                 GPIO_Pin_3
#define COM2_RX_GPIO_PORT           GPIOA
#define COM2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define COM2_IRQn                   USART2_IRQn

// Definition for USART3 
#define COM3_CLK                    RCC_APB1Periph_USART3
#define COM3_TX_PIN                 GPIO_Pin_10
#define COM3_TX_GPIO_PORT           GPIOB
#define COM3_TX_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define COM3_RX_PIN                 GPIO_Pin_11
#define COM3_RX_GPIO_PORT           GPIOB
#define COM3_RX_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define COM3_IRQn                   USART3_IRQn

extern USART_TypeDef* COM_USART[COM_COUNT];
extern COM_TypeDef printf_COMx;

/**
  * @brief  Inintialization of USART
  * @param  COM: which USART to inialialize
  * @param  br: Baudrate used for USART
  * @retval None
  */
void uart_init(COM_TypeDef COM, u32 br);

typedef void on_receive_listener(const uint8_t byte);
/**
  * @brief  Register an on receive listener for UART
  * @param  COM: which USART to use
	* @param  listener: The function used as listener, with void return type and a single u8 parameter
  */
void uart_interrupt_init(COM_TypeDef COM, on_receive_listener *listener);

/**
  * @brief  Enable the function of sending data of Printf via USART
  * @param  COM: which USART to be used for Printf
  */
void uart_printf_enable(COM_TypeDef COM);

/**
  * @brief  Disable the function of sending data of Printf via UART
  */
void uart_printf_disable(void);

/**
  * @brief  Sending one byte of data via USART
  * @param  COM: which USART to be used for sending data
  * @param  data: one byte data to be sent
  * @retval None
  */
void uart_tx_byte(COM_TypeDef COM, const uint8_t data);

/**
  * @brief  Sending multiple bytes of data via USART
  * @param  COM: which USART to be used for sending data
  * @param  tx_buf: string to be sent
  * @retval None
  */
void uart_tx(COM_TypeDef COM, const uint8_t * tx_buf, ...);

/**
  * @brief  Hang the program until one byte of data is received via USART
  * @param  COM: which USART to be used for receiving data
  * @retval One byte of data received
  */
uint8_t uart_rx_byte(COM_TypeDef COM);

#endif		/* __UART_H */
