#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


void initADC(ADC_HandleTypeDef * ADC_HANDLE, ADC_ChannelConfTypeDef * ADC_CHANNEL_HANDLE, DMA_HandleTypeDef * DMA_HANDLE, uint32_t * ADC_READING);
void initDMA(DMA_HandleTypeDef * DMA_HANDLE, ADC_HandleTypeDef * ADC_HANDLE);
	 
	 
#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

