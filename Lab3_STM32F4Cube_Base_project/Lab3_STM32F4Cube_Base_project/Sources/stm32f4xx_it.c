/**
  ******************************************************************************
  * @file    HAL/HAL_TimeBase/Src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version   V1.2.4
  * @date      13-November-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
#include "main.h"
#include "stm32f4xx_it.h"
#include "gpio.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_EXTI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int keyPressed = 0;
uint16_t row;
uint16_t col;
int timeExpired = 0;
int rowCaptured = 0;
int colCaptured = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
}

void TIM2_IRQHandler(){
	interruptResetTimer();
	timeExpired = 1;
}

void EXTI9_5_IRQHandler(){
	startResetTimer();
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);

	
	
//	if(rowCaptured == 1 && colCaptured == 0){
//		col = 3;
//		colCaptured = 1;
//	}	
//	if(rowCaptured == 0 && colCaptured == 0){
//		row = 3;
//		rowCaptured = 1;
//	}
	
	
	//Row determination
//	if(keyPressed == 0){
//		//startResetTimer();
//		row = 3;
//		keyPressed++;
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
//	}
//	//Column Determination
//	else if(keyPressed == 1){
//		col = 3;
//		keyPressed++;
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
//	}
}

void EXTI4_IRQHandler(){
	
//	if(rowCaptured == 1 && colCaptured == 0){
//		col = 2;
//		colCaptured = 1;
//	}
//	if(rowCaptured == 0 && colCaptured == 0){
//		row = 2;
//		rowCaptured = 1;
//	}
	
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
	
	
	
	//stopResetTimer();
	
	//Row determination
//	if(keyPressed == 0){
//		row = 2;
//		keyPressed++;
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
//	}
//	
//	//Column Determination
//	else if(keyPressed == 1){
//		col = 2;
//		keyPressed++;
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
//	}
}

void EXTI3_IRQHandler(){
	
//	if(rowCaptured == 1 && colCaptured == 0){
//		col = 1;
//		colCaptured = 1;
//	}
//	if(rowCaptured == 0 && colCaptured == 0){
//		row = 1;
//		rowCaptured = 1;
//	}
//	
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
	
	//stopResetTimer();
	
	//Row determination
//	if(keyPressed == 0){
//		row = 1;
//		keyPressed++;
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
//	}
//	
//	//Column Determinuation
//	else if(keyPressed == 1){
//		col = 1;
//		keyPressed++;
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
//	}
}
	
void EXTI2_IRQHandler(){

//	if(rowCaptured == 1 && colCaptured == 0){
//		col = 0;
//		colCaptured = 1;
//	}
//	if(rowCaptured == 0 && colCaptured == 0){
//		row = 0;
//		rowCaptured = 1;
//	}
//		
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
	
	//Row determination
//	if(keyPressed == 0){
//		row = 0;
//		keyPressed++;
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
//	}
//	//Column Determination
//	else if(keyPressed == 1){
//		if(timeExpired && (row == 3)){
//			initializePitchRoll(timeExpired);
//			timeExpired = 0;
//			keyPressed = 0;
//			stopResetTimer();
//			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
//		}
//		else{
//			col = 0;
//			keyPressed++;
//			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
//		}
//	}
	
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	
	getKey(GPIO_Pin);
	
//	if(rowCaptured && colCaptured){
//		getKeyPressed(row, col);
//		rowCaptured = 0;
//		colCaptured = 0;
//		initializeGPIO(0);
//	}
//	else{
//		reInitializeGPIO();
//	}
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
void EXTI0_IRQHandler(void) //HAL_GPIO_EXTI_IRQHandler
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	
}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	printf("callback from accellerometer interrupt \n");
//	float acceleration[3];
//	LIS3DSH_ReadACC(&acceleration);
//	printf("Acceleration: %f,%f,%f \n",acceleration[0],acceleration[1],acceleration[2]);
//}
/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
