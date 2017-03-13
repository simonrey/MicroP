#include "adc.h"


void initADC(ADC_HandleTypeDef * handleADC){
	HAL_ADC_Init(handleADC);
}