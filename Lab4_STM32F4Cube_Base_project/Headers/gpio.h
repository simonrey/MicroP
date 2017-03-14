#ifndef __gpio_H
#define __gpio_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//GPIOC
#define DIGIT_CONTROL_1 GPIO_PIN_1
#define DIGIT_CONTROL_2 GPIO_PIN_2
#define DIGIT_CONTROL_3 GPIO_PIN_4
#define DIGIT_CONTROL_4 GPIO_PIN_5
//GPIOB	 
#define SEG0 GPIO_PIN_0
#define SEG1 GPIO_PIN_1
#define SEG2 GPIO_PIN_2
#define SEG3 GPIO_PIN_4
#define SEG4 GPIO_PIN_5
#define SEG5 GPIO_PIN_7
#define SEG6 GPIO_PIN_8
#define SEG7 GPIO_PIN_11
	 
//GPIOE
#define ROW0 GPIO_PIN_4
#define ROW1 GPIO_PIN_5
#define ROW2 GPIO_PIN_6
#define ROW3 GPIO_PIN_7
#define COL0 GPIO_PIN_8
#define COL1 GPIO_PIN_9
#define COL2 GPIO_PIN_10
#define COL3 GPIO_PIN_11
	 

	 
	 
static const uint16_t segments[] = {SEG0,SEG1,SEG2,SEG3,SEG4,SEG5,SEG6};

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
	 
void enableClockGPIO(void);
void initKeypadGPIO_1(GPIO_InitTypeDef * gpioInitType);
void initKeypadGPIO_2(GPIO_InitTypeDef * gpioInitType);
void initDisplayGPIO(GPIO_InitTypeDef * gpioInitType);

void setCurrentDigit(int newCurrentDigit);
int getCurrentDigit(void);
int updateDigit(int temperature, int digit);

int getCurrentDisplayTemp(void);
void setCurrentDisplayTemp(int newCurrentDisplayTemp);

void filter(void);

void setRawTemp(float t);
float getRawTemp(void);

int SegmentEncoder(int toDecode, int * segArr, int digit);
void setSegmentSelectLines(int * segmentArray);
void resetDigitSelectLines(void);
void setDigitSelectLines(int digit);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */