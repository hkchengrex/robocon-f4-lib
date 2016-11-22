#include "usart_dma_tx.h"
#include "usart.h"

void uart_dma_init(COM_TypeDef COM, u8* addr){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_InitTypeDef	DMA_InitStructure;
	switch (COM){
		case COM1:
			DMA_DeInit(GET_COM_TX_DMA(COM1));
			DMA_InitStructure.DMA_Channel = GET_COM_TX_DMA_CHANNEL(COM1);
		break;
		
		case COM2:
			DMA_DeInit(GET_COM_TX_DMA(COM2));
			DMA_InitStructure.DMA_Channel = GET_COM_TX_DMA_CHANNEL(COM2);
		break;
		
		case COM3:
			DMA_DeInit(GET_COM_TX_DMA(COM3));
			DMA_InitStructure.DMA_Channel = GET_COM_TX_DMA_CHANNEL(COM3);
		break;
		
		case COM4:
			DMA_DeInit(GET_COM_TX_DMA(COM4));
			DMA_InitStructure.DMA_Channel = GET_COM_TX_DMA_CHANNEL(COM4);
		break;
		
		case COM5:
			DMA_DeInit(GET_COM_TX_DMA(COM5));
			DMA_InitStructure.DMA_Channel = GET_COM_TX_DMA_CHANNEL(COM5);
		break;
		
		default:
			//Throw error
			break;
	}
  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) addr;
	DMA_InitStructure.DMA_BufferSize = (uint16_t) sizeof(addr);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
}
