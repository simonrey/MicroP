
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "interrupts.h"
#include "accelerometer.h"
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

void EXTI0_IRQHandler(void) //HAL_GPIO_EXTI_IRQHandler
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
