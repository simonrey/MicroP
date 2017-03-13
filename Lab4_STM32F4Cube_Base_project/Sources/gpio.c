//All the GPIO stuff goes here

#include "gpio.h"
#include <stdlib.h>
#include <math.h>

void enableClockGPIO(void){
	//Enable GPIO clocks here
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
}

void initKeypadGPIO_1(GPIO_InitTypeDef * gpioInitType){
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_2);
	HAL_GPIO_DeInit(GPIOE,(GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2));

	
	//PA5 - Set as input
	gpioInitType->Pin = ROW0;
	gpioInitType->Mode = GPIO_MODE_IT_FALLING;
	gpioInitType->Pull = GPIO_PULLUP;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, gpioInitType);
		
	//PB4 - Set as input
	gpioInitType->Pin = ROW1;
	gpioInitType->Mode =GPIO_MODE_IT_FALLING;
	gpioInitType->Pull = GPIO_PULLUP;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, gpioInitType);
	
	//PC3 - Set as input
	gpioInitType->Pin = ROW2;
	gpioInitType->Mode = GPIO_MODE_IT_FALLING;
	gpioInitType->Pull = GPIO_PULLUP;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, gpioInitType);

	//PD2 - Set as input
	gpioInitType->Pin = ROW3;
	gpioInitType->Mode = GPIO_MODE_IT_FALLING;
	gpioInitType->Pull = GPIO_PULLUP;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, gpioInitType);
	
	//PE5 to PE2 - Set as output
	gpioInitType->Pin = COL0 | COL1 | COL2| COL3;
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, gpioInitType);
	HAL_GPIO_WritePin(GPIOE,(col0 | col1 | col2 | col3),GPIO_PIN_RESET);
}

void initKeypadGPIO_2(GPIO_InitTypeDef * gpioInitType){
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_2);
	HAL_GPIO_DeInit(GPIOE,(GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2));

	//ROW PA5 set as output
	gpioInitType->Pin = GPIO_PIN_5;
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,gpioInitType);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
		
	//ROW PB4
	gpioInitType->Pin = GPIO_PIN_4;
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,gpioInitType);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);

	//RPW PC3
	gpioInitType->Pin = GPIO_PIN_3;
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC,gpioInitType);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);
		
	//ROW PD2
	gpioInitType->Pin = GPIO_PIN_2;
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD,gpioInitType);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
	
	//PE5 to PE2 - Set as input
	gpioInitType->Pin = (GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2);
	gpioInitType->Mode = GPIO_MODE_INPUT;
	gpioInitType->Pull = GPIO_PULLUP;
	gpioInitType->Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, gpioInitType);
	
}

void initializeDisplayGPIO(GPIO_InitTypeDef * gpioInitType){
	
	//Output on GPIOE -- Digit control lines
	gpioInitType->Pin = ((DIGIT_CONTROL_1 | DIGIT_CONTROL_2 | DIGIT_CONTROL_3 | DIGIT_CONTROL_4));
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLDOWN;
	gpioInitType->Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, gpioInitType);

	//Output on GPIOE -- Segment control lines
	gpioInitType->Pin = ((SEG0 | SEG1 | SEG2 | SEG3 | SEG4 | SEG5 | SEG6 | SEG7));
	gpioInitType->Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitType->Pull = GPIO_PULLUP;
	gpioInitType->Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOE, gpioInitType);
	
}


//Segmennt Display Functions here

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

void resetDigitSelectLines(void){
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DIGIT_CONTROL_4, GPIO_PIN_RESET);
}
void resetSegmentSelectLines(void){
	HAL_GPIO_WritePin(GPIOE, SEG0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, SEG1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, SEG2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, SEG3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, SEG4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, SEG5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, SEG6, GPIO_PIN_RESET);
	
}
void setSegmentSelectLines(int * segmentArray){
	for(int i = 0 ; i < 7 ; i++){
		if(segmentArray[i]) HAL_GPIO_WritePin(GPIOE, segments[i], GPIO_PIN_SET);
	}
}
void setDigitSelectLine(int digit){
	HAL_GPIO_WritePin(GPIOE, pow(digit,2), GPIO_PIN_SET);
}

//Keypad Functions here