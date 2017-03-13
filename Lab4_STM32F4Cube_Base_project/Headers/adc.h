#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


void initADC(ADC_HandleTypeDef * handleADC);
	 
#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

