/*******************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "gpio.h"
#include "adc.h"
#include "threads.h"

extern void initializeLED_IO			(void);
extern void start_Thread_LED			(void);
extern void Thread_LED(void const *argument);
extern osThreadId tid_Thread_LED;

uint32_t readingADC[1];
	
//All handles
GPIO_InitTypeDef keypadGPIO;
GPIO_InitTypeDef displayGPIO;
ADC_HandleTypeDef handleADC;
ADC_ChannelConfTypeDef channelADC;
DMA_HandleTypeDef handleDMA;
SPI_HandleTypeDef handleSPI;
TIM_HandleTypeDef handleTIM;
TIM_OC_InitTypeDef initTIM;
GPIO_InitTypeDef handleLED;

//Accelerometer Variables
float rollGoal = 0;
float pitchGoal = 0;

//Project Mode
int mode;

//Keypad Variables
int isDisplayingTargetAngle 			= 0;
int tempTargetRollAngle 					= 0;
int tempTargetPitchAngle					= 0;
int	currentTargetRollAngle				= 0;
int currentTargetPitchAngle				= 0;

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

//HANDLE ACCESSORS
uint32_t getReadingADC(void){
	return readingADC[0]; 
}
uint32_t * getReadingADCLocation(void){
	return readingADC;
}

SPI_HandleTypeDef * getHandleSPI(void){
	return &handleSPI;
}

ADC_HandleTypeDef * getHandleADC(void){
	return &handleADC;
}
DMA_HandleTypeDef * getHandleDMA(void){
	return &handleDMA;
}
ADC_ChannelConfTypeDef * getHandleADCChannel(void){
	return &channelADC;
}
TIM_HandleTypeDef * getHandleTIM(void){
	return &handleTIM;
}
TIM_OC_InitTypeDef * getInitTIM(void){
	return &initTIM;
}
GPIO_InitTypeDef * getKeypadGPIO(void){
	return &keypadGPIO;
}

//Variable ACCESSORS
float getRollGoal(void){
	return rollGoal;
}
float getPitchGoal(void){
	return pitchGoal;
}
int getMode(void){
	return mode;
}
int getIsDisplayingTargetAngle(void){
	return isDisplayingTargetAngle;
}
int getTempTargetRollAngle(void){
	return tempTargetRollAngle;
}
int getTempTargetPitchAngle(void){
	return tempTargetPitchAngle;
}
int getCurrentTargetRollAngle(void){
	return currentTargetRollAngle;
}
int getCurrentTargetPitchAngle(void){
	return currentTargetPitchAngle;
}

//Variable Mutators
void setIsDisplayingTargetAngle(int newValue){
	isDisplayingTargetAngle = newValue;
}
void setTempTargetRollAngle(int newValue){
	tempTargetRollAngle = newValue;
}
void setTempTargetPitchAngle(int newValue){
	tempTargetPitchAngle = newValue;
}
void setCurrentTargetRollAngle(int newValue){
	currentTargetRollAngle = newValue;
}
void setCurrentTargetPitchAngle(int newValue){
	currentTargetPitchAngle = newValue;
}


int main (void){
	
	osThreadId tempThread;
	osThreadId accelThread;
	osThreadId keypadThread;
	osThreadId displayThread;
	
  osKernelInitialize();                     /* initialize CMSIS-RTOS          */
  HAL_Init();                               /* Initialize the HAL Library     */
  SystemClock_Config();                     /* Configure the System Clock     */

	/* User codes goes here*/
	
	enableClockGPIO();
	initDisplayGPIO(&displayGPIO);
	initKeypadGPIO_1(&keypadGPIO);
	initLED(&handleLED, &handleTIM, &initTIM);
	
	/*
	Temperature stuff starts on the initialization of the ADC
	startTempThread() takes the tempThreadId and returns it again so main could use it in the parent thread
	if necessary. The temperature stuff should work this way however the GPIO pins need to be set right.
	*/
	//initADC(&handleADC,&channelADC,&handleDMA,readingADC);
	
	tempThread = startTempThread(tempThread);
	accelThread = startAccelThread(accelThread);
	displayThread = startDisplayThread(displayThread);
	keypadThread = startKeypadThread(keypadThread);
	if((tempThread || accelThread || displayThread)==NULL){
		printf("Something went wrong");
	}
	
	
  initializeLED_IO();                       /* Initialize LED GPIO Buttons    */
  start_Thread_LED();                       /* Create LED thread              */
	/* User codes ends here*/
  
	osKernelStart();                          /* start thread execution         */
	
	return 0;
}
