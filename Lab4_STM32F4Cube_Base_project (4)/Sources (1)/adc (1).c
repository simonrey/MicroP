#include "adc.h"
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection



void initADC(ADC_HandleTypeDef * ADC_HANDLE, ADC_ChannelConfTypeDef * ADC_CHANNEL_HANDLE, DMA_HandleTypeDef * DMA_HANDLE, uint32_t * ADC_READING){
	__GPIOC_CLK_ENABLE();
	__ADC1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();
		
	HAL_ADC_MspInit(ADC_HANDLE);

	ADC_HANDLE->Instance = ADC1;
	ADC_HANDLE->Init.DMAContinuousRequests = ENABLE;
	ADC_HANDLE->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
	ADC_HANDLE->Init.Resolution = ADC_RESOLUTION12b;
	ADC_HANDLE->Init.ScanConvMode = DISABLE;
	ADC_HANDLE->Init.ContinuousConvMode = ENABLE;
	ADC_HANDLE->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_HANDLE->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_HANDLE->Init.NbrOfConversion = 1;
	
	HAL_ADC_Init(ADC_HANDLE);
	
	ADC_CHANNEL_HANDLE->Channel = ADC_CHANNEL_16;
	ADC_CHANNEL_HANDLE->SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC_CHANNEL_HANDLE->Rank = 1;
	ADC_CHANNEL_HANDLE->Offset = 0;
	
	HAL_ADC_ConfigChannel(ADC_HANDLE, ADC_CHANNEL_HANDLE);
	HAL_ADC_Start(ADC_HANDLE);
	
	initDMA(DMA_HANDLE, ADC_HANDLE);
	HAL_ADC_Start_DMA(ADC_HANDLE, ADC_READING, 1);
	
}

void initDMA(DMA_HandleTypeDef * DMA_HANDLE, ADC_HandleTypeDef * ADC_HANDLE){
	__DMA2_CLK_ENABLE();
	DMA_HANDLE->Instance = DMA2_Stream4;
	
	DMA_HANDLE->Init.Channel = DMA_CHANNEL_0;
	DMA_HANDLE->Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_HANDLE->Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_HANDLE->Init.MemInc = DMA_MINC_ENABLE;
	DMA_HANDLE->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	DMA_HANDLE->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	DMA_HANDLE->Init.Mode = DMA_CIRCULAR;
	DMA_HANDLE->Init.Priority = DMA_PRIORITY_HIGH;
	DMA_HANDLE->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	DMA_HANDLE->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	DMA_HANDLE->Init.MemBurst = DMA_MBURST_SINGLE;
	DMA_HANDLE->Init.PeriphBurst = DMA_PBURST_SINGLE;
	
	HAL_DMA_Init(DMA_HANDLE);
	ADC_HANDLE->DMA_Handle = DMA_HANDLE;
	//__HAL_LINKDMA(ADC_HANDLE, DMA_HANDLE, DMA_HANDLE);
	
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
	
}
