#ifndef __gpio_H
#define __gpio_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
	 
//CHANGE THESE TO MATCH!
	 
#define DIGIT_CONTROL_1 GPIO_PIN_4
#define DIGIT_CONTROL_2 GPIO_PIN_5
#define DIGIT_CONTROL_3 GPIO_PIN_6
#define DIGIT_CONTROL_4 GPIO_PIN_7
#define SEG0 GPIO_PIN_8
#define SEG1 GPIO_PIN_9
#define SEG2 GPIO_PIN_10
#define SEG3 GPIO_PIN_11
#define SEG4 GPIO_PIN_12
#define SEG5 GPIO_PIN_13
#define SEG6 GPIO_PIN_14
#define SEG7 GPIO_PIN_15
	 
// CHANGE THESE TO MATCH!	 

#define ROW0 GPIO_PIN_0
#define ROW1 GPIO_PIN_1
#define ROW2 GPIO_PIN_2
#define ROW3 GPIO_PIN_3
#define COL0 GPIO_PIN_4
#define COL1 GPIO_PIN_5
#define COL2 GPIO_PIN_6
#define COL3 GPIO_PIN_7
	 

	 
	 
const uint16_t segments[] = {SEG0,SEG1,SEG2,SEG3,SEG4,SEG5,SEG6};

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
	 
void enableClockGPIO(void);
void initKeypadGPIO_1(GPIO_InitTypeDef * gpioInitType);
void initKeypadGPIO_2(GPIO_InitTypeDef * gpioInitType);
void initDisplayGPIO(GPIO_InitTypeDef * gpioInitType);


int SegmentEncoder(int toDecode, int * segArr, int digit);
void setSegmentSelectLinesOne(int * segmentArray);
void setDigitSelectLinesZero(void);
void writeSegments(int * segmentArray);



#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */