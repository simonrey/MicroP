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
int digitCount = 0;
int pitch[3]={1,2,3};
int roll[3];
int interruptDisable;

TIM_HandleTypeDef tim_handle;
GPIO_InitTypeDef init_gpio;

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

void enableInterrupts(){
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
	HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 1);
	HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 1);
	HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void disableInterrupts(){
	HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	HAL_NVIC_ClearPendingIRQ(EXTI4_IRQn);
	HAL_NVIC_ClearPendingIRQ(EXTI3_IRQn);
	HAL_NVIC_ClearPendingIRQ(EXTI2_IRQn);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);		
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
}

void getKey(int pin){
	int r = -1;
	int c = -1;
	
	while(r < 0){
		if(!HAL_GPIO_ReadPin(GPIOD,row0)){
			r = 0;
		}
		if(!HAL_GPIO_ReadPin(GPIOC,row1)){
			r = 1;
		}
		if(!HAL_GPIO_ReadPin(GPIOB,row2)){
			r = 2;
		}
		if(!HAL_GPIO_ReadPin(GPIOA,row3)){
			r = 3;
		}
	}
	disableInterrupts();
	reInitializeGPIO();
	while(c < 0){
		if(!HAL_GPIO_ReadPin(GPIOE,col0)){
			c = 0;
		}
		if(!HAL_GPIO_ReadPin(GPIOE,col1)){
			c = 1;
		}
		if(!HAL_GPIO_ReadPin(GPIOE,col2)){
			c = 2;
		}
		if(!HAL_GPIO_ReadPin(GPIOE,col3)){
			c = 3;
		}
	}
	getKeyPressed(r,c);
	
	initializeGPIO(0);
	
	
}

void initializeGPIO(int initialSetup){
	
	

	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_2);
	HAL_GPIO_DeInit(GPIOE,(GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2));
	
	//Mapping
	//B8 - PA5
	//B7 - PB4
	//B6 - PC3
	//B5 - PD2
	//B4 - PE5
	//B3 - PE4
	//B2 - PE3
	//B1 - PE2
	
	//PA5 - Set as input
	init_gpio.Pin = row3;
	init_gpio.Mode = GPIO_MODE_IT_FALLING;
	init_gpio.Pull = GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &init_gpio);
		
	//PB4 - Set as input
	init_gpio.Pin = row2;
	init_gpio.Mode =GPIO_MODE_IT_FALLING;
	init_gpio.Pull = GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init_gpio);
	
	//PC3 - Set as input
	init_gpio.Pin = row1;
	init_gpio.Mode = GPIO_MODE_IT_FALLING;
	init_gpio.Pull = GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &init_gpio);

	//PD2 - Set as input
	init_gpio.Pin = row0;
	init_gpio.Mode = GPIO_MODE_IT_FALLING;
	init_gpio.Pull = GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &init_gpio);
	
	//PE5 to PE2 - Set as output
	init_gpio.Pin = col0 | col1 | col2 | col3;
	init_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &init_gpio);
	HAL_GPIO_WritePin(GPIOE,(col0 | col1 | col2 | col3),GPIO_PIN_RESET);

	disableInterrupts();
	enableInterrupts();	
}

void reInitializeGPIO(void){
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_2);
	HAL_GPIO_DeInit(GPIOE,(GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2));
	
	//Mapping
	//B8 - PA5
	//B7 - PB4
	//B6 - PC3
	//B5 - PD2
	//B4 - PE5
	//B3 - PE4
	//B2 - PE3
	//B1 - PE2

	//ROW PA5 set as output
	init_gpio.Pin = GPIO_PIN_5;
	init_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&init_gpio);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
		
	//ROW PB4
	init_gpio.Pin = GPIO_PIN_4;
	init_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,&init_gpio);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);

	//RPW PC3
	init_gpio.Pin = GPIO_PIN_3;
	init_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC,&init_gpio);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);
		
	//ROW PD2
	init_gpio.Pin = GPIO_PIN_2;
	init_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	init_gpio.Pull = GPIO_PULLDOWN;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD,&init_gpio);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	
	//PE5 to PE2 - Set as input
	init_gpio.Pin = (GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2);
	init_gpio.Mode = GPIO_MODE_INPUT;
	init_gpio.Pull = GPIO_PULLUP;
	init_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &init_gpio);
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
	
//	pitch = malloc(3*sizeof(int));
//	roll = malloc(3*sizeof(int));
	digitCount = 0;
}

void deInitPitchRoll(){
	free(pitch);
	free(roll);
}

void getKeyPressed(int row, int col){
	
	int key = row*4+col;
	switch(key){
		case 0 :
			key = 1;
			break;
		case 1 :
			key = 2;
			break;
		case 2:
			key = 3;
			break;
		case 3 :
			key = 10;
			break;
		case 4:
			key = 4;
			break;
		case 5:
			key = 5;
			break;
		case 6:
			key = 6;
			break;
		case 7 :
			key = 11;
			break;
		case 8 :
			key = 7;
			break;
		case 9 :
			key = 8;
			break;
		case 10 :
			key = 9;
			break;
		case 11 :
			key = 12;
			break;
		case 12 :
			key = -1;
			break;
		case 13 :
			key = 0;
			break;
		case 14 :
			key = -2;
			break;
		case 15 :
			key = 13;
			break;
	}
	printf("%i",key);
	
//	
//	if(digitCount>2){
//		if(key == -2){
//			digitCount = 0;
//		}
//		if(key == -1){
//			digitCount--;
//		}
//		else{
//			roll[digitCount-3] = key;
//			digitCount++;
//		}
//	}
//	
//	if(digitCount<3){
//		if(key == -2){
//			digitCount = 3;
//			printPitch();
//		}
//		if(key == -1){
//			digitCount--;
//		}
//		else{
//			pitch[digitCount] = key;
//			digitCount++;
//		}
//	}
	
}

void printPitch(){
	
	printf("P[0] = %i\nP[1] = %i\nP[2] = %i\n", pitch[0], pitch[1], pitch[2]);
}


/* USER CODE END 1 */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
