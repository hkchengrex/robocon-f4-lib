#ifndef _USART_DMA_H
#define _USART_DMA_H

#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "usart.h"

#define COM1_TX_DMA	DMA2_Stream7
#define COM1_TX_DMA_CHANNEL	DMA_Channel_4

#define COM2_TX_DMA	DMA1_Stream6
#define COM2_TX_DMA_CHANNEL	DMA_Channel_4

#define COM3_TX_DMA	DMA1_Stream3
#define COM3_TX_DMA_CHANNEL	DMA_Channel_4

#define COM4_TX_DMA	DMA1_Stream4
#define COM4_TX_DMA_CHANNEL	DMA_Channel_4

#define COM5_TX_DMA	DMA1_Stream7
#define COM5_TX_DMA_CHANNEL	DMA_Channel_4

#define GET_COM_TX_DMA(COM) COM ## _TX_DMA
#define GET_COM_TX_DMA_CHANNEL(COM) COM ## _TX_DMA_CHANNEL

#endif
