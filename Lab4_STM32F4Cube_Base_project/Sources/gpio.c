//All the GPIO stuff goes here

#include "gpio.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"


int currentDigit = 0;
int currentDisplayTemp = 0;
float currentTemp;

float rawTemp;
float pastTemperatures[50];
int pastTemperaturesIndex = 0;



void enableClockGPIO(void){
	//Enable GPIO clocks here
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
}

void initLED(GPIO_InitTypeDef * handleLED, TIM_HandleTypeDef * handleTIM, TIM_OC_InitTypeDef * initTIM){
	
	handleLED->Pin = (LED0 | LED1 | LED2 | LED3);
	handleLED->Mode = GPIO_MODE_AF_PP;
	handleLED->Pull = GPIO_NOPULL;
	handleLED->Speed = GPIO_SPEED_FREQ_HIGH;
	handleLED->Alternate = GPIO_AF2_TIM4; 
	HAL_GPIO_Init(GPIOD, handleLED);
	
	// init timer
	__TIM4_CLK_ENABLE();
	
	

	handleTIM->Instance = TIM4;
	
	handleTIM->Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
	handleTIM->Init.CounterMode			= TIM_COUNTERMODE_UP;
	handleTIM->Init.Period				= 32;
	handleTIM->Init.Prescaler			    = 1280;
	handleTIM->Init.RepetitionCounter	    = 0;

	HAL_TIM_Base_Init(handleTIM);
    HAL_TIM_Base_Start(handleTIM);
	
	HAL_TIM_PWM_MspInit(handleTIM);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_4);
	//HAL_TIM_ConfigClockSource????
	
	// init pwm channels
	HAL_TIM_PWM_Init(handleTIM);
	

	initTIM->OCMode 			= TIM_OCMODE_PWM1;
	initTIM->OCPolarity 	= TIM_OCPOLARITY_HIGH;
	initTIM->OCIdleState = TIM_OCIDLESTATE_SET;
	initTIM->Pulse				= (16);
	
	HAL_TIM_PWM_ConfigChannel(handleTIM,initTIM,TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(handleTIM,initTIM,TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(handleTIM,initTIM,TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(handleTIM,initTIM,TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_4);
	
}

void initKeypadGPIO_1(GPIO_InitTypeDef * gpioInitType){
	
	HAL_GPIO_DeInit(GPIOE,(ROW0 | ROW1 | ROW2 | ROW3 | COL0 | COL1 | COL2 | COL3));

	
	//Set rows as input
	gpioInitType->Pin = (ROW0 | ROW1 | ROW2 | ROW3);
	gpioInitType->Mode = GPIO_MODE_INPUT;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, gpioInitType);
	
	//Set cols as input
	gpioInitType->Pin = (COL0 | COL1 | COL2| COL3);
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, gpioInitType);
	HAL_GPIO_WritePin(GPIOE,(COL0 | COL1 | COL2 | COL3),GPIO_PIN_SET);
}

void initKeypadGPIO_2(GPIO_InitTypeDef * gpioInitType){
	
	HAL_GPIO_DeInit(GPIOE,(ROW0 | ROW1 | ROW2 | ROW3 | COL0 | COL1 | COL2 | COL3));

	//Set rows as output
	gpioInitType->Pin = (ROW0 | ROW1 | ROW2 | ROW3);
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE,gpioInitType);
	HAL_GPIO_WritePin(GPIOE,(ROW0 | ROW1 | ROW2 | ROW3),GPIO_PIN_SET);
	
	//PE5 to PE2 - Set as input
	gpioInitType->Pin = (COL0 | COL1 | COL2| COL3);
	gpioInitType->Mode = GPIO_MODE_INPUT;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, gpioInitType);
	
}

void initDisplayGPIO(GPIO_InitTypeDef * gpioInitType){
	
	//Output on GPIOE -- Digit control lines
	gpioInitType->Pin = ((DIGIT_CONTROL_1 | DIGIT_CONTROL_2 | DIGIT_CONTROL_3 | DIGIT_CONTROL_4));
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOC, gpioInitType);

	//Output on GPIOE -- Segment control lines
	gpioInitType->Pin = ((SEG0 | SEG1 | SEG2 | SEG3 | SEG4 | SEG5 | SEG6 | SEG7));
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLUP;
	gpioInitType->Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, gpioInitType);
	
}


//Segmennt Display Functions here

int getCurrentDisplayTemp(void){
	return currentDisplayTemp;
}
void setCurrentDisplayTemp(int newCurrentDisplayTemp){
	currentDisplayTemp = newCurrentDisplayTemp;
}


