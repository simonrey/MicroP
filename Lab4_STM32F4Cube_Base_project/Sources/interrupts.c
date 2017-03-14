
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "interrupts.h"
#include "gpio.h"
#include "main.h"
#include <math.h>

int filterUpdateCount = 0;
int counterDisplayTempUpdate = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  float rawTemp = getReadingADC(); //(float)getRawTemp();
  float updatedTemp = rawTemp * 3.0;
    
  updatedTemp = updatedTemp / 4095.0;
  updatedTemp = updatedTemp - 0.76;
  updatedTemp = updatedTemp / 0.0025;
  updatedTemp = updatedTemp + 25.0;

	setRawTemp(updatedTemp);
}

void DMA2_Stream4_IRQHandler()
{
    HAL_DMA_IRQHandler(getHandleDMA());
}

void ADC_IRQHandler()
{
    HAL_ADC_IRQHandler(getHandleADC());
}

void SysTick_Handler(void){
  /* USER CODE BEGIN SysTick_IRQn 0 */
	
		// this function is called whenever systick interupt occurs

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();																																	// resets the interupt flag so that new interupt can happen
	// counter++; ????? is this needed???
  /* USER CODE BEGIN SysTick_IRQn 1 */
	//most code should go here so that the timer is reset quickly
	
	//cycle through each digit of the display updating it with its appropriate value
	setCurrentDigit(getCurrentDigit() + 1);
	if(getCurrentDigit() > 3)
	{
		setCurrentDigit(1);
	}
	updateDigit(getCurrentDisplayTemp(), getCurrentDigit());
	
	filterUpdateCount++;
	if(filterUpdateCount > 10)
	{
		filter();
		filterUpdateCount = 0;
	}
	
	//every nth tick poll for new temperature
	counterDisplayTempUpdate++;
	if(counterDisplayTempUpdate == 200)
	{
		counterDisplayTempUpdate = 0;
		setCurrentDisplayTemp((int)((getCurrentTemp())*100.0));
		// if the blue button is pressed give results in fahrenheit
//		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
//		{
//			setCurrentDisplayTemp((int)(((getCurrentTemp())*1.8 + 32.0)*100.0));
//		}
//		else
//		{
//			setCurrentDisplayTemp((int)((getCurrentTemp())*100.0));
//		}
	}
  /* USER CODE END SysTick_IRQn 1 */
}