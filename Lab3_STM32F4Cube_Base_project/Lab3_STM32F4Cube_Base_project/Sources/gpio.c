/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
#include <stdlib.h>
/* USER CODE BEGIN 0 */
int digitCount;
int * pitch;
int * roll;

TIM_HandleTypeDef tim_handle;

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

void initializeGPIO(int keyPressed){
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_2);
	HAL_GPIO_DeInit(GPIOE,(GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2));
	
	GPIO_InitTypeDef init_gpio;
	
	
	//PA5 - Set as input/output
	init_gpio.Pin = (GPIO_PIN_5);
	init_gpio.Mode = keyPressed ? GPIO_MODE_IT_FALLING : GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = keyPressed ? GPIO_PULLUP : GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &init_gpio);
	
	//PE5 - Set as output/input
	init_gpio.Pin = (GPIO_PIN_5);
	init_gpio.Mode = keyPressed ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_IT_RISING;
	init_gpio.Pull = keyPressed ? GPIO_PULLDOWN : GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, &init_gpio);
		
	//PB4 - Set as input/output
	init_gpio.Pin = (GPIO_PIN_4);
	init_gpio.Mode = keyPressed ? GPIO_MODE_IT_FALLING : GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = keyPressed ? GPIO_PULLUP : GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &init_gpio);
		
	//PE4 - Set as output/input
	init_gpio.Pin = (GPIO_PIN_4);
	init_gpio.Mode = keyPressed ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_IT_RISING;
	init_gpio.Pull = keyPressed ? GPIO_PULLDOWN : GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, &init_gpio);
	
	//PC3 - Set as input/output
	init_gpio.Pin = (GPIO_PIN_3);
	init_gpio.Mode = keyPressed ? GPIO_MODE_IT_FALLING : GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = keyPressed ? GPIO_PULLUP : GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOC, &init_gpio);
	
	//PE3 - Set as output/input
	init_gpio.Pin = (GPIO_PIN_3);
	init_gpio.Mode = keyPressed ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_IT_RISING;
	init_gpio.Pull = keyPressed ? GPIO_PULLDOWN : GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, &init_gpio);
		
	//PD2 - Set as input/output
	init_gpio.Pin = (GPIO_PIN_2);
	init_gpio.Mode = keyPressed ? GPIO_MODE_IT_FALLING : GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = keyPressed ? GPIO_PULLUP : GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOD, &init_gpio);
		
	//PE2 - Set as output/input
	init_gpio.Pin = (GPIO_PIN_2);
	init_gpio.Mode = keyPressed ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_IT_RISING;
	init_gpio.Pull = keyPressed ? GPIO_PULLDOWN : GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, &init_gpio);
	
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	
	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	
	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	
}

void initializeResetTimer(void){
		
	__TIM2_CLK_ENABLE();

	tim_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle.Init.Period = 665;
	tim_handle.Init.Prescaler = (42000-1);
	tim_handle.Instance = TIM2;
	
	HAL_TIM_Base_Init(&tim_handle);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
}

void interruptResetTimer(){
	HAL_TIM_IRQHandler(&tim_handle);
}

void startResetTimer(void){
	HAL_TIM_Base_Start_IT(&tim_handle);
}
void stopResetTimer(void){
	HAL_TIM_Base_Stop(&tim_handle);
	HAL_TIM_Base_DeInit(&tim_handle);
	initializeResetTimer();
}

void initializePitchRoll(int reInit){
	
	if(reInit){
		deInitPitchRoll();
	}
	
	pitch = malloc(3*sizeof(int));
	roll = malloc(3*sizeof(int));
	digitCount = 0;
}

void deInitPitchRoll(){
	free(pitch);
	free(roll);
}

void getKeyPressed(int row, int col){
	
	int key = row*3+col;
	switch(key){
		case 3 :
			key = 10;
			break;
		case 7 :
			key = 11;
			break;
		case 11 :
			key = 12;
			break;
		case 15 :
			key = 13;
			break;
		case 12 :
			key = -1;
			break;
		case 14 :
			key = -2;
			break;
	}
	
	if(digitCount>2){
		if(key == -2){
			digitCount = 0;
		}
		if(key == -1){
			digitCount--;
		}
		else{
			roll[digitCount-3] = key;
			digitCount++;
		}
	}
	
	if(digitCount<3){
		if(key == -2){
			digitCount = 3;
		}
		if(key == -1){
			digitCount--;
		}
		else{
			pitch[digitCount] = key;
			digitCount++;
		}
	}	
}



/* USER CODE END 1 */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
