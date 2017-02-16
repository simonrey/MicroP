/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "gpio.h"
#include <stdlib.h>

#define V25 7600
#define AVG_SLOPE 25



ADC_HandleTypeDef adc1;

struct segments{
	int SEG[8];
	int digit;
	int VALUE;
	struct segments * NEXT_SEGMENT;
};


void SystemClock_Config(void);
void GPIO_Setup(void);
void ADC_Setup(void);
int SegmentDecoder(int toDecode, struct segments * segment, int * place);
int TemperatureFormula(int OutADC);
int CtoF(int temperatureC);
int WriteToPin(int isNegative, struct segments * segment, int place);

int setSegmentSelectLineHigh(int digit, struct segments * segment0);
int setDigitSelectLinesHigh(int digit);
int setDigitSelectLinesZero(void);
int setSegmentSelectLinesOne(void);


int main(void)
{


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	GPIO_Setup();
	ADC_Setup();
	struct segments seg0;
	struct segments seg1;
	struct segments seg2;
	struct segments seg3;
	
	seg0.NEXT_SEGMENT = &seg1;
	seg0.digit = 0;
	seg1.NEXT_SEGMENT = &seg2;
	seg1.digit = 1;
	seg2.NEXT_SEGMENT = &seg3;
	seg2.digit = 2;
	seg3.NEXT_SEGMENT = &seg0;
	seg3.digit = 3;
	
  
  while (1)
  {

		
		int temperatureC = HAL_ADC_GetValue(&adc1);
		int digitCount = 0;
		int isNegative;
		temperatureC = TemperatureFormula(temperatureC);
		
		//If user input is 1 - Perform C
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
			isNegative = SegmentDecoder(temperatureC, &seg0, &digitCount);
		}
		//If user input is 0 - Perform F
		else{
			isNegative = SegmentDecoder(CtoF(temperatureC), &seg0, &digitCount);
		}
		WriteToPin(isNegative, &seg0, digitCount);
		digitCount = 0;
		
		
  }
 
	
}

int WriteToPin(int isNegative, struct segments * segment, int place){
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_All,GPIO_PIN_RESET);
	
	for(int i = 0 ; i < place ; i ++){
		
		if(segment->SEG[0]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
		if(segment->SEG[1]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
		if(segment->SEG[2]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
		if(segment->SEG[3]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
		if(segment->SEG[4]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
		if(segment->SEG[5]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
		if(segment->SEG[6]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
		if(segment->SEG[7]) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
		
		segment = segment->NEXT_SEGMENT;
		
	}		
	
	if(isNegative){
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_SET);
	}
	
	return 0;
	
}

int updateDigit(int temperature, int digit, struct segments * segment0){
	
	setDigitSelectLinesZero();
	setSegmentSelectLinesOne();
	SegmentDecoder(temperature, segment0,0);
	setDigitSelectLinesHigh(digit);
	setSegmentSelectLineHigh(digit, segment0);
	
	return 0;
	
}
int setDigitSelectLinesHigh(int digit){
	
	HAL_GPIO_WritePin(GPIOE, (uint16_t)digit, GPIO_PIN_SET); 
	return 0;
}

int setSegmentSelectLineHigh(int digit, struct segments * segment0){
	struct segments * s = segment0;
	if(s->digit == digit){
		if(s->SEG[0]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
		if(s->SEG[1]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
		if(s->SEG[2]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
		if(s->SEG[3]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
		if(s->SEG[4]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
		if(s->SEG[5]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
		if(s->SEG[6]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
		if(s->SEG[7]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
	}
	else{
		s = s->NEXT_SEGMENT;
	}
	return 0;
}

int setDigitSelectLinesZero(void){
	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	return 0;
}

int setSegmentSelectLinesOne(void){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
	return 0;
}

int CtoF(int temperatureC){
	return temperatureC*34;
}

int SegmentDecoder(int toDecode, struct segments * segment, int * place){
	
	int isNegative = 0;
	
	if(toDecode<0){
		toDecode = toDecode * -1;
		isNegative = 1;
	}
	
	while(toDecode){
		
		segment->VALUE = toDecode % 10;	
		switch(segment->VALUE){
			case 0 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 1;//d
				segment->SEG[4] = 1;//e
				segment->SEG[5] = 1;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
			case 1 :
				segment->SEG[0] = 0;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 0;//d
				segment->SEG[4] = 0;//e
				segment->SEG[5] = 0;//f
				segment->SEG[6] = 0;//g
				segment->SEG[7] = 0;//h
			case 2 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 0;//c
				segment->SEG[3] = 1;//d
				segment->SEG[4] = 1;//e
				segment->SEG[5] = 0;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
			case 3 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 1;//d
				segment->SEG[4] = 0;//e
				segment->SEG[5] = 0;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
			case 4 :
				segment->SEG[0] = 0;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 0;//d
				segment->SEG[4] = 0;//e
				segment->SEG[5] = 1;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
			case 5 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 0;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 1;//d
				segment->SEG[4] = 0;//e
				segment->SEG[5] = 1;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
			case 6 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 0;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 1;//d
				segment->SEG[4] = 1;//e
				segment->SEG[5] = 1;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
			case 7 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 0;//d
				segment->SEG[4] = 0;//e
				segment->SEG[5] = 0;//f
				segment->SEG[6] = 0;//g
				segment->SEG[7] = 0;//h
			case 8 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 1;//d
				segment->SEG[4] = 1;//e
				segment->SEG[5] = 1;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
			case 9 :
				segment->SEG[0] = 1;//a
				segment->SEG[1] = 1;//b
				segment->SEG[2] = 1;//c
				segment->SEG[3] = 0;//d
				segment->SEG[4] = 0;//e
				segment->SEG[5] = 1;//f
				segment->SEG[6] = 1;//g
				segment->SEG[7] = 0;//h
		}
		(*place)++;
		toDecode = toDecode/10;
		segment = segment->NEXT_SEGMENT;
	}
	return isNegative;
}


int TemperatureFormula(int OutADC){
	return (((OutADC-V25)/AVG_SLOPE)+25);
}

void ADC_Setup(void){
	
	ADC_HandleTypeDef adc1;
	//Init all the stuff
	
	//__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_ADC_Init(&adc1);
}

void GPIO_Setup(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//Input pin PA0 for button
	GPIO_InitTypeDef init_gpio;
	init_gpio.Pin = GPIO_PIN_0;
	init_gpio.Mode = GPIO_MODE_INPUT;
	init_gpio.Pull = GPIO_NOPULL;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &init_gpio);
	
	//Output on GPIOB -- Need 13 pins, just decided to hit all of them on for now
	init_gpio.Pin = ((GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6  | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15));
	init_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, &init_gpio);

}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
