/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "gpio.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	/* Init GPIO blocks*/
	InitializeGPIO();
	
  /* Initialize all configured peripherals */
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriorityGrouping(2);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 2,2);


	SPI_HandleTypeDef hspi;
	HAL_SPI_MspInit(&hspi); // sets up gpio stuff for the SPI config and accelerometer interrupt pin

	LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
	LIS3DSH_InitStruct.AA_Filter_BW							= LIS3DSH_AA_BW_50;
	LIS3DSH_InitStruct.Axes_Enable							= LIS3DSH_XYZ_ENABLE;
	LIS3DSH_InitStruct.Continous_Update						= LIS3DSH_ContinousUpdate_Enabled;
	LIS3DSH_InitStruct.Full_Scale							= LIS3DSH_FULLSCALE_2;
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate					= LIS3DSH_DATARATE_25;
	LIS3DSH_InitStruct.Self_Test							= LIS3DSH_SELFTEST_NORMAL;
	LIS3DSH_Init(&LIS3DSH_InitStruct); // must be called before LIS3DSH_DataReadyInterruptConfig, since this also sets up the SPI configuration

	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_IntConfigStruct;
	LIS3DSH_IntConfigStruct.Dataready_Interrupt	= LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	LIS3DSH_IntConfigStruct.Interrupt_signal	= LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_IntConfigStruct.Interrupt_type		= LIS3DSH_INTERRUPT_REQUEST_LATCHED;
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_IntConfigStruct);

	while (1){
	}
}

/** System Clock Configuration*/
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif

