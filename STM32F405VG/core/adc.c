#include "adc.h"

static volatile u16 adc_reading[ADC_PORT_COUNT];

void adc_init(){
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStructure;
	
	//RCC init
	for (u8 i=0; i<ADC_COUNT; i++){
		if (IS_RCC_APB1_PERIPH(ADCs[i].rcc)){
			RCC_APB1PeriphClockCmd(ADCs[i].rcc, ENABLE); 
		}else if(IS_RCC_APB2_PERIPH(ADCs[i].rcc)){
			RCC_APB2PeriphClockCmd(ADCs[i].rcc, ENABLE); 
		}
		
		RCC_AHB1PeriphClockCmd(ADCs[i].dma_rcc, ENABLE);  
	}
	
	//GPIO init
	for (u8 i=0; i<ADC_PORT_COUNT; i++){
		gpio_rcc_init(ADCPorts[i].gpio);
		gpio_init(ADCPorts[i].gpio, GPIO_Mode_AN, GPIO_High_Speed, GPIO_OType_OD, GPIO_PuPd_NOPULL);
	}
	
	//ADC common init
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//ADC init
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	DMA_InitTypeDef DMA_InitStructure; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	u16 index = 0;
	for (u8 i=0; i<ADC_COUNT; i++){
		u8 channel_count = 0;
		for (u8 i=0; i<ADC_PORT_COUNT; i++){
			if (ADCPorts[i].adc == ADCs[i].adc){
				channel_count++;
				//Channel init
				ADC_RegularChannelConfig(ADCPorts[i].adc, ADCPorts[i].channel, channel_count, ADC_SampleTime_480Cycles);
			}
		}
		ADC_InitStructure.ADC_NbrOfConversion = channel_count;
		ADC_Init(ADCs[i].adc, &ADC_InitStructure);
		
		DMA_InitStructure.DMA_Channel = ADCs[i].channel;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADCs[i].adc)->DR;
		DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&(adc_reading[index]);
		DMA_InitStructure.DMA_BufferSize = channel_count;
		DMA_Init(ADCs[i].stream, &DMA_InitStructure);
		DMA_Cmd(ADCs[i].stream,ENABLE);
		ADC_DMARequestAfterLastTransferCmd(ADCs[i].adc, ENABLE);
		ADC_DMACmd(ADCs[i].adc, ENABLE);
		ADC_Cmd(ADCs[i].adc, ENABLE);
		ADC_SoftwareStartConv(ADCs[i].adc);
		
		index += channel_count;
	}
}

u16 adc_get(AdcID id){
	return adc_reading[id];
}
