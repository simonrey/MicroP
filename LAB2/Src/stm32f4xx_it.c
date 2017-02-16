/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include <math.h>

/* USER CODE BEGIN 0 */
#define V25 0.76
#define AVG_SLOPE 2.5

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/


int updateDigit(int temperature, int digit);
int setDigitSelectLinesHigh(int digit);
int setSegmentSelectLineHigh(int *s);
int setDigitSelectLinesZero(void);
int setSegmentSelectLinesZero(void);
int* SegmentDecoder(int toDecode, int *segArr);

int counterTempUpdate = 0;
int currentDigit = 0;
int currentTemp = 0;

void updateTemp();


void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
	
		// this function is called whenever systick interupt occurs

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();																																	// resets the interupt flag so that new interupt can happen
	// counter++; ????? is this needed???
  /* USER CODE BEGIN SysTick_IRQn 1 */
	//most code should go here so that the timer is reset quickly
	
	if(currentDigit > 3)
	{
		currentDigit = 0;
	}
	if(counterTempUpdate == 500)
	{
//		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4, GPIO_PIN_SET);
	}
	
	if(counterTempUpdate == 1000)
	{
		currentDigit++;
		updateDigit(currentTemp,currentDigit);
		updateTemp();
		printf("Atemp: %i\n",currentTemp);
		counterTempUpdate = 0;
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
		{
			printf("button");
		}
		
//		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4, GPIO_PIN_RESET);

	}

	counterTempUpdate++;


  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/


/* USER CODE BEGIN 1 */
int updateDigit(int temperature, int digit){
	int s[8] = {0};
	int digitValue = (int)(temperature/(pow((double)10,(double)digit))) % 10;
	
	setDigitSelectLinesZero();
	//setSegmentSelectLinesZero();
	SegmentDecoder(digitValue, s);
	setDigitSelectLinesHigh(digit);
	
	return 0;
}
int setDigitSelectLinesHigh(int digit){
	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
	return 0;
}

int setSegmentSelectLineHigh(int *s){

	if(s[0]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	if(s[1]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	if(s[2]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
	if(s[3]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	if(s[4]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	if(s[5]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
	if(s[6]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
	if(s[7]==1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);

	return 0;
}

int setDigitSelectLinesZero(void){
	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	return 0;
}

int setSegmentSelectLinesZero(void){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
	return 0;
}
int* SegmentDecoder(int toDecode, int *segArr){
	switch(toDecode){
		case 0 :
			segArr[0] = 1;//a
			segArr[1] = 1;//b
			segArr[2] = 1;//c
			segArr[3] = 1;//d
			segArr[4] = 1;//e
			segArr[5] = 1;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
		case 1 :
			segArr[0] = 0;//a
			segArr[1] = 1;//b
			segArr[2] = 1;//c
			segArr[3] = 0;//d
			segArr[4] = 0;//e
			segArr[5] = 0;//f
			segArr[6] = 0;//g
			segArr[7] = 0;//h
			break;
		case 2 :
			segArr[0] = 1;//a
			segArr[1] = 1;//b
			segArr[2] = 0;//c
			segArr[3] = 1;//d
			segArr[4] = 1;//e
			segArr[5] = 0;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
		case 3 :
			segArr[0] = 1;//a
			segArr[1] = 1;//b
			segArr[2] = 1;//c
			segArr[3] = 1;//d
			segArr[4] = 0;//e
			segArr[5] = 0;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
		case 4 :
			segArr[0] = 0;//a
			segArr[1] = 1;//b
			segArr[2] = 1;//c
			segArr[3] = 0;//d
			segArr[4] = 0;//e
			segArr[5] = 1;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
		case 5 :
			segArr[0] = 1;//a
			segArr[1] = 0;//b
			segArr[2] = 1;//c
			segArr[3] = 1;//d
			segArr[4] = 0;//e
			segArr[5] = 1;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
		case 6 :
			segArr[0] = 1;//a
			segArr[1] = 0;//b
			segArr[2] = 1;//c
			segArr[3] = 1;//d
			segArr[4] = 1;//e
			segArr[5] = 1;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
		case 7 :
			segArr[0] = 1;//a
			segArr[1] = 1;//b
			segArr[2] = 1;//c
			segArr[3] = 0;//d
			segArr[4] = 0;//e
			segArr[5] = 0;//f
			segArr[6] = 0;//g
			segArr[7] = 0;//h
			break;
		case 8 :
			segArr[0] = 1;//a
			segArr[1] = 1;//b
			segArr[2] = 1;//c
			segArr[3] = 1;//d
			segArr[4] = 1;//e
			segArr[5] = 1;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
		case 9 :
			segArr[0] = 1;//a
			segArr[1] = 1;//b
			segArr[2] = 1;//c
			segArr[3] = 0;//d
			segArr[4] = 0;//e
			segArr[5] = 1;//f
			segArr[6] = 1;//g
			segArr[7] = 0;//h
			break;
	}
	
	setSegmentSelectLineHigh(segArr);

	return segArr;
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
