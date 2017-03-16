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
	 
//GPIOE - Keypad
#define ROW0 GPIO_PIN_4
#define ROW1 GPIO_PIN_5
#define ROW2 GPIO_PIN_6
#define ROW3 GPIO_PIN_7
#define COL0 GPIO_PIN_8
#define COL1 GPIO_PIN_9
#define COL2 GPIO_PIN_10
#define COL3 GPIO_PIN_11

//GPIOD
#define LED0 GPIO_PIN_12
#define LED1 GPIO_PIN_13
#define LED2 GPIO_PIN_14
#define LED3 GPIO_PIN_15	 
	 
static const uint16_t segments[] = {SEG0,SEG1,SEG2,SEG3,SEG4,SEG5,SEG6};
static const float coeffsArrayTemp[51] = {0.000950456065525981, 0.0032982880300802, 0.00449132702555116, -0.00147679039173661, -0.0173486722707884, -0.0322288371748662, -0.020649059911765, 0.0395767340724028, 0.141361472999714, 0.24066169296412, 0.282151127183525, 0.24066169296412, 0.141361472999714, 0.0395767340724028, -0.020649059911765, -0.0322288371748662, -0.0173486722707884, -0.00147679039173661, 0.00449132702555116, 0.0032982880300802, 0.000950456065525981};

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

//All GPIO initialization
void enableClockGPIO(void);
void initKeypadGPIO_1(GPIO_InitTypeDef * gpioInitType);
void initKeypadGPIO_2(GPIO_InitTypeDef * gpioInitType);
void initDisplayGPIO(GPIO_InitTypeDef * gpioInitType);
void initLED(GPIO_InitTypeDef * handleLED, TIM_HandleTypeDef * handleTIM, TIM_OC_InitTypeDef * initTIM);

//These are holdovers from the previous labs
void setCurrentDigit(int newCurrentDigit);
int getCurrentDigit(void);
int updateDigit(int temperature, int digit);

	
//Setter and getter for temps
int getCurrentDisplayTemp(void);
void setCurrentDisplayTemp(int newCurrentDisplayTemp);
void setRawTemp(float t);
float getRawTemp(void);

//Temperature functions
void filter(void);


//Display functions
int SegmentEncoder(int toDecode, int * segArr, int digit);
void setSegmentSelectLines(int * segmentArray);
void resetDigitSelectLines(void);
void setDigitSelectLines(int digit);

//Keypad functions
int getRowSelected(void);
int getColumnSelected(void);
int powerOfTen(int exponent);
int deleteLastNumber(int angle);
//Project mode
void display(void);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