void filter(void){
			pastTemperatures[pastTemperaturesIndex] = getRawTemp();
			pastTemperaturesIndex++;
			if(pastTemperaturesIndex > 50)
			{
				pastTemperaturesIndex = 0;
			}
			float scratchTemp = 0;
			for(int i = pastTemperaturesIndex; i < 51; i++)
			{
				scratchTemp = scratchTemp + pastTemperatures[i]*coeffsArrayTemp[i - pastTemperaturesIndex];
			}
			
			for(int i = 0; i < pastTemperaturesIndex; i++)
			{
				scratchTemp = scratchTemp + pastTemperatures[i]*coeffsArrayTemp[i + (51 - pastTemperaturesIndex)];
			}
			setCurrentTemp(scratchTemp);
}


int updateDigit(int temperature, int digit){
	int s[8] = {0};
	int digitValue = (int)(temperature/(pow((double)10,(double)(digit)))) % 10; //isolates the digit to be updated of the temperature
	
	resetDigitSelectLines(); //sets all select lines to zero so that only one digit is on at a time
	SegmentEncoder(digitValue, s, digit);
	setDigitSelectLines(digit);
	
	return 0;
}
float getRawTemp(void){
	return rawTemp;
}
void setRawTemp(float t){
	rawTemp = t;
}

float getCurrentTemp(void){
	return currentTemp;
}
void setCurrentTemp(float t){
	currentTemp = t;
}

int getCurrentDigit(void){
	return currentDigit;
}
void setCurrentDigit(int newCurrentDigit){
	currentDigit = newCurrentDigit;
}
int SegmentEncoder(int toDecode, int * segArr, int digit){
	
	int isNegative = 0;
	toDecode = (toDecode/(10^digit)) % 10;
	if(toDecode<0){
		toDecode = toDecode * -1;
		isNegative = 1;
	}
		switch(toDecode){
			case 0 :
				segArr[0] = 1;//a
				segArr[1] = 1;//b
				segArr[2] = 1;//c
				segArr[3] = 1;//d
				segArr[4] = 1;//e
				segArr[5] = 1;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
			case 1 :
				segArr[0] = 0;//a
				segArr[1] = 1;//b
				segArr[2] = 1;//c
				segArr[3] = 0;//d
				segArr[4] = 0;//e
				segArr[5] = 0;//f
				segArr[6] = 0;//g
				segArr[7] = 0;//h
				break;
			case 2 :
				segArr[0] = 1;//a
				segArr[1] = 1;//b
				segArr[2] = 0;//c
				segArr[3] = 1;//d
				segArr[4] = 1;//e
				segArr[5] = 0;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
			case 3 :
				segArr[0] = 1;//a
				segArr[1] = 1;//b
				segArr[2] = 1;//c
				segArr[3] = 1;//d
				segArr[4] = 0;//e
				segArr[5] = 0;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
			case 4 :
				segArr[0] = 0;//a
				segArr[1] = 1;//b
				segArr[2] = 1;//c
				segArr[3] = 0;//d
				segArr[4] = 0;//e
				segArr[5] = 1;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
			case 5 :
				segArr[0] = 1;//a
				segArr[1] = 0;//b
				segArr[2] = 1;//c
				segArr[3] = 1;//d
				segArr[4] = 0;//e
				segArr[5] = 1;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
			case 6 :
				segArr[0] = 1;//a
				segArr[1] = 0;//b
				segArr[2] = 1;//c
				segArr[3] = 1;//d
				segArr[4] = 1;//e
				segArr[5] = 1;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
			case 7 :
				segArr[0] = 1;//a
				segArr[1] = 1;//b
				segArr[2] = 1;//c
				segArr[3] = 0;//d
				segArr[4] = 0;//e
				segArr[5] = 0;//f
				segArr[6] = 0;//g
				segArr[7] = 0;//h
				break;
			case 8 :
				segArr[0] = 1;//a
				segArr[1] = 1;//b
				segArr[2] = 1;//c
				segArr[3] = 1;//d
				segArr[4] = 1;//e
				segArr[5] = 1;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
			case 9 :
				segArr[0] = 1;//a
				segArr[1] = 1;//b
				segArr[2] = 1;//c
				segArr[3] = 0;//d
				segArr[4] = 0;//e
				segArr[5] = 1;//f
				segArr[6] = 1;//g
				segArr[7] = 0;//h
				break;
		}
		return isNegative;
}

void setSegmentSelectLines(int * segmentArray){
	for(int i = 0 ; i < 7 ; i++){
		if(segmentArray[i]) HAL_GPIO_WritePin(GPIOE, segments[i], GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOE,segments[i], GPIO_PIN_RESET);
	}
}

void resetDigitSelectLines(void){
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_4, GPIO_PIN_RESET);
}
void setDigitSelectLines(int digit){
	//sets the appropriate digit select line high
	switch(digit)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);		
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);		
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
			break;
	}
}

void display(void){

}


//Keypad Functions here
