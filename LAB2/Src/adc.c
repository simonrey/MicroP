/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */


void ConfigureDMA();

float currentTemp;
extern float sampleTemp;

ADC_HandleTypeDef ADC1_Handle;
DMA_HandleTypeDef  DMA_Handle;
uint32_t ADC1ConvertedValues[2]; 
IRQn_Type IRQ_Handle;
void configureADC()
{
	printf("config adc start \n");
	HAL_ADC_MspInit(&ADC1_Handle);
	__GPIOC_CLK_ENABLE();
  __ADC1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.Init.DMAContinuousRequests 	= ENABLE;
	ADC1_Handle.Init.ClockPrescaler 				= ADC_CLOCKPRESCALER_PCLK_DIV8;
	ADC1_Handle.Init.Resolution							= ADC_RESOLUTION12b;
	ADC1_Handle.Init.ScanConvMode						= DISABLE;
	ADC1_Handle.Init.ContinuousConvMode			= ENABLE;
	ADC1_Handle.Init.DiscontinuousConvMode	= DISABLE;
	ADC1_Handle.Init.ExternalTrigConvEdge		= ADC_EXTERNALTRIGINJECCONVEDGE_NONE;
	ADC1_Handle.Init.DataAlign							= ADC_DATAALIGN_RIGHT;
	ADC1_Handle.Init.NbrOfConversion				= 1;
	
	HAL_ADC_Init(&ADC1_Handle);
	
	ADC_ChannelConfTypeDef ADC1_CH16Handle;
	
	ADC1_CH16Handle.Channel 			= ADC_CHANNEL_16;
	ADC1_CH16Handle.SamplingTime 	= ADC_SAMPLETIME_480CYCLES;  // Many different options here decide which is best
	ADC1_CH16Handle.Rank 					= 1;
	ADC1_CH16Handle.Offset 				= 0; // this is reserved for future use, doesnt matter
	
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_CH16Handle);
	HAL_ADC_Start(&ADC1_Handle); // do we need this
	
	ConfigureDMA();
  HAL_ADC_Start_DMA(&ADC1_Handle, ADC1ConvertedValues, 2);
}

 
void ConfigureDMA()
{
    __DMA2_CLK_ENABLE(); 
    DMA_Handle.Instance = DMA2_Stream4;
  
    DMA_Handle.Init.Channel 						= DMA_CHANNEL_0;
    DMA_Handle.Init.Direction 					= DMA_PERIPH_TO_MEMORY;
    DMA_Handle.Init.PeriphInc 					= DMA_PINC_DISABLE;
    DMA_Handle.Init.MemInc 							= DMA_MINC_ENABLE;
    DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DMA_Handle.Init.MemDataAlignment 		= DMA_MDATAALIGN_WORD;
    DMA_Handle.Init.Mode 								= DMA_CIRCULAR;
    DMA_Handle.Init.Priority 						= DMA_PRIORITY_HIGH;
    DMA_Handle.Init.FIFOMode 						= DMA_FIFOMODE_DISABLE;         
    DMA_Handle.Init.FIFOThreshold 			= DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Handle.Init.MemBurst 						= DMA_MBURST_SINGLE;
    DMA_Handle.Init.PeriphBurst 				= DMA_PBURST_SINGLE; 
    
    HAL_DMA_Init(&DMA_Handle);
    
    __HAL_LINKDMA(&ADC1_Handle, DMA_Handle, DMA_Handle);
 
    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);   
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	float rawTemp = ADC1ConvertedValues[0]; //(float)getRawTemp();
	float updatedTemp = rawTemp * 3.0;
		
	updatedTemp = updatedTemp / 4095.0;
	updatedTemp = updatedTemp - 0.76;
	updatedTemp = updatedTemp / 0.0025;
	updatedTemp = updatedTemp + 25.0;

	sampleTemp = updatedTemp;
}

void DMA2_Stream4_IRQHandler()
{
		HAL_DMA_IRQHandler(&DMA_Handle);
}

void ADC_IRQHandler()
{
		HAL_ADC_IRQHandler(&ADC1_Handle);
}

	/* USER CODE END 0 */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
