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

extern ADC_HandleTypeDef ADC1_Handle;

void configureADC()
{
	printf("config adc start \n");
	HAL_ADC_MspInit(&ADC1_Handle);
	//int ADCclkEnableErrorCode = __HAL_RCC_ADC_CLK_ENABLE(); 
	//printf("ADC CLK enabled with error code: %i \n", ADCclkEnableErrorCode);
	configureGPIO();
	
	
	
	
	
	__GPIOC_CLK_ENABLE();
  __ADC1_CLK_ENABLE();

	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.Init.ClockPrescaler 				= ADC_CLOCKPRESCALER_PCLK_DIV4;
	ADC1_Handle.Init.Resolution							= ADC_RESOLUTION12b;
	ADC1_Handle.Init.ScanConvMode						= DISABLE;
	ADC1_Handle.Init.ContinuousConvMode			= ENABLE;
	ADC1_Handle.Init.DiscontinuousConvMode	= DISABLE;
	ADC1_Handle.Init.ExternalTrigConvEdge		= ADC_EXTERNALTRIGINJECCONVEDGE_NONE;
	ADC1_Handle.Init.DataAlign							= ADC_DATAALIGN_RIGHT;
	
	ADC1_Handle.Init.NbrOfConversion				= 1;
	
	HAL_ADC_Init(&ADC1_Handle);
	
	
	
	ADC_ChannelConfTypeDef ADC1_CH16Handle;
	
	ADC1_CH16Handle.Channel = ADC_CHANNEL_16;
	ADC1_CH16Handle.SamplingTime = ADC_SAMPLETIME_480CYCLES;  // Many different options here decide which is best
	ADC1_CH16Handle.Rank = 1;
	ADC1_CH16Handle.Offset = 0; // this is reserved for future use, doesnt matter
	
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_CH16Handle);
	printf("state before start %i \n", HAL_ADC_GetState(&ADC1_Handle));
	HAL_ADC_Start(&ADC1_Handle);
	printf("state after start %i \n", HAL_ADC_GetState(&ADC1_Handle));
}



int getRawTemp() // run configureADC() first, then can run this as much as needed
{
	while(1)
	{
		//printf("adc output in loop %i", HAL_ADC_GetValue(&ADC1_Handle));
		if(HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) == HAL_OK)
		{
			return HAL_ADC_GetValue(&ADC1_Handle);
		}
	}
}

int updateTemp()
{
	int currentTemp = 3000;
	float rawTemp = (float)getRawTemp();
	float updatedTemp = rawTemp * 3.0;
	printf("rawTemp: %f\n",rawTemp);
		
	updatedTemp = updatedTemp / 4095.0;
	updatedTemp = updatedTemp - 0.76;
	updatedTemp = updatedTemp / 0.0025;
	updatedTemp = updatedTemp + 25.0;
	
	currentTemp = (int)(updatedTemp*100.0);
	return currentTemp;
}

int alternateUpdateTemp()
{
	//addresses from pg 137 of "Doc_06 - STM32F407xx Datasheet"
	uint16_t *TS_CAL1 = (uint16_t *)0x1FFF7A2C;
	uint16_t *TS_CAL2 = (uint16_t *)0x1FFF7A2E;
	float rawTemp = (float)getRawTemp();
	
	//temp formula from pg 9, http://www.st.com/content/ccc/resource/technical/document/application_note/b9/21/44/4e/cf/6f/46/fa/DM00035957.pdf/files/DM00035957.pdf/jcr:content/translations/en.DM00035957.pdf
	float temp 		= (80.0/(((float)*TS_CAL2) - ((float)*TS_CAL1)))*(rawTemp - ((float)*TS_CAL1)) + 30.0;
	int currentTemp = (int)(temp*100.0);
	return currentTemp;
}
	/* USER CODE END 0 */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
